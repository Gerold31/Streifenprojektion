#ifndef RECONSTRUCTION_H
#define RECONSTRUCTION_H

#include <vector>

#include <opencv2/core/core.hpp>


class Reconstruction
{
public:
	struct Point
	{
		cv::Vec3d pos;
		cv::Vec2i imgPos;
	};

	Reconstruction();
	~Reconstruction();

	void addPoint(cv::Vec3d pos, cv::Vec2i imgPos);
	void setReference(const cv::Mat &ref);
	const std::vector<Point>& getPoints() const;
private:
	std::vector<Point> points;
	cv::Mat reference;
};

#endif // RECONSTRUCTION_H
