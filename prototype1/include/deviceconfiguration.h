#ifndef DEVICECONFIGURATION_H
#define DEVICECONFIGURATION_H

#include <opencv2/core/core.hpp>


class DeviceConfiguration
{
public:
	double fov;
	cv::Vec3d projectorPos;
	double projectorPitch;
	double projectorSkew;
};

#endif // DEVICECONFIGURATION_H
