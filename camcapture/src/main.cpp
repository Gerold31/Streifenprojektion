/*
 * main.cpp
 *
 *  Created on: 13.11.2014
 *      Author: jojo
 */

#include <cstdio>
#include <cstring>
#include <ctime>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <opencv2/highgui/highgui.hpp>

using namespace cv;
using namespace std;

const char MYWINDOW_NAME[] = "Camera";

int main(int argc, char *argv[]) {
	int device = 0;
	bool gui = false;
	bool addTime = false;
	string filename = "frame.png";

	for (int i = 1; i < argc; ++i) {
		if (strcmp(argv[i], "-d") == 0) {
			device = atoi(argv[++i]);
		} else if (strcmp(argv[i], "-g") == 0) {
			gui = true;
		} else if (strcmp(argv[i], "-o") == 0) {
			filename = argv[++i];
		} else if (strcmp(argv[i], "-t") == 0) {
			addTime = true;
		} else if (strcmp(argv[i], "-?") == 0) {
			cout << "Usage: " << argv[0] << " [options...]" << endl;
			cout << "" << endl;
			cout << "Option  Argument        Description" << endl;
			cout << "  -d    <device>    Set device to use." << endl;
			cout << "  -g                Use a gui." << endl;
			cout << "  -o   <filename>   Change the name of output files." << endl;
			cout << "  -t                Add current date to filenames." << endl;
			cout << "  -?                Show this page." << endl;
			return EXIT_SUCCESS;
		} else {
			cerr << "Invalid argument: " << argv[i] << endl;
			cerr << "Type '" << argv[0] << " -?' for help." << endl;
			return EXIT_FAILURE;
		}
	}

	if (addTime) {
		time_t Zeitstempel;
		tm *nun;
		Zeitstempel = time(0);
		nun = localtime(&Zeitstempel);

		int year = nun->tm_year + 1900;
		int month = nun->tm_mon + 1;
		int day = nun->tm_mday;
		int hour = nun->tm_hour;
		int min = nun->tm_min;
		int sec = nun->tm_sec;

		size_t dotPos = filename.find_last_of('.');
		stringstream stream;
		stream << filename.substr(0, dotPos) << '_'
			<< setw(4) << setfill('0') << year << '-'
			<< setw(2) << setfill('0') << month << '-'
			<< setw(2) << setfill('0') << day << '_'
			<< setw(2) << setfill('0') << hour << '-'
			<< setw(2) << setfill('0') << min << '-'
			<< setw(2) << setfill('0') << sec
			<< filename.substr(dotPos);
		filename = stream.str();
	}

	// ---

	VideoCapture cap(device);
	if (!cap.isOpened()) {
		cerr << "Cannot open cammera." << endl;
		return EXIT_FAILURE;
	}

	if (gui) {
		namedWindow(MYWINDOW_NAME, CV_WINDOW_AUTOSIZE);
		
		int i = 1;
		while (true) {
			Mat frame;
			bool ret = cap.read(frame);

			if (!ret) {
				cerr << "Cannot read image from camera." << endl;
				return EXIT_FAILURE;
			}
			
			imshow(MYWINDOW_NAME, frame);
			
			const int key = waitKey(30) % 256;
			if (key >= 0) {
				switch (key) {
					case ' ':
						cout << "Capturing image ...";
						{
							size_t dotPos = filename.find_last_of('.');
							stringstream name;
							name << filename.substr(0, dotPos) << '_' << i++ << filename.substr(dotPos);
							imwrite(name.str(), frame);
						}
						cout << " done." << endl;
						break;
					case 27:
						cout << "Stop application." << endl;
						return EXIT_SUCCESS;
						break;
					default:
						cout << "Unknown key: " << key << endl;
						break;
				}
			}
		}
	} else {
		Mat frame;
		bool ret = cap.read(frame);

		if (!ret) {
			cerr << "Cannot read image from camera." << endl;
			return EXIT_FAILURE;
		}

		imwrite(filename, frame);
	}
	return EXIT_SUCCESS;
}
