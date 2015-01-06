#include <pointcloud.h>

#include <iostream>

#include <opencv2/core/core.hpp>


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

	std::cout << pos[0] << " " << pos[1] << " " << pos[2] << " 1 0 0\n";
}
