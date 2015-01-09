#include <pointcloud.h>

#include <iostream>

#include <opencv2/core/core.hpp>

#include "configuration.h"

PointCloud::PointCloud()
{

}

PointCloud::~PointCloud()
{

}

void PointCloud::addPoint(double x, double y, double z, cv::Vec2i imgPos)
{
	addPoint(cv::Vec3d{x, y, z}, imgPos);
}

void PointCloud::addPoint(cv::Vec3d pos, cv::Vec2i imgPos)
{
	Point p;
	p.pos = pos;
	p.imgPos = imgPos;
	points.push_back(p);

	cv::Mat &img = Configuration::lineDetection->getBackground();

	float r = img.data[img.step[0]*imgPos[1] + img.step[1]*imgPos[0] + 2]/255.;
	float g = img.data[img.step[0]*imgPos[1] + img.step[1]*imgPos[0] + 1]/255.;
	float b = img.data[img.step[0]*imgPos[1] + img.step[1]*imgPos[0] + 0]/255.;

	std::cout << pos[0] << " " << pos[1] << " " << pos[2] << " " << r << " " << g << " " << b << "\n";
}
