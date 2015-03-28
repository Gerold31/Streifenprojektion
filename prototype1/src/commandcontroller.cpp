#include "commandcontroller.h"

#include <cstdlib>
#include <iostream>
#include <istream>
#include <regex>

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>

#include "common.h"
#include "configuration.h"

using cv::imread;
using cv::Mat;
using cv::Matx44d;
using cv::Vec3d;
using std::atof;
using std::cerr;
using std::endl;
using std::exit;
using std::getline;
using std::istream;
using std::regex;
using std::regex_match;
using std::shared_ptr;
using std::smatch;
using std::string;

const regex COMMAND_COMMENT{"\\s*|#.*|//.*"};
const regex COMMAND_REFERENCE{"^b\\s+(.*)$"};
const regex COMMAND_FOV{"^fov\\s+(-?[\\d\\.]+)$"};
const regex COMMAND_PROJECTOR_POS{"^pp\\s+(-?[\\d\\.]+)\\s+(-?[\\d\\.]+)\\s+(-?[\\d\\.]+)$"};
const regex COMMAND_PROJECTOR_ORIENTATION{"^po\\s+(-?[\\d\\.]+)\\s+(-?[\\d\\.]+)$"};
const regex COMMAND_TRANSFORMATION{"^t\\s+(-?[\\d\\.]+)\\s+(-?[\\d\\.]+)\\s+(-?[\\d\\.]+)\\s+(-?[\\d\\.]+)\\s+(-?[\\d\\.]+)\\s+(-?[\\d\\.]+)\\s+(-?[\\d\\.]+)\\s+(-?[\\d\\.]+)\\s+(-?[\\d\\.]+)\\s+(-?[\\d\\.]+)\\s+(-?[\\d\\.]+)\\s+(-?[\\d\\.]+)\\s+(-?[\\d\\.]+)\\s+(-?[\\d\\.]+)\\s+(-?[\\d\\.]+)\\s+(-?[\\d\\.]+)$"};
const regex COMMAND_DEVICECONF{"c\\s+(-?[\\d\\.]+)\\s+(-?[\\d\\.]+)\\s+(-?[\\d\\.]+)(\\s.*)?"};
const regex COMMAND_IMAGE{"^(.*)$"};


CommandController::CommandController()
{
}

CommandController::~CommandController()
{
}

void CommandController::main()
{
	// get device configuration from arguments
	updateDevice(Configuration::deviceConfiguration);

	// get command stream
	shared_ptr<istream> inp = Configuration::inputStream;
	if (inp == nullptr) {
		cerr << "Invalid command stream." << endl;
		exit(EXIT_FAILURE);
	}

	// execute all commands
	istream& in = *inp;
	while (in.good()) {
		string str;
		getline(in, str);

		smatch match;
		if (regex_match(str, COMMAND_COMMENT)) {
			continue;
		}
		if (regex_match(str, match, COMMAND_REFERENCE)) {
			Mat background = imread(match[1], CV_LOAD_IMAGE_COLOR);
			if (!background.data) {
				cerr << "Could not open image: " << match[1] << endl;
				exit(EXIT_FAILURE);
				return;
			}
			updateReference(background);
		} else if (regex_match(str, match, COMMAND_FOV)) {
			double fov = atof(match[1].str().c_str());
			updateFov(fov);
		} else if (regex_match(str, match, COMMAND_PROJECTOR_POS)) {
			Vec3d pos = {
				atof(match[1].str().c_str()),
				atof(match[2].str().c_str()),
				atof(match[3].str().c_str())
			};
			updateProjectorPos(pos);
		} else if (regex_match(str, match, COMMAND_PROJECTOR_ORIENTATION)) {
			double skew = atof(match[1].str().c_str());
			double pitch = atof(match[2].str().c_str());
			updateProjectorOrientation(skew, pitch);
		} else if (regex_match(str, match, COMMAND_TRANSFORMATION)) {
			Matx44d transf;
			int i = 1;
			for (int row = 0; row < 4; ++row) {
				for (int col = 0; col < 4; ++col) {
					transf(row, col) = atof(match[i++].str().c_str());
				}
			}
			updateTransformation(transf);
		} else if (regex_match(str, match, COMMAND_DEVICECONF)) {
			DeviceConfiguration dconf;
			dconf.projectorPos = {atof(match[1].str().c_str()), 0., 0.};
			dconf.fov = atof(match[2].str().c_str());
			dconf.projectorPitch = atof(match[3].str().c_str());
			dconf.projectorSkew = 0.0;
			dconf.transformation = IDENTITY44d;
			updateDevice(dconf);
		} else if (regex_match(str, match, COMMAND_IMAGE)) {
			Mat img = imread(match[1], CV_LOAD_IMAGE_COLOR);
			if (!img.data) {
				cerr << "Could not open image: " << match[1] << endl;
				exit(EXIT_FAILURE);
				return;
			}
			processImage(img);
		}
	}

	// check for error by reading stream
	if (!in.eof()) {
		cerr << "Error occurred by reading file." << endl;
		exit(EXIT_FAILURE);
	}
}
