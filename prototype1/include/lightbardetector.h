#ifndef LIGHTBARDETECTOR_H
#define LIGHTBARDETECTOR_H

#include <opencv2/core/core.hpp>

#include <line.h>


class LightBarDetector
{
public:
	LightBarDetector() {}
	virtual ~LightBarDetector() {}

	virtual void setBackground(const cv::Mat&);
	virtual void processImage(const cv::Mat&, Line&) = 0;
};

#endif // LIGHTBARDETECTOR_H
