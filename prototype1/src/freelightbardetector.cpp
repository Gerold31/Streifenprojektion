//opencv
#include <opencv\cv.h>
#include <opencv\highgui.h>
//costum
#include "freelightbardetector.h"

void FreeLightBarDetector::findLightBar(const cv::Mat &img, Line &line)
{
	cv::Mat image = filter(img);

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

cv::Mat FreeLightBarDetector::filter(const cv::Mat &img)
{
	cv::Mat ret = img;
	int avrgR = 0, avrgG = 0, avrgB = 0;
	
	for(int i = 0; i < ret.rows; i++)
	{
		for(int j = 0; j < ret.cols; j++)
		{
			avrgR += ret.data[ret.step[0]*i + ret.step[1]*j + 2]; //R
			avrgG += ret.data[ret.step[0]*i + ret.step[1]*j + 1]; //G
			avrgB += ret.data[ret.step[0]*i + ret.step[1]*j + 0]; //B
		}
	}

	int threshholdR = avrgR/(ret.rows*ret.cols);
	int threshholdG = avrgG/(ret.rows*ret.cols);
	int threshholdB = avrgB/(ret.rows*ret.cols);

	threshholdR += (255 - threshholdR)/2;
	threshholdG -= (255 - threshholdG)/2;
	threshholdB -= (255 - threshholdB)/2;

	for(int i = 0; i < ret.rows; i++)
	{
		for(int j = 0; j < ret.cols; j++)
		{
			if(ret.data[ret.step[0]*i + ret.step[1]*j + 2] < threshholdR 
				|| ret.data[ret.step[0]*i + ret.step[1]*j + 1] < threshholdG 
				|| ret.data[ret.step[0]*i + ret.step[1]*j + 0] < threshholdB)
			{
				ret.data[ret.step[0]*i + ret.step[1]*j + 2] = 0;
				ret.data[ret.step[0]*i + ret.step[1]*j + 1] = 0;
				ret.data[ret.step[0]*i + ret.step[1]*j + 0] = 0;
			} else 
			{
				ret.data[ret.step[0]*i + ret.step[1]*j + 2] = 255;
				ret.data[ret.step[0]*i + ret.step[1]*j + 1] = 255;
				ret.data[ret.step[0]*i + ret.step[1]*j + 0] = 255;
			}
		}
	}
	
	// TEST
	cv::namedWindow("line", cv::WINDOW_AUTOSIZE);
	cv::imshow("line", ret);

	return ret;
}