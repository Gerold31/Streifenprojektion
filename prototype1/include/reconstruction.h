#ifndef RECONSTRUCTION_H
#define RECONSTRUCTION_H

#include <vector>

#include <opencv2/core/core.hpp>


class Reconstruction
{
public:
	Reconstruction();
	~Reconstruction();

	void addPoint(cv::Vec3d pos, cv::Vec2i imgPos);
	void setSource(cv::Mat sourceImg);
private:
	cv::Mat img;
};

#endif // RECONSTRUCTION_H
