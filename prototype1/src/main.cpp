#include <cstdlib>
#include <iostream>
#include <istream>
#include <regex>
#include <string>

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>

#include <deviceconfiguration.h>
#include <configuration.h>
#include <line.h>
#include <pointcloud.h>

using cv::imread;
using cv::imshow;
using cv::Mat;
using cv::namedWindow;
using cv::Scalar;
using cv::Size;
using cv::waitKey;
using std::atof;
using std::cerr;
using std::endl;
using std::getline;
using std::istream;
using std::regex;
using std::regex_match;
using std::smatch;
using std::string;


const regex COMMAND_COMMENT{"\\s*|#.*|//.*"};
const regex COMMAND_BACKGROUND{"^b\\s+(.*)$"};
const regex COMMAND_DEVICECONF{"c\\s+(-?[\\d\\.]+)\\s+(-?[\\d\\.]+)\\s+(-?[\\d\\.]+)(\\s.*)?"};
const regex COMMAND_IMAGE{"^(.*)$"};

const char WINDOW_DEBUG_LINE[] = "Debug: Line";
const char WINDOW_DEBUG_CLOUD[] = "Debug: Point Cloud";

int main(int argc, char *argv[])
{
	Configuration::init(argc, argv);

	if (Configuration::debugLine) {
		namedWindow(WINDOW_DEBUG_LINE);
	}
	if (Configuration::debugCloud) {
		namedWindow(WINDOW_DEBUG_CLOUD);
	}

	std::istream& in = *Configuration::inputStream;
	PointCloud pointCloud;
	while (in.good()) {
		string str;
		getline(in, str);

		smatch match;
		if (regex_match(str, COMMAND_COMMENT)) {
			continue;
		}
		if (regex_match(str, match, COMMAND_BACKGROUND)) {
			Mat background = imread(match[1], CV_LOAD_IMAGE_COLOR);
			if (!background.data) {
				cerr << "Could not open image: " << match[1] << endl;
				return EXIT_FAILURE;
			}
			Configuration::lineDetection->setBackground(background);
		} else if (regex_match(str, match, COMMAND_DEVICECONF)) {
			DeviceConfiguration dconf;
			dconf.projectorOffset = atof(match[1].str().c_str());
			dconf.fov = atof(match[2].str().c_str());
			dconf.projectorAngle = atof(match[3].str().c_str());
			Configuration::reconstructor->setDeviceConfiguration(dconf);
		} else if (regex_match(str, match, COMMAND_IMAGE)) {
			Mat img = imread(match[1], CV_LOAD_IMAGE_COLOR);
			Line line{img.cols, img.rows};
			if (!img.data) {
				cerr << "Could not open image: " << match[1] << endl;
				return EXIT_FAILURE;
			}
			Configuration::lineDetection->processImage(img, line);
			if (Configuration::debugLine) {
				Mat demo{Size{img.cols, img.rows}, CV_8UC3, Scalar::all(0)};
				for (const Line::Sample& sample : line.getSamples()) {
					demo.data[demo.step[0]*sample.pos[1] + demo.step[1]*sample.pos[0] + 2] = 255;
					demo.data[demo.step[0]*sample.pos[1] + demo.step[1]*sample.pos[0] + 1] = 255;
					demo.data[demo.step[0]*sample.pos[1] + demo.step[1]*sample.pos[0] + 0] = 255;
				}
				imshow(WINDOW_DEBUG_LINE, demo);
				waitKey();
			}
			Configuration::reconstructor->processLine(line, pointCloud);
		}
	}

	if (!in.eof()) {
		cerr << "Error occurred by reading file." << endl;
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}
