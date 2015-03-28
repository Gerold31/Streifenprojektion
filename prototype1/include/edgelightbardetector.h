#ifndef EDGELIGHTBARDETECTOR_H
#define EDGELIGHTBARDETECTOR_H

#include "lightbardetector.h"
#include "line.h"


class EdgeLightBarDetector : public LightBarDetector
{
public:
	EdgeLightBarDetector() {}
	virtual ~EdgeLightBarDetector() {}

	virtual void processImage(const cv::Mat &img, Line &line);
};

#endif //EDGELIGHTBARDETECTOR_H
