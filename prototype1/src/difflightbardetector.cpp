//opencv
#include <opencv\cv.h>
#include <opencv\highgui.h>
//costum
#include "DiffLightBarDetector.h"

void DiffLightBarDetector::setBackground(const cv::Mat& background)
{
	this->back = background;
}

void DiffLightBarDetector::findLightBar(const cv::Mat& img, Line& line)
{
	cv::Mat image = difference(img);

	int countBlack = 0, countWhite = 0;
	bool pass = false;

	for(int i = 0; i < image.rows; i++)
	{
		for(int j = 0; j < image.cols; j++)
		{
			if(image.data[image.step[0]*i + image.step[1]* j + 0] == 0)
			{
				if(pass)
				{
					line.addSample(countBlack, i, countWhite);
					countBlack += countWhite;
					countWhite = 0;
					pass = false;
				}
				countBlack++;
			} else {
				countWhite++;
				pass = true;
			}
		}

		if(countWhite)
			line.addSample(countBlack, i, countWhite);

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
				dif.data[dif.step[0]*i + dif.step[1]* j + 0] = 0; //B
				dif.data[dif.step[0]*i + dif.step[1]* j + 1] = 0; //G
				dif.data[dif.step[0]*i + dif.step[1]* j + 2] = 0; //R
			}
			else
			{
				dif.data[dif.step[0]*i + dif.step[1]* j + 0] = 255; //B
				dif.data[dif.step[0]*i + dif.step[1]* j + 1] = 255; //G
				dif.data[dif.step[0]*i + dif.step[1]* j + 2] = 255; //R
			}
		}
	}

	//cv::namedWindow("difference filtered", cv::WINDOW_AUTOSIZE);
	//cv::imshow("difference filtered", dif);

	return dif;
}