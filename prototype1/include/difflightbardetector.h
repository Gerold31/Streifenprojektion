#ifndef DIFFLIGHTBARDETECTOR_H
#define DIFFLIGHTBARDETECTOR_H

#include <opencv/cv.h>

#include "line.h"
#include "lightbardetector.h"


class DiffLightBarDetector : public LightBarDetector
{
private:
	cv::Mat back;
	cv::Mat difference(const cv::Mat& img) const;

public:
	DiffLightBarDetector() {}
	virtual ~DiffLightBarDetector() {}

	virtual void setBackground(const cv::Mat& background) override;
	virtual void processImage(const cv::Mat& img, Line& line) override;
};

#endif // DIFFLIGHTBARDETECTOR_H
