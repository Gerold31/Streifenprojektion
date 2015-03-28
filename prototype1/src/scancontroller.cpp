#include "scancontroller.h"

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


ScanController::ScanController()
{
}

ScanController::~ScanController()
{
}

void ScanController::main()
{
	// get device configuration from arguments
	updateDevice(Configuration::deviceConfiguration);

	// setup hardware
	Camera c(Configuration::captureDevice);
	Laser::getSingleton()->toggle(false);
	Servo::getSingleton()->setAngle(MIN_ANGLE-5);

	// create debug window for camera if required
	if (Configuration::debugCamera) {
		namedWindow(WINDOW_DEBUG_CAMERA);
	}

	// create reference image
	cv::Mat reference;

	//std::this_thread::sleep_for(std::chrono::milliseconds(1000));
	cv::waitKey(500);

	for(int a=MIN_ANGLE; a<=MAX_ANGLE; a++)
	{
		imageNumber = a;
		Configuration::deviceConfiguration.projectorPitch = -a * M_PI/180;
		updateDevice(Configuration::deviceConfiguration);

		Laser::getSingleton()->toggle(false);
		Servo::getSingleton()->setAngle(a);

		// wait for servo to be in position
		//std::this_thread::sleep_for(std::chrono::milliseconds(100));
		cv::waitKey(300);

		c.capture().copyTo(reference);
		updateReference(reference);

		Laser::getSingleton()->toggle(true);

		c.capture();

		cv::Mat img = c.capture();

		if(Configuration::debugCamera)
		{
			cv::imshow(WINDOW_DEBUG_CAMERA, img);
		}

		processImage(img);

		//std::this_thread::sleep_for(std::chrono::milliseconds(400));
		cv::waitKey(1);
	}
	Laser::getSingleton()->toggle(false);
}
