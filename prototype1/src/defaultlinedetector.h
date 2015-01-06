#ifndef DEFAULTLINEDETECTOR_H
#define DEFAULTLINEDETECTOR_H

#include <opencv2/core/core.hpp>

#include <linedetector.h>


class DefaultLineDetector : public LineDetector
{
public:
	DefaultLineDetector();
	virtual ~DefaultLineDetector();

	virtual void setBackground(const cv::Mat& background);
	virtual void processImage(const cv::Mat& img, Line& line);
private:
	cv::Mat diff(const cv::Mat& back, const cv::Mat& img) const;

	cv::Mat background;
};

#endif // DEFAULTLINEDETECTOR_H
