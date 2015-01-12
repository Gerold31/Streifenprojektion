#include <cstdint>
#include <iostream>
#include <istream>
#include <memory>
#include <regex>
#include <string>
#include <thread>

#define GLM_FORCE_RADIANS

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <camerafigure.h>
#include <configuration.h>
#include <laserfigure.h>
#include <plot3d.h>

using glm::mat4;
using glm::rotate;
using glm::translate;
using glm::vec3;
using std::cout;
using std::endl;
using std::getline;
using std::istream;
using std::make_shared;
using std::regex;
using std::regex_match;
using std::shared_ptr;
using std::size_t;
using std::smatch;
using std::string;
using std::thread;


const regex input_point{"\\s*(-?[\\d\\.]+)\\s+(-?[\\d\\.]+)\\s+(-?[\\d\\.]+)\\s+(-?[\\d\\.]+)\\s+(-?[\\d\\.]+)\\s+(-?[\\d\\.]+).*"};
const regex input_cam{"Cam:\\s*(-?[\\d\\.]+)\\s+(-?[\\d\\.]+).*"};
const regex input_laser{"Laser:\\s*(-?[\\d\\.]+)\\s+(-?[\\d\\.]+).*"};

int main(int argc, char *argv[])
{
	Plot3D plot;
	thread *readThread = nullptr;
	bool running = true;

	Configuration::init(argc, argv);

	if (Configuration::drawBox) {
		for (int i = 0; i < 1000000; ++i) {
			float x = (double) rand() / RAND_MAX;
			float y = (double) rand() / RAND_MAX;
			float z = (double) rand() / RAND_MAX;
			plot.addPoint(x, y, z, 1.0f, 1.0f, 1.0f);
		}
	}

	shared_ptr<istream> inp = Configuration::inputStream;
	if (inp != nullptr) {
		readThread = new thread([inp,&plot,&running]() {
			istream &in = *inp;
			shared_ptr<CameraFigure> cam{nullptr};
			shared_ptr<LaserFigure> laser{nullptr};
			while (in.good() && running) {
				string line;
				smatch match;
				getline(in, line);
				if (regex_match(line, match, input_cam)) {
					if (cam == nullptr) {
						cam = make_shared<CameraFigure>();
						plot.addDrawable(cam);
					}
					float fl = atof(match[1].str().c_str());
					float ratio = atof(match[2].str().c_str());
					cam->setConfiguration(fl, ratio, 10.0f);
					if (Configuration::verbose) {
						cout << "Update camera position." << endl;
					}
				} else if (regex_match(line, match, input_laser)) {
					if (laser == nullptr) {
						laser = make_shared<LaserFigure>();
						plot.addDrawable(laser);
					}
					float offset = atof(match[1].str().c_str());
					float angle = atof(match[2].str().c_str());
					mat4& tr = laser->transformation;
					vec3 pos = {-offset, 0.0f, 0.0f};
					tr = translate(mat4{}, pos);
					tr = rotate(tr, angle, vec3{0.0f, 1.0f, 0.0f});
					if (Configuration::verbose) {
						cout << "Update laser position." << endl;
					}
				} else if (regex_match(line, match, input_point)) {
					float data[6];
					for (int i = 1; i <= 6; ++i) {
						data[i-1] = atof(match[i].str().c_str());
					}
					plot.addPoint(data[0], data[1], data[2], data[3], data[4], data[5]);
					if (Configuration::verbose) {
						cout << "Point added: " << data[0] << ", " << data[1] << ", " << data[2] << endl;
						cout << "      Color: " << data[3] << ", " << data[4] << ", " << data[5] << endl;
					}
				} else {
					cout << "Unknown command: " << line << endl;
				}
			}
		});
	}

	plot.show(800, 600);

	if (readThread != nullptr) {
		running = false;
		// TODO stop blocking of thread
		readThread->join();
		delete readThread;
	}

	return EXIT_SUCCESS;
}
