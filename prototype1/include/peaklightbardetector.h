#ifndef PEAKLIGHTBARDETECTOR_H
#define PEAKLIGHTBARDETECTOR_H

#include <opencv2/core/core.hpp>

#include "lightbardetector.h"


struct WorkData;

class PeakLightBarDetector : public LightBarDetector
{
public:
	PeakLightBarDetector() {}
	virtual ~PeakLightBarDetector() {}

	virtual void processImage(const cv::Mat& img, Line& line) override;

private:
	void computeAreaAvg(WorkData& data) const;
	void computeEval(WorkData& data) const;
	void computeLine(WorkData& data) const;
	double evaluate(const cv::Vec3d color) const;

	cv::Vec2i areaSize = {11, 11};
	double threshold = 0.6;
	int median = 1;
};

#endif // PEAKLIGHTBARDETECTOR_H
