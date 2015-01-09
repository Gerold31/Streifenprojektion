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
	};

	Line(int resolutionX, int resolutionY);

	void addSample(const cv::Vec2i& pos);
	void addSample(int posX, int posY);

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

inline void Line::addSample(const cv::Vec2i &pos)
{
	Line::Sample sample;
	sample.pos = pos;
	samples.push_back(sample);
}

inline void Line::addSample(int posX, int posY)
{
	addSample(cv::Vec2i{posX, posY});
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
