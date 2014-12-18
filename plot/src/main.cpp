#include <cstdint>
#include <cstring>
#include <iostream>
#include <thread>

#include <plot3d.h>

using std::cin;
using std::cout;
using std::size_t;
using std::strcmp;
using std::thread;


int main(int argc, char *argv[])
{
	Plot3D plot;
	thread *readThread = nullptr;

	for (size_t i = 1; i < argc; ++i) {
		if (strcmp(argv[i], "--box") == 0) {
			for (int i = 0; i < 1000000; ++i) {
				float x = (double) rand() / RAND_MAX;
				float y = (double) rand() / RAND_MAX;
				float z = (double) rand() / RAND_MAX;
				plot.addPoint(x, y, z, 1.0f, 0.0f, 0.0f);
			}
		} else if (strcmp(argv[i], "--readin") == 0) {
			readThread = new thread([&]() {
				float xyzrgb[6];
				for (;;) {
					for (int i = 0; i < 6; ++i) {
						cin >> xyzrgb[i];
					}
					plot.addPoint(xyzrgb[0], xyzrgb[1], xyzrgb[2], xyzrgb[3], xyzrgb[4], xyzrgb[5]);
					//cout << "Paoint added: " << xyzrgb[0] << ", " << xyzrgb[1] << ", " << xyzrgb[2] << '\n';
					//cout << "       Color: " << xyzrgb[3] << ", " << xyzrgb[4] << ", " << xyzrgb[5] << '\n';
				}
			});
		} else {
			// TODO set view
		}
	}

	plot.show();

	if (readThread != nullptr)
		delete readThread;

	return EXIT_SUCCESS;
}
