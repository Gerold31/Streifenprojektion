#ifndef LINEDETECTOR_H
#define LINEDETECTOR_H

#include <vector>

#include <opencv2/core/core.hpp>

#include <line.h>


class LineDetector
{
public:
	LineDetector() {};
	virtual ~LineDetector() {};

	virtual void setBackground(const cv::Mat& background) = 0;
	virtual void processImage(const cv::Mat& img, Line& line) = 0;
};

#endif // LINEDETECTOR_H
