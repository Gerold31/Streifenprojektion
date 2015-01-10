#ifndef LINE_H
#define LINE_H

#include <vector>

#include <opencv2/core/core.hpp>


class Line
{
public:
	struct Sample
	{
		cv::Vec2i pos;
		int thickness;
	};

	Line(int resolutionX, int resolutionY);

	void addSample(const cv::Vec2i& pos, int thickness = 0);
	void addSample(int posX, int posY, int thickness = 0);

	std::vector<Sample>& getSamples();
	const std::vector<Sample>& getSamples() const;
	const cv::Vec2i getResolution() const;

private:
	std::vector<Sample> samples;
	cv::Vec2i resolution;
};


inline Line::Line(int resolutionX, int resolutionY) :
	resolution{resolutionX, resolutionY}
{
}

inline void Line::addSample(const cv::Vec2i &pos, int thickness)
{
	Line::Sample sample;
	sample.pos = pos;
	sample.thickness = thickness;
	samples.push_back(sample);
}

inline void Line::addSample(int posX, int posY, int thickness)
{
	addSample(cv::Vec2i{posX, posY}, thickness);
}

inline std::vector<Line::Sample>& Line::getSamples()
{
	return samples;
}

inline const std::vector<Line::Sample> &Line::getSamples() const
{
	return samples;
}

inline const cv::Vec2i Line::getResolution() const
{
	return resolution;
}

#endif // LINE_H
