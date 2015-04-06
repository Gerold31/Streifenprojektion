#include <iostream>

#include "point.h"
#include "pointio.h"

using std::cerr;
using std::endl;


int main(int argc, char *argv[])
{
	Vec3d min, max;
	if (argc == 3) {
		// TODO
		cerr << "Not implemented yet." << endl;
		return EXIT_FAILURE;
	} else if (argc == 7) {
		for (int i = 0; i < 3; ++i) {
			min[i] = atof(argv[i + 1]);
		}
		for (int i = 0; i < 3; ++i) {
			max[i] = atof(argv[i + 4]);
		}
	} else {
		cerr << "Invalid amount of arguments." << endl;
		cerr << endl;
		cerr << "Usage: " << argv[0] << " <min_r>,<min_g>,<min_b> <max_r>,<max_g>,<max_b>" << endl;
		cerr << "   or: " << argv[0] << " <min_r> <min_g> <min_b> <max_r> <max_g> <max_b>" << endl;
		return EXIT_FAILURE;
	}

	Point p;
	while (readPoint(p)) {
		if (p.color >= min && p.color <= max) {
			writePoint(p);
		}
	}

	return EXIT_SUCCESS;
}
