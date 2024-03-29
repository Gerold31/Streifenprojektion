#include <configuration.h>

#include <cstdlib>
#include <cstring>
#include <fstream>
#include <iostream>
#include <istream>
#include <memory>

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

bool Configuration::verbose = false;
bool Configuration::drawBox = false;
shared_ptr<istream> Configuration::inputStream{&cin, [](istream*){}};

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
				if (equal(arg, "-")) {
					inputStream = shared_ptr<istream>{&cin, [](istream* p){}};
				} else {
					shared_ptr<ifstream> in = make_shared<ifstream>(arg);
					if (!in->is_open()) {
						cerr << "Unable to open file: " << arg << endl;
						exit(EXIT_FAILURE);
					}
					inputStream = in;
				}
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
				} else if (equal(arg, "--box")) {
					drawBox = true;
				} else if (equal(arg, "--readin")) {
					cerr << "Using --readin is deprecated." << endl;
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
		return nullptr;
	}
}

void Configuration::handleOption(const char op)
{
	switch (op) {
	case 'h':
	{
		helpAndExit(EXIT_SUCCESS);
	}
	case 'v':
	{
		verbose = true;
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
	cerr << "file: File to plot in 3-dimensional space." << endl;
	cerr << "      Program will use stdin by default." << endl;
	cerr << endl;
	cerr << "options:" << endl;
	cerr << "      -v" << endl;
	cerr << "          Print some more messages about current state." << endl;
	cerr << endl;
	cerr << "      --box" << endl;
	cerr << "          Add many points  which are created randomly" << endl;
	cerr << "          between (0,0,0) and (1,1,1)." << endl;
	cerr << endl;
	cerr << "      -h, --help" << endl;
	cerr << "          Show this information." << endl;
	cerr << endl;
	std::exit(exitCode);
}
