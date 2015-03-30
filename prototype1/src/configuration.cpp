#include <configuration.h>

#include <cstdlib>
#include <cstring>
#include <fstream>
#include <iostream>
#include <istream>
#include <memory>
#include <cerrno>

#include "commandcontroller.h"
#include "common.h"
#include "difflightbardetector.h"
#include "defaultreconstructor.h"
#include "edgelightbardetector.h"
#include "freelightbardetector.h"
#include "lightbardetector.h"
#include "reconstructor.h"
#include "scancontroller.h"

using std::cerr;
using std::cin;
using std::endl;
using std::exit;
using std::ifstream;
using std::istream;
using std::make_shared;
using std::shared_ptr;


static int argc = 0;
static int argi = 0;
static char const* const* argv = nullptr;
static const char* nextParam = nullptr;
static const char* option = nullptr;
static char optionPlace[3] = "- ";

bool Configuration::createCommands = false;
bool Configuration::debugLightbar = false;
bool Configuration::debugCamera = false;
bool Configuration::debugHeightmap = false;
bool Configuration::verbose = false;
bool Configuration::waitKey = false;
shared_ptr<Controller> Configuration::controller{make_shared<CommandController>()};
shared_ptr<LightBarDetector> Configuration::lightBarDetector{make_shared<DiffLightBarDetector>()};
shared_ptr<Reconstructor> Configuration::reconstructor{make_shared<DefaultReconstructor>()};
shared_ptr<istream> Configuration::inputStream{&cin, [](istream*){}};
int Configuration::captureDevice = -1;
DeviceConfiguration Configuration::deviceConfiguration{0.0, {0.0, 0.0, 0.0}, 0.0, 0.0, IDENTITY44d};
const char *Configuration::savePrefix = nullptr;

void Configuration::init(int argc, char* argv[])
{
	::argc = argc;
	::argv = argv;

	int argumentCount = 0;
	bool no_option = false;
	for (argi = 1; argi < argc; ++argi) {
		const char *arg = argv[argi];

		if (equal(arg, "--"))
		{
			no_option = true;
		}
		else if (no_option || arg[0] != '-')
		{
			// handle argument
			if (++argumentCount == 1) {
				shared_ptr<ifstream> in = make_shared<ifstream>(arg);
				if (!in->is_open()) {
					cerr << "Unable to open file: " << arg << endl;
					exit(EXIT_FAILURE);
				}
				inputStream = in;
			} else {
				cerr << "Too many arguments." << endl << endl;
				helpAndExit();
			}
		}
		else
		{
			if (arg[1] != '-') {
				for (int j = 1; arg[j] != '\0'; ++j) {
					char op = arg[j];
					optionPlace[1] = op;
					option = optionPlace;

					if (arg[j + 1] == '\0') {
						nextParam = nullptr;
						handleOption(op);
					} else {
						nextParam = &arg[j + 1];
						handleOption(op);
						if (nextParam == nullptr) {
							break;
						}
					}
				}
			} else {
				option = arg;
				if (equal(arg, "--help")) {
					handleOption('h');
				} else if (equal(arg, "--create-commands")) {
					createCommands = true;
					if (savePrefix == nullptr)
						savePrefix = "";
				} else {
					handleOption('-');
				}
			}
		}
	}
}

bool Configuration::equal(const char *str, const char *op)
{
	return std::strcmp(str, op) == 0;
}

const char* Configuration::getParam()
{
	if (nextParam != nullptr) {
		const char* param = nextParam;
		nextParam = nullptr;
		return param;
	} else if (++argi < argc) {
		return argv[argi];
	} else {
		cerr << "Missing parameter for option: " << option << endl << endl;
		helpAndExit();
	}
	return nullptr;
}

void Configuration::handleOption(const char op)
{
	switch (op) {
	case 'h':
	{
		helpAndExit(EXIT_SUCCESS);
	}
	case 'H':
	{
		debugHeightmap = true;
		break;
	}
	case 'l':
	{
		debugLightbar = true;
		break;
	}
	case 'k':
	{
		waitKey = true;
		break;
	}
	case 'L':
	{
		const char* lined = getParam();
		if (equal(lined, "default") || equal(lined, "diff")) {
			lightBarDetector = make_shared<DiffLightBarDetector>();
		} else if (equal(lined, "free")) {
			lightBarDetector = make_shared<FreeLightBarDetector>();
		} else if (equal(lined, "edge")) {
			lightBarDetector = make_shared<EdgeLightBarDetector>();
		} else {
			cerr << "Unknown line detector: " << lined << endl << endl;
			helpAndExit();
		}
		break;
	}
	case 'R':
	{
		const char* rec = getParam();
		if (equal(rec, "default")) {
			reconstructor = make_shared<DefaultReconstructor>();
		} else {
			cerr << "Unknown reconstructor: " << rec << endl << endl;
			helpAndExit();
		}
		break;
	}
	case 'v':
	{
		verbose = true;
		break;
	}
	case 'c':
	{
		errno = 0;
		captureDevice = strtol(getParam(), nullptr, 10);
		controller = make_shared<ScanController>();
		if(errno)
		{
			cerr << errno << ": " << strerror(errno) << endl;
			helpAndExit();
		}
		break;
	}
	case 'C':
	{
		debugCamera = true;
		break;
	}
	case 'd':
	{
		errno = 0;
		deviceConfiguration.fov = strtod(getParam(), nullptr) * M_PI/180;
		deviceConfiguration.projectorSkew = strtod(getParam(), nullptr) * M_PI/180;
		deviceConfiguration.projectorPos = {strtod(getParam(), nullptr), strtod(getParam(), nullptr), strtod(getParam(), nullptr)};
		if(errno)
		{
			cerr << strerror(errno) << endl;
			helpAndExit();
		}
		break;
	}
	case 's':
	{
		savePrefix = getParam();
		break;
	}
	default:
		cerr << "Invalid option: " << option << endl << endl;
		helpAndExit();
	}
}

bool Configuration::helpAndExit(int exitCode)
{
	cerr << "Usage: " << argv[0] << " [options...] [file]" << endl;
	cerr << endl;
	cerr << "file: File with commands to execute." << endl;
	cerr << "      Program will use stdin by default." << endl;
	cerr << endl;
	cerr << "options:" << endl;
	cerr << "      -l" << endl;
	cerr << "          Show reconstructed lines." << endl;
	cerr << endl;
	cerr << "      -H" << endl;
	cerr << "          Show heightmap of result." << endl;
	cerr << endl;
	cerr << "      -k" << endl;
	cerr << "          Wait for key after showing a debug image." << endl;
	cerr << endl;
	cerr << "      -L <line-detector>" << endl;
	cerr << "          Set the line detector to use." << endl;
	cerr << "          Possible values: default, diff, free" << endl;
	cerr << endl;
	cerr << "      -R <reconstructor>" << endl;
	cerr << "          Set the reconstructor to use." << endl;
	cerr << "          Possible values: default" << endl;
	cerr << endl;
	cerr << "      -v" << endl;
	cerr << "          Print some more messages about current state." << endl;
	cerr << endl;
	cerr << "      -c <device>" << endl;
	cerr << "          Capture images using <device>." << endl;
	cerr << endl;
	cerr << "      -C" << endl;
	cerr << "          Show captured images." << endl;
	cerr << endl;
	cerr << "      -s <prefix>" << endl;
	cerr << "          Save captured images." << endl;
	cerr << endl;
	cerr << "      -d <fov> <skew> <offset>" << endl;
	cerr << "          Configure device." << endl;
	cerr << endl;
	cerr << "      --create-commands" << endl;
	cerr << "          Create commands for the future and disable reconstruction." << endl;
	cerr << "          This option implies '-s ""' if -s is not given as option." << endl;
	cerr << endl;
	cerr << "      -h, --help" << endl;
	cerr << "          Show this information." << endl;
	cerr << endl;
	std::exit(exitCode);
}
