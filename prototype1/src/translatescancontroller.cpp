#include "translatescancontroller.h"

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>

#include "camera.h"
#include "configuration.h"
#include "laser.h"
#include "servo.h"

using cv::namedWindow;

#define MIN_ANGLE (-40)
#define MAX_ANGLE (30)

const char WINDOW_DEBUG_CAMERA[] = "Debug: Camera";


TranslateScanController::TranslateScanController(double dx, double dy, double dz, int n)
{
	this->dx = dx;
	this->dy = dy;
	this->dz = dz;
	this->n = n;
}

TranslateScanController::~TranslateScanController()
{
}

void TranslateScanController::main()
{
	// get device configuration from arguments
	updateDevice(Configuration::deviceConfiguration);

	// setup hardware
	Camera c(Configuration::captureDevice);
	Laser::getSingleton()->toggle(false);

	//make sure the servo rotates in positive direction to reduce inaccuracies caused by backlash
	Servo::getSingleton()->setAngle(-5);
	Servo::getSingleton()->setAngle(0);

	// create debug window for camera if required
	if (Configuration::debugCamera) {
		namedWindow(WINDOW_DEBUG_CAMERA);
	}

	// create reference image
	cv::Mat reference;

	//std::this_thread::sleep_for(std::chrono::milliseconds(1000));
	cv::waitKey(500);

	for(int i=0; i<n; i++)
	{
		imageNumber = i;

		Laser::getSingleton()->toggle(false);

		// wait 1 frame in case the laser was turned off while this frame was captured
		c.capture();

		c.capture().copyTo(reference);
		updateReference(reference);

		Laser::getSingleton()->toggle(true);

		// wait 1 frame in case the laser was turned on while this frame was captured
		c.capture();

		cv::Mat img = c.capture();

		if(Configuration::debugCamera)
		{
			cv::imshow(WINDOW_DEBUG_CAMERA, img);
		}

		processImage(img);

		std::cerr << "move the object" << std::endl;
		std::string s; std::getline(std::cin, s);

		Configuration::deviceConfiguration.transformation = Configuration::deviceConfiguration.transformation * cv::Matx44d(1, 0, 0, dx, 0, 1, 0, dy, 0, 0, 1, dz, 0, 0, 0, 1);
		updateDevice(Configuration::deviceConfiguration);
	}
	Laser::getSingleton()->toggle(false);
}
