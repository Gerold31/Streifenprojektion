#include "camera.h"

Camera::~Camera()
{
	for(auto i=mDevices.begin(); i!=mDevices.end(); ++i)
	{
		delete (*i).second;
	}
}

cv::Mat *Camera::capture(int device)
{
	if(!mDevices.count(device))
	{
		mDevices[device] = new cv::VideoCapture(device);
	}
	if(mDevices[device]->isOpened())
	{
		cv::Mat *m = new cv::Mat;
		//*mDevices[device] >> *m;
		mDevices[device]->grab();
		mDevices[device]->retrieve(*m);
		return m;
	}
	return nullptr;
}
