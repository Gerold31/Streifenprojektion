#ifndef PLOT3D_H
#define PLOT3D_H

#include <mutex>
#include <vector>

#include <glm/glm.hpp>

#include <point.h>
#include <pointcloud.h>


class Plot3D {
public:
	void addPoint(float x, float y, float z, float r, float g, float b);
	void show();
private:
	PointCloud *pointCloud = nullptr;
	std::vector<Point> points;
	mutable std::mutex m;
};

#endif // PLOT3D_H