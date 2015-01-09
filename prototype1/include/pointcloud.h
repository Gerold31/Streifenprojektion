#ifndef POINTCLOUD_H
#define POINTCLOUD_H

#include <vector>

#include <opencv2/core/core.hpp>


class PointCloud
{
public:
	struct Point {
		cv::Vec3d pos;
		cv::Vec2i imgPos;
	};

	PointCloud();
	~PointCloud();

	void addPoint(double x, double y, double z, cv::Vec2i imgPos);
	void addPoint(cv::Vec3d pos, cv::Vec2i imgPos);
private:
	std::vector<Point> points;
};

#endif // POINTCLOUD_H
