#ifndef CAMERA_H
#define CAMERA_H

#include "opencv2/opencv.hpp"
#include <thread>
#include <mutex>

class Camera
{
public:
	Camera(int device);
	~Camera();

	cv::Mat capture();

private:
	cv::VideoCapture *mDevice;
	std::thread mThread;
	std::mutex mMutex, mNextAccess;
	cv::Mat mImg;
	int mImgID;

	void readImg();
};

#endif // CAMERA_H
