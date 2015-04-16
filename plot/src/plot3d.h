#ifndef PLOT3D_H
#define PLOT3D_H

#include <memory>
#include <mutex>
#include <vector>

#include <glm/glm.hpp>

#include <camera.h>
#include <drawable.h>
#include <laserfigure.h>
#include <point.h>
#include <pointcloud.h>


class Plot3D {
public:
	void addDrawable(std::shared_ptr<Drawable> drawable);
	void addDrawable(std::shared_ptr<LaserFigure> drawable);
	void addPoint(float x, float y, float z, float r, float g, float b);
	void show(int resolutionX, int resolutionY);
private:
	Camera cam;
	std::vector<std::shared_ptr<Drawable>> drawables;
	std::shared_ptr<PointCloud> pointCloud = nullptr;
	std::shared_ptr<LaserFigure> laser = nullptr;
	std::vector<std::shared_ptr<Drawable>> newDrawables;
	std::vector<Point> newPoints;
	mutable std::mutex mutexDrawables;
	mutable std::mutex mutexPoints;

	float moveSpeed = 1;
	float sensibility = 1;
};

#endif // PLOT3D_H
