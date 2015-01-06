#include <defaultlinedetector.h>

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>

using cv::Mat;


DefaultLineDetector::DefaultLineDetector()
{

}

DefaultLineDetector::~DefaultLineDetector()
{

}

void DefaultLineDetector::setBackground(const Mat &background)
{
	this->background = background;
}

void DefaultLineDetector::processImage(const Mat &img, Line &line)
{
	Mat dif = diff(background, img);

	int countBlack = 0, countWhite = 0;
	bool pass = false;

	for (int j = 0; j < dif.cols; j++)
	{
		for (int i = 0; i < dif.rows; i++)
		{
			if (dif.data[dif.step[0]*i + dif.step[1]*j] == 0)
			{
				if (pass)
					break;
				countBlack++;
			}
			else
			{
				countWhite++;
				pass = true;
			}
		}
		if (countBlack < dif.rows) {
			line.addSample(j, countBlack);
			// TODO line.addSample(j, countBlack + countWhite/2);
		}

		countBlack = 0;
		countWhite = 0;
		pass = false;
	}
}

Mat DefaultLineDetector::diff(const Mat& back, const Mat& img) const
{
	Mat dif = img - back;

	//cv::namedWindow("difference");
	//cv::imshow("difference", dif);

	for (int i = 0; i < dif.rows; i++)
	{
		for (int j = 0; j < dif.cols; j++)
		{
			if (dif.data[dif.step[0]*i + dif.step[1]*j + 0] < 75
					&& dif.data[dif.step[0]*i + dif.step[1]*j + 1] < 75
					&& dif.data[dif.step[0]*i + dif.step[1]*j + 2] < 75)
			{
				dif.data[dif.step[0]*i + dif.step[1]*j + 0] = 0;
				dif.data[dif.step[0]*i + dif.step[1]*j + 1] = 0;
				dif.data[dif.step[0]*i + dif.step[1]*j + 2] = 0;
			}
			else
			{
				dif.data[dif.step[0]*i + dif.step[1]*j + 0] = 255;
				dif.data[dif.step[0]*i + dif.step[1]*j + 1] = 255;
				dif.data[dif.step[0]*i + dif.step[1]*j + 2] = 255;
			}
		}
	}

	//cv::namedWindow("difference filtered");
	//cv::imshow("difference filtered", dif);

	return dif;
}
