#include "controller.h"

#include <cmath>
#include <iostream>

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>

#include "configuration.h"

using cv::imshow;
using cv::imwrite;
using cv::Mat;
using cv::namedWindow;
using cv::Scalar;
using cv::Size;
using cv::Vec2i;
using cv::Vec3b;
using cv::waitKey;
using std::cout;
using std::endl;
using std::tan;

const char WINDOW_DEBUG_LIGHTBAR[] = "Debug: Line";
const char WINDOW_DEBUG_CLOUD[] = "Debug: Point Cloud";
const char WINDOW_DEBUG_HEIGHTMAP[] = "Debug: Heightmap";


Controller::Controller()
{
}

Controller::~Controller()
{
}

void Controller::processImage(const cv::Mat &img)
{
	// save image if required
	char filename[64];
	if(Configuration::savePrefix != nullptr) {
		sprintf(filename, "%s_%d.png", Configuration::savePrefix, imageNumber);
		imwrite(filename, img);
	}

	// special handling for command creation
	if (Configuration::createCommands) {
		if (applyFov)
			cout << "fov " << deviceConfiguration.fov << endl;
		if (applyProjectorOrientation)
			cout << "po " << deviceConfiguration.projectorSkew << " "
				 << deviceConfiguration.projectorPitch << endl;
		if (applyProjectorPosition)
			cout << "pp " << deviceConfiguration.projectorPos[0]
				 << " " << deviceConfiguration.projectorPos[1]
				 << " " << deviceConfiguration.projectorPos[2] << endl;
		if (applyTransformation) {
			cout << "t";
			for (int row = 0; row < 4; ++row) {
				for (int col = 0; col < 4; ++col) {
					cout << " " << deviceConfiguration.transformation(row, col);
				}
			}
			cout << endl;
		}

		applyFov = false;
		applyProjectorOrientation = false;
		applyProjectorPosition = false;
		applyTransformation = false;

		cout << filename << endl;
		return;
	}

	// print additional informations
	if (applyFov) {
		// format:  cam: <focal length> <aspect ratio>
		cout << "cam: " << 1 / tan(deviceConfiguration.fov / 2.0)
			 << " " << 4. /3 << endl;
	}
	if (applyProjectorPosition || applyProjectorOrientation) {
		// format:  laser: [<posX>, <posY>, <posZ>] <skew> <pitch>
		cout << "laser: " << deviceConfiguration.projectorPos
			 << " " << deviceConfiguration.projectorSkew
			 << " " << deviceConfiguration.projectorPitch << std::endl;
	}

	// appliy device configuration if something has changed
	if (applyFov || applyProjectorOrientation || applyProjectorPosition || applyTransformation)
		Configuration::reconstructor->setDeviceConfiguration(deviceConfiguration);

	applyFov = false;
	applyProjectorOrientation = false;
	applyProjectorPosition = false;
	applyTransformation = false;

	// run light bar detection and reconstruction
	Line line{img.cols, img.rows};
	Configuration::lightBarDetector->processImage(img, line);
	Configuration::reconstructor->processLine(line, reconstruction);

	// show debug windows
	if (Configuration::debugLightbar) {
		// Lightbar Window
		// create window if it does not exits yet
		if (!windowLightbar) {
			namedWindow(WINDOW_DEBUG_LIGHTBAR);
			windowLightbar = true;
		}
		// show lightbar
		Mat demo{Size{img.cols, img.rows}, CV_8UC3, Scalar::all(0)};
		for (const Line::Sample& sample : line.getSamples()) {
			demo.at<Vec3b>(sample.pos[1], sample.pos[0]) = Vec3b::all(255);
		}
		imshow(WINDOW_DEBUG_LIGHTBAR, demo);
	}
	if (Configuration::debugHeightmap) {
		// Heightmap Window
		// create window if it does not exist yet
		if (!windowHeightmap) {
			namedWindow(WINDOW_DEBUG_HEIGHTMAP);
			windowHeightmap = true;
		}
		// show heightmap
		Mat hmap{Size{img.cols, img.rows}, CV_8UC3, Scalar{255, 127, 127}};

		if (!reconstruction.getPoints().empty()) {
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

			for (const Reconstruction::Point p : reconstruction.getPoints()) {
				Vec2i pos = p.imgPos;
				uchar value = (uchar) (255 * (max - p.pos[2]) / (max - min));
				hmap.at<Vec3b>(pos[1], pos[0]) = Vec3b::all(value);
			}
		}

		imshow(WINDOW_DEBUG_HEIGHTMAP, hmap);
	}

	// wait if a debug window is used
	if (Configuration::debugLightbar || Configuration::debugHeightmap) {
		//waitKey();
	}
}

void Controller::updateDevice(const DeviceConfiguration &dconf)
{
	if (dconf.fov != deviceConfiguration.fov)
		applyFov = true;
	if (dconf.projectorPos != deviceConfiguration.projectorPos)
		applyProjectorPosition = true;
	if (dconf.projectorPitch != deviceConfiguration.projectorPitch
			|| dconf.projectorSkew != deviceConfiguration.projectorSkew)
		applyProjectorOrientation = true;
	if (dconf.transformation != deviceConfiguration.transformation)
		applyTransformation = true;

	deviceConfiguration = dconf;
}

void Controller::updateReference(const cv::Mat &ref)
{
	// save image if required
	char filename[64];
	if(Configuration::savePrefix != nullptr) {
		sprintf(filename, "%s_%d_ref.png", Configuration::savePrefix, imageNumber);
		imwrite(filename, ref);
	}

	// special handling for command creation
	if (Configuration::createCommands) {
		cout << "b " << filename << endl;
		return;
	}

	// apply new reference image
	Configuration::lightBarDetector->setBackground(ref);
	reconstruction.setReference(ref);
}

void Controller::updateFov(double fov)
{
	deviceConfiguration.fov = fov;
	applyFov = true;
}

void Controller::updateProjectorOrientation(double skew, double pitch)
{
	deviceConfiguration.projectorSkew = skew;
	deviceConfiguration.projectorPitch = pitch;
	applyProjectorOrientation = true;
}

void Controller::updateProjectorPos(const cv::Vec3d &pos)
{
	deviceConfiguration.projectorPos = pos;
	applyProjectorPosition = true;
}

void Controller::updateTransformation(const cv::Matx44d &t)
{
	deviceConfiguration.transformation = t;
	applyTransformation = true;
}
