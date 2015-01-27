#include "reconstruction.h"

#include <iostream>

#include <opencv2/core/core.hpp>

using cv::Mat;
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
	if (img.empty()) {
		r = 1.0;
		g = 1.0;
		b = 1.0;
	} else {
		r = img.data[img.step[0]*imgPos[1] + img.step[1]*imgPos[0] + 2]/255.;
		g = img.data[img.step[0]*imgPos[1] + img.step[1]*imgPos[0] + 1]/255.;
		b = img.data[img.step[0]*imgPos[1] + img.step[1]*imgPos[0] + 0]/255.;
	}

	cout << pos[0] << " " << pos[1] << " " << pos[2] << " "
		 << r << " " << g << " " << b << endl;
}

void Reconstruction::setSource(const cv::Mat &sourceImg)
{
	img = sourceImg;
}

const std::vector<Reconstruction::Point> &Reconstruction::getPoints() const
{
	return points;
}
