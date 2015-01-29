#include "camera.h"

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
	while(id != mImgID)
	{
		printf(" \b");
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
