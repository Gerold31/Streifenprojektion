#ifndef FREELIGHTBARDETECTOR_H
#define FREELIGHTBARDETECTOR_H

#include "lightbardetector.h"
#include "line.h"

class FreeLightBarDetector : public LightBarDetector
{
public:
	FreeLightBarDetector() {}
	virtual ~FreeLightBarDetector() {}

	virtual void findLightBar(const cv::Mat &img, Line &line);
private:
	cv::Mat filter(const cv::Mat &img);
};

#endif

