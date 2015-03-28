#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <opencv2/core/core.hpp>

#include "deviceconfiguration.h"
#include "reconstruction.h"


class Controller
{
public:
	Controller();
	virtual ~Controller();

	virtual void main() = 0;

protected:
	int imageNumber = 0;

	void processImage(const cv::Mat &img);
	void updateDevice(const DeviceConfiguration &dconf);
	void updateReference(const cv::Mat &ref);

	void updateFov(double fov);
	void updateProjectorOrientation(double skew, double pitch);
	void updateProjectorPos(const cv::Vec3d &pos);
	void updateTransformation(const cv::Matx44d &t);

private:
	DeviceConfiguration deviceConfiguration;
	Reconstruction reconstruction;

	bool applyFov = true;
	bool applyProjectorOrientation = true;
	bool applyProjectorPosition = true;
	bool applyTransformation = true;

	bool windowLightbar = false;
	bool windowHeightmap = false;
};

#endif // CONTROLLER_H
