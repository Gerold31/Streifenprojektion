#include "reconstruction.h"

#include <iostream>

#include <opencv2/core/core.hpp>

using cv::Mat;
using cv::Vec3b;
using std::cout;
using std::endl;


Reconstruction::Reconstruction()
{
}

Reconstruction::~Reconstruction()
{
}

void Reconstruction::addPoint(cv::Vec3d pos, cv::Vec2i imgPos)
{
	points.push_back(Point{pos, imgPos});

	float r, g, b;
	if (reference.empty()) {
		r = 1.0;
		g = 1.0;
		b = 1.0;
	} else {
		Vec3b color = reference.at<Vec3b>(imgPos[1], imgPos[0]);
		r = color[2]/255.;
		g = color[1]/255.;
		b = color[0]/255.;
	}

	cout << pos[0] << " " << pos[1] << " " << pos[2] << " "
		 << r << " " << g << " " << b << endl;
}

void Reconstruction::setReference(const cv::Mat &ref)
{
	reference = ref;
}

const std::vector<Reconstruction::Point> &Reconstruction::getPoints() const
{
	return points;
}
