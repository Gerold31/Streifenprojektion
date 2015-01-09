#include <cstdint>
#include <iostream>
#include <istream>
#include <thread>

#include <configuration.h>
#include <plot3d.h>

using std::cout;
using std::endl;
using std::istream;
using std::shared_ptr;
using std::size_t;
using std::thread;


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
			float data[6];
			while (in.good() && running) {
				for (int i = 0; i < 6; ++i) {
					in >> data[i];
				}
				plot.addPoint(data[0], data[1], data[2], data[3], data[4], data[5]);
				if (Configuration::verbose) {
					cout << "Paoint added: " << data[0] << ", " << data[1] << ", " << data[2] << endl;
					cout << "       Color: " << data[3] << ", " << data[4] << ", " << data[5] << endl;
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
