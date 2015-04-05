#include "camera.h"

#include <thread>

Camera::Camera(int device)
{
	mDevice = new cv::VideoCapture(device);
	mThread = std::thread(&Camera::readImg, this);
	mImgID = 0;
}

Camera::~Camera()
{
	delete mDevice;
}

cv::Mat Camera::capture()
{
	int id = mImgID;
	cv::Mat r;
	while(id == mImgID)
	{
		std::this_thread::yield();
	}

	mNextAccess.lock();
	mMutex.lock();
	mNextAccess.unlock();
	r = mImg;
	mMutex.unlock();
	return r;
}

void Camera::readImg()
{
	while(1)
	{
		mNextAccess.lock();
		mMutex.lock();
		mNextAccess.unlock();
		mDevice->grab();
		mDevice->retrieve(mImg);
		mImgID++;
		mMutex.unlock();
	}
}
