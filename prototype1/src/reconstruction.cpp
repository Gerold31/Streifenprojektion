#include "reconstruction.h"

#include <iostream>

#include <opencv2/core/core.hpp>

using cv::Mat;
using std::cout;


Reconstruction::Reconstruction()
{
}

Reconstruction::~Reconstruction()
{
}

void Reconstruction::addPoint(cv::Vec3d pos, cv::Vec2i imgPos)
{
	points.push_back(Point{pos, imgPos});

	float r = img.data[img.step[0]*imgPos[1] + img.step[1]*imgPos[0] + 2]/255.;
	float g = img.data[img.step[0]*imgPos[1] + img.step[1]*imgPos[0] + 1]/255.;
	float b = img.data[img.step[0]*imgPos[1] + img.step[1]*imgPos[0] + 0]/255.;

	cout << pos[0] << " " << pos[1] << " " << pos[2] << " "
		 << r << " " << g << " " << b << "\n";
}

void Reconstruction::setSource(cv::Mat sourceImg)
{
	img = sourceImg;
}

const std::vector<Reconstruction::Point> &Reconstruction::getPoints() const
{
	return points;
}
