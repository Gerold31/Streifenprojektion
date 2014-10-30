#include <stdio.h>
#include <opencv2/opencv.hpp>
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <fstream>

using namespace cv;
Mat dst, src, src_gray, detected_edges, ref;
int edgeThresh = 1;
int lowThreshold;
int const max_lowThreshold = 100;
int ratio = 3;
int kernel_size = 3;

const char *win = "Display Image";

void generateMesh(Mat img)
{
    std::vector<Point2i> topLine, bottomLine;

    for(int x = 0; x < img.cols; ++x)
    {
        Point2i top(-1, -1), bottom(-1, -1);
        bool line = false;
        for(int y = 0; y < img.rows; ++y)
        {
            uchar p = img.at<uchar>(y, x);
            if(p == 0)
            {
                if(!line)
                {
                    top = Point2i(x, y);
                    line = true;
                }
                bottom = Point2i(x, y);
            }
        }
        if(line)
        {
            topLine.push_back(top);
            bottomLine.push_back(bottom);
        }
    }

    std::ofstream file("mesh.obj");
    //file.open("mesh.obj", std::ios_base::out);

    file << "o Mesh" << std::endl;

    double avgyt = 0, avgyb = 0;

    for(auto i = topLine.begin(); i!= topLine.end(); ++i)
    {
        avgyt += (*i).y;
    }
    avgyt /= topLine.size();

    for(auto i = bottomLine.begin(); i!= bottomLine.end(); ++i)
    {
        avgyb += (*i).y;
    }
    avgyb /= bottomLine.size();


    auto i = topLine.begin();
    auto j = bottomLine.begin();
    for(;i!= topLine.end(); ++i, ++j)
    {
        int y = (*i).y;
        file << "v " << (*i).x << " " << avgyt << " " << avgyt - y << std::endl;
        y = (*j).y;
        file << "v " << (*j).x << " " << avgyb << " " << avgyb - y << std::endl;
    }

    for(int i=1; i<(topLine.size()-1)*2; i+=2)
    {
        file << "f " << i << " " << i+1 << " " << i+2 << std::endl;
        file << "f " << i+1 << " " << i+3 << " " << i+2 << std::endl;
    }

    std::cout << img.cols << ", " << topLine.size() << ", " << bottomLine.size() << std::endl;


    file.close();
}

/**
 * @function CannyThreshold
 * @brief Trackbar callback - Canny thresholds input with a ratio 1:3
 */
void CannyThreshold(int, void*)
{
  /// Reduce noise with a kernel 3x3
  //blur( src_gray, detected_edges, Size(3,3) );

  /// Canny detector
  //Canny( detected_edges, detected_edges, lowThreshold, lowThreshold*ratio, kernel_size );

  /// Using Canny's output as a mask, we display our result
  dst = Scalar::all(0);

  src_gray.copyTo( dst, detected_edges);

  dst = dst > 64;

  imshow( win, dst );

  generateMesh(dst);
 }

int main(int argc, char** argv )
{
    int deviceId = atoi(argv[1]);
    VideoCapture cap(deviceId);

    dst.create( src.size(), src.type() );

    namedWindow(win, CV_WINDOW_AUTOSIZE );

    createTrackbar( "Min Threshold:", win, &lowThreshold, max_lowThreshold, CannyThreshold );


    if(!cap.isOpened()) {
        std::cerr << "Capture Device ID " << deviceId << " cannot be opened." << std::endl;
        return -1;
    }

    src = imread( argv[1] );

    //cap >> ref;

    while(waitKey(10) < 0)
    {
        //cap >> src;

        //src = max(src, ref) - min(src, ref);

        cv::cvtColor(src, src_gray, CV_BGR2GRAY);

        CannyThreshold(0, 0);
        //imshow(win, src_gray);
    }

    return 0;
}
