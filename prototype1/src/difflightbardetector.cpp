#include "difflightbardetector.h"

#include <opencv/cv.h>
#include <opencv/highgui.h>

using cv::Mat;


void DiffLightBarDetector::setBackground(const cv::Mat& background)
{
	this->back = background;
}

void DiffLightBarDetector::processImage(const cv::Mat& img, Line& line)
{
	cv::Mat dif = difference(img);
	int countBlack = 0, countWhite = 0;
	bool pass = false;

	for(int i = 0; i < dif.rows; i++)
	{
		for(int j = 0; j < dif.cols; j++)
		{
			if(dif.data[dif.step[0]*i + dif.step[1]* j + 0] == 0)
			{
				if(pass)
					break;
				countBlack++;
			} else {
				countWhite++;
				pass = true;
			}
		}

		if(countBlack < dif.cols) {
			line.addSample(countBlack, i, countWhite);
		}

		countBlack = 0;
		countWhite = 0;
		pass = false;
	}
}

cv::Mat DiffLightBarDetector::difference(const cv::Mat& img) const
{
	cv::Mat dif = img - this->back;

	//cv::namedWindow("difference", cv::WINDOW_AUTOSIZE);
	//cv::imshow("difference", dif);

	for(int i = 0; i < dif.rows; i++)
	{
		for(int j = 0; j < dif.cols; j++)
		{
			if(dif.data[dif.step[0]*i + dif.step[1]* j + 0] < 75
				&& dif.data[dif.step[0]*i + dif.step[1]* j + 1] < 75
				&& dif.data[dif.step[0]*i + dif.step[1]* j + 2] < 75)
			{
				dif.data[dif.step[0]*i + dif.step[1]* j + 0] = 0;
				dif.data[dif.step[0]*i + dif.step[1]* j + 1] = 0;
				dif.data[dif.step[0]*i + dif.step[1]* j + 2] = 0;
			}
			else
			{
				dif.data[dif.step[0]*i + dif.step[1]* j + 0] = 255;
				dif.data[dif.step[0]*i + dif.step[1]* j + 1] = 255;
				dif.data[dif.step[0]*i + dif.step[1]* j + 2] = 255;
			}
		}
	}

	//cv::namedWindow("difference filtered", cv::WINDOW_AUTOSIZE);
	//cv::imshow("difference filtered", dif);

	return dif;
}
