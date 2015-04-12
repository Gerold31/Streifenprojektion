#include "peaklightbardetector.h"

#include <algorithm>
#include <vector>

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#include "configuration.h"

using cv::imshow;
using cv::Mat;
using cv::medianBlur;
using cv::namedWindow;
using cv::Size;
using cv::Vec2i;
using cv::Vec3b;
using cv::Vec3d;
using cv::Vec3i;
using std::max;
using std::min;


struct WorkData {
	Vec2i pos;
	Mat img;        // CV_8UC3
	Mat areaAvgMat; // CV_8UC3
	Mat evalMat;    // CV_64FC1
	Mat filtered;   // CV_U8C1 (use 255 and 0 only)
	Mat lineMat;    // CV_U8C1 (use booleans)
};

#define WINDOW_AREA       "Peak: Average"
#define WINDOW_EVALUATION "Peak: Evaluation"
#define WINDOW_FILTER     "Peak: Filter"

static bool window_created = false;

void PeakLightBarDetector::processImage(const cv::Mat& img, Line& line)
{
	WorkData data;
	medianBlur(img, data.img, median);

	// produce mat with evaluation
	computeAreaAvg(data);
	computeEval(data);

	// decide whether it is part of line (using evaluation
	data.filtered = {Size{data.img.cols, data.img.rows}, CV_8UC1, cv::Scalar{0}};
	for (int r = 0; r < img.rows; ++r) {
		for (int c = 0; c < img.cols; ++c) {
			if (data.evalMat.at<double>(r, c) >= threshold) {
				data.filtered.at<uchar>(r, c) = 255;
			}
		}
	}

	// unly use the center of the line
	computeLine(data);

	// remove points without neighbors
	for (int r = 0; r < img.rows; ++r) {
		for (int c = 0;  c < img.cols; ++c) {
			if (!data.lineMat.at<uchar>(r, c))
				continue;

			bool n = false;
			for (int i = max(r - 1, 0); i < min(r + 2, img.rows); ++i)
				for (int j = max(c - 1, 0); j < min(c + 2, img.cols); ++j)
					if (i != r || j != c)
						n = n || data.lineMat.at<uchar>(i, j);

			if (!n)
				data.lineMat.at<uchar>(r, c) = false;
		}
	}

	// add points to line
	for (int r = 0; r < img.rows; ++r) {
		for (int c = 0; c < img.cols; ++c) {
			if (data.lineMat.at<uchar>(r, c))
				line.addSample(c, r);
		}
	}

	// show debug windows
	if (Configuration::verbose2) {
		if (!window_created) {
			namedWindow(WINDOW_AREA);
			namedWindow(WINDOW_EVALUATION);
			namedWindow(WINDOW_FILTER);
			window_created = true;
		}
		imshow(WINDOW_AREA, data.areaAvgMat);
		imshow(WINDOW_EVALUATION, data.evalMat);
		imshow(WINDOW_FILTER, data.filtered);
	}
}

void PeakLightBarDetector::computeAreaAvg(WorkData& data) const
{
	Vec3i areaSum{0, 0, 0};
	int termsOfSum = 0;
	data.areaAvgMat = {Size{data.img.cols, data.img.rows}, CV_8UC3};

	for (int r = 0; r < areaSize[1] / 2 + 1 && r < data.img.rows; ++r) {
		for (int c = 0; c < areaSize[0] / 2 + 1 && c < data.img.cols; ++c) {
			areaSum += data.img.at<Vec3b>(r, c);
			termsOfSum += 1;
		}
	}

	for (int r = 0; r < data.img.rows; ++r) {
		for (int c = 0; c < data.img.cols; ++c) {
			Vec3d areaAvg = Vec3d{areaSum} / (termsOfSum * 255);
			data.areaAvgMat.at<Vec3b>(r, c) = Vec3b{areaAvg * 255};
			for (int l = max(0, r - areaSize[1] / 2); l < r + areaSize[1] / 2 && l < data.img.rows ; ++l) {
				if (c - areaSize[0] / 2 >= 0) {
					areaSum -= data.img.at<Vec3b>(l, c - areaSize[0] / 2);
					termsOfSum -= 1;
				}
				if (c + areaSize[0] / 2 + 1 < data.img.cols) {
					areaSum += data.img.at<Vec3b>(l, c + areaSize[0] / 2 + 1);
					termsOfSum += 1;
				}
			}
		}

		if (++r >= data.img.rows)
			break;

		for (int c = max(0, data.img.cols - areaSize[0] / 2); c < data.img.cols; ++c) {
			if (r - areaSize[1] / 2 - 1 >= 0) {
				areaSum -= data.img.at<Vec3b>(r - areaSize[1] / 2 - 1, c);
				termsOfSum -= 1;
			}
			if (r + areaSize[1] / 2 < data.img.rows) {
				areaSum += data.img.at<Vec3b>(r + areaSize[1] / 2, c);
				termsOfSum += 1;
			}
		}

		for (int c = data.img.cols - 1; c >= 0; --c) {
			for (int l = max(0, r - areaSize[1] / 2); l < r + areaSize[1] / 2 && l < data.img.rows ; ++l) {
				if (c + areaSize[0] / 2 + 1 < data.img.cols) {
					areaSum -= data.img.at<Vec3b>(l, c + areaSize[0] / 2 + 1);
					termsOfSum -= 1;
				}
				if (c - areaSize[0] / 2 >= 0) {
					areaSum += data.img.at<Vec3b>(l, c - areaSize[0] / 2);
					termsOfSum += 1;
				}
			}
			Vec3d areaAvg = Vec3d{areaSum} / (termsOfSum * 255);
			data.areaAvgMat.at<Vec3b>(r, c) = Vec3b{areaAvg * 255};
		}
	}
}

void PeakLightBarDetector::computeEval(WorkData& data) const
{
	data.evalMat = {Size{data.img.cols, data.img.rows}, CV_64FC1};

	for (int r = 0; r < data.img.rows; ++r) {
		for (int c = 0; c < data.img.cols; ++c) {
			Vec3d color = Vec3d{data.img.at<Vec3b>(r, c)} / 255.;
			Vec3d avg = Vec3d{data.areaAvgMat.at<Vec3b>(r, c)} / 255.;

			Vec3d diff = color - avg;
			Vec3d rest = Vec3d::all(1.) - avg;
			Vec3d relative;
			for (int i = 0; i < 3; ++i) {
				relative[i] = rest[i] > 0. ? diff[i] / rest[i] : 0.5;
			}

			double eval = evaluate(relative);
			data.evalMat.at<double>(r, c) = max(eval, 0.);
		}
	}
}

void PeakLightBarDetector::computeLine(WorkData &data) const
{
	Mat tmp;
	data.lineMat = {Size{data.img.cols, data.img.rows}, CV_8UC1, cv::Scalar{0}};

	cv::GaussianBlur(data.filtered, tmp, Size{11, 11}, 0, 0);

	for (int r = 0; r < data.img.rows; ++r) {
		for (int c = 0; c < data.img.cols; ++c) {
			int n = 0;
			uchar val = tmp.at<uchar>(r, c);

			for (int i = -1; i <= 1; ++i) {
				int r1 = r + i;
				int r2 = r - i;
				int c1 = c - 1;
				int c2 = c + 1;
				if (min(r1, r2) >= 0 && max(r1, r2) < data.img.rows
						&& min(c1, c2) >= 0 && max(c1, c2) < data.img.cols
						&& tmp.at<uchar>(r1, c1) <= val
						&& tmp.at<uchar>(r2, c2) <= val) {

					if (tmp.at<uchar>(r1, c1) == val
							&& r + 2*i >= 0 && r + 2*i < data.img.rows
							&& c - 2 >= 0
							&& tmp.at<uchar>(r + 2*i, c - 2) >= val)
						continue;
					if (tmp.at<uchar>(r2, c2) == val
							&& r - 2*i >= 0 && r - 2*i < data.img.rows
							&& c + 2 < data.img.cols
							&& tmp.at<uchar>(r - 2*i, c + 2) >= val)
						continue;
					++n;
				}
			}

			if (min(r - 1, r + 1) >= 0 && max(r - 1, r + 1) < data.img.rows
					&& tmp.at<uchar>(r - 1, c) <= val
					&& tmp.at<uchar>(r + 1, c) <= val) {

				if (tmp.at<uchar>(r - 1, c) == val
						&& r - 2 >= 0
						&& tmp.at<uchar>(r - 2, c) >= val)
					continue;
				if (tmp.at<uchar>(r + 1, c) == val
						&& r + 2 < data.img.rows
						&& tmp.at<uchar>(r + 2, c) >= val)
					continue;
				++n;
			}

			if (n >= 2)
				data.lineMat.at<uchar>(r, c) = true;
		}
	}
}

double PeakLightBarDetector::evaluate(const Vec3d color) const
{
	double r = color[2], g = color[1], b = color[0];
	double result = 0;
	result += 0.6 * r;
	result += 0.2 * g;
	result += 0.2 * b;
	return result;
}
