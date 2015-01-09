#ifndef CAMERA_H
#define CAMERA_H

#include "singleton.h"

#include "opencv2/opencv.hpp"

class Camera : public Singleton<Camera>
{
public:
	cv::Mat *capture(int device);

private:
	~Camera();

	std::map<int, cv::VideoCapture *> mDevices;

	friend class Singleton;
};

#endif // CAMERA_H
