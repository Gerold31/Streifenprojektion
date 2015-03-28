#ifndef COMMON_H
#define COMMON_H

#include <opencv2/core/core.hpp>


#define IDENTITY44d cv::Matx44d{\
	1., 0., 0., 0.,\
	0., 1., 0., 0.,\
	0., 0., 1., 0.,\
	0., 0., 0., 1.}

inline cv::Vec3d applyTransformation(const cv::Matx44d &t, const cv::Vec3d &v)
{
	cv::Vec4d hpos = cv::Vec4d{v[0], v[1], v[2], 1.0};
	hpos = t * hpos;
	return cv::Vec3d{hpos[0] / hpos[3], hpos[1] / hpos[3], hpos[2] / hpos[3]};
}

#endif // COMMON_H
