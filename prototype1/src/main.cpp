#include <cstdlib>
#include <iostream>
#include <istream>
#include <regex>
#include <string>

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>

#include "camera.h"
#include "configuration.h"
#include "deviceconfiguration.h"
#include "laser.h"
#include "line.h"
#include "reconstruction.h"
#include "servo.h"

#define MIN_ANGLE (-50)
#define MAX_ANGLE (20)

using cv::imread;
using cv::imshow;
using cv::Mat;
using cv::namedWindow;
using cv::Scalar;
using cv::Size;
using cv::Vec2i;
using cv::waitKey;
using std::atof;
using std::cerr;
using std::endl;
using std::getline;
using std::istream;
using std::regex;
using std::regex_match;
using std::shared_ptr;
using std::smatch;
using std::string;

static void updateHeightmap(const Reconstruction& reconstruction, int sizeX, int sizeY);


const regex COMMAND_COMMENT{"\\s*|#.*|//.*"};
const regex COMMAND_BACKGROUND{"^b\\s+(.*)$"};
const regex COMMAND_DEVICECONF{"c\\s+(-?[\\d\\.]+)\\s+(-?[\\d\\.]+)\\s+(-?[\\d\\.]+)(\\s.*)?"};
const regex COMMAND_IMAGE{"^(.*)$"};

const char WINDOW_DEBUG_LINE[] = "Debug: Line";
const char WINDOW_DEBUG_CLOUD[] = "Debug: Point Cloud";
const char WINDOW_DEBUG_CAMERA[] = "Debug: Camera";
const char WINDOW_DEBUG_HEIGHTMAP[] = "Debug: Heightmap";


int main_app(int argc, char *argv[])
{
	Configuration::init(argc, argv);

	if (Configuration::debugLightbar) {
		namedWindow(WINDOW_DEBUG_LINE);
	}
	if (Configuration::debugCloud) {
		namedWindow(WINDOW_DEBUG_CLOUD);
	}
	if (Configuration::debugCamera) {
		namedWindow(WINDOW_DEBUG_CAMERA);
	}
	if (Configuration::debugHeightmap) {
		namedWindow(WINDOW_DEBUG_HEIGHTMAP);
	}

	std::cout << "Cam: " << 1/std::tan(Configuration::deviceConfiguration.fov/2.) << " " << 4. /3 << std::endl;

	Reconstruction reconstruction;

	// Live reconstruction
	if(Configuration::captureDevice != -1)
	{
		Servo::getSingleton()->setAngle(MIN_ANGLE-5);
		Laser::getSingleton()->toggle(false);

		cv::Mat *img;
		cv::Mat &background = *Camera::getSingleton()->capture(Configuration::captureDevice);

		Configuration::lineDetection->setBackground(background);
		reconstruction.setSource(background);

		if(Configuration::savePrefix != nullptr)
		{
			char filename[64];
			sprintf(filename, "%sref.png", Configuration::savePrefix);
			cv::imwrite(filename, background);
		}
		if(Configuration::debugCamera)
		{
			cv::imshow(WINDOW_DEBUG_CAMERA, background);
		}

		//std::this_thread::sleep_for(std::chrono::milliseconds(1000));
		cv::waitKey(1000);

		Laser::getSingleton()->toggle(true);
		for(int a=MIN_ANGLE; a<=MAX_ANGLE; a++)
		{
			Configuration::deviceConfiguration.projectorPitch = -a * M_PI/180;
			//std::cout << "Laser: " << Configuration::deviceConfiguration.projectorPos << " " << a * M_PI/180 << std::endl;
			Configuration::reconstructor->setDeviceConfiguration(Configuration::deviceConfiguration);
			Servo::getSingleton()->setAngle(a);
			//std::this_thread::sleep_for(std::chrono::milliseconds(100));
			cv::waitKey(100);

			img = Camera::getSingleton()->capture(Configuration::captureDevice);

			if(Configuration::savePrefix != nullptr)
			{
				char filename[64];
				sprintf(filename, "%s%d.png", Configuration::savePrefix, a);
				cv::imwrite(filename, *img);
			}

			if(Configuration::debugCamera)
			{
				cv::imshow(WINDOW_DEBUG_CAMERA, *img);
			}

			Line line{img->cols, img->rows};
			Configuration::lineDetection->processImage(*img, line);
			if (Configuration::debugLightbar) {
				Mat demo{Size{img->cols, img->rows}, CV_8UC3, Scalar::all(0)};
				for (const Line::Sample& sample : line.getSamples()) {
					demo.data[demo.step[0]*sample.pos[1] + demo.step[1]*sample.pos[0] + 2] = 255;
					demo.data[demo.step[0]*sample.pos[1] + demo.step[1]*sample.pos[0] + 1] = 255;
					demo.data[demo.step[0]*sample.pos[1] + demo.step[1]*sample.pos[0] + 0] = 255;
				}
				imshow(WINDOW_DEBUG_LINE, demo);
			}
			Configuration::reconstructor->processLine(line, reconstruction);

			if (Configuration::debugHeightmap) {
				updateHeightmap(reconstruction, img->cols, img->rows);
			}

			//std::this_thread::sleep_for(std::chrono::milliseconds(400));
			cv::waitKey(400);

			delete img;
		}
		Laser::getSingleton()->toggle(false);
	}

	// Read commands from stdin
	shared_ptr<istream> inp = Configuration::inputStream;
	if (inp != nullptr) {
		istream& in = *inp;
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
				reconstruction.setSource(background);
			} else if (regex_match(str, match, COMMAND_DEVICECONF)) {
				DeviceConfiguration dconf;
				dconf.projectorPos = {atof(match[1].str().c_str()), 0., 0.};
				dconf.fov = atof(match[2].str().c_str());
				dconf.projectorPitch = atof(match[3].str().c_str());
				Configuration::reconstructor->setDeviceConfiguration(dconf);
			} else if (regex_match(str, match, COMMAND_IMAGE)) {
				Mat img = imread(match[1], CV_LOAD_IMAGE_COLOR);
				Line line{img.cols, img.rows};
				if (!img.data) {
					cerr << "Could not open image: " << match[1] << endl;
					return EXIT_FAILURE;
				}
				Configuration::lineDetection->processImage(img, line);
				Configuration::reconstructor->processLine(line, reconstruction);
				if (Configuration::debugHeightmap) {
					updateHeightmap(reconstruction, img.cols, img.rows);
				}
				if (Configuration::debugLightbar) {
					Mat demo{Size{img.cols, img.rows}, CV_8UC3, Scalar::all(0)};
					for (const Line::Sample& sample : line.getSamples()) {
						demo.data[demo.step[0]*sample.pos[1] + demo.step[1]*sample.pos[0] + 2] = 255;
						demo.data[demo.step[0]*sample.pos[1] + demo.step[1]*sample.pos[0] + 1] = 255;
						demo.data[demo.step[0]*sample.pos[1] + demo.step[1]*sample.pos[0] + 0] = 255;
					}
					imshow(WINDOW_DEBUG_LINE, demo);
					waitKey();
				}
			}
		}

		if (!in.eof()) {
			cerr << "Error occurred by reading file." << endl;
			return EXIT_FAILURE;
		}
	}

	return EXIT_SUCCESS;
}

void updateHeightmap(const Reconstruction& reconstruction, int sizeX, int sizeY)
{
	if (!Configuration::debugHeightmap || reconstruction.getPoints().empty())
		return;

	double first = reconstruction.getPoints()[0].pos[2];
	double min = first, max = first;
	for (const Reconstruction::Point p : reconstruction.getPoints()) {
		if (p.pos[2] < min) {
			min = p.pos[2];
		}
		if (p.pos[2] > max) {
			max = p.pos[2];
		}
	}

	Mat demo{Size{sizeX, sizeY}, CV_8UC3, Scalar{255, 127, 127}};
	for (const Reconstruction::Point p : reconstruction.getPoints()) {
		Vec2i pos = p.imgPos;
		uchar value = (uchar) (255 * (max - p.pos[2]) / (max - min));
		demo.data[demo.step[0]*pos[1] + demo.step[1]*pos[0] + 2] = value;
		demo.data[demo.step[0]*pos[1] + demo.step[1]*pos[0] + 1] = value;
		demo.data[demo.step[0]*pos[1] + demo.step[1]*pos[0] + 0] = value;
	}

	imshow(WINDOW_DEBUG_HEIGHTMAP, demo);
}

#ifndef MAINLESS
int main(int argc, char *argv[])
{
	main_app(argc, argv);
}
#endif // MAINLESS
