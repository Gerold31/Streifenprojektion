#include <plot3d.h>

int main(int argc, char *argv[])
{
	Plot3D plot;
	for (int i = 0; i < 1000000; ++i) {
		plot.addPoint((double) rand() / RAND_MAX, (double) rand() / RAND_MAX, (double) rand() / RAND_MAX, 1.0f, 0.0f, 0.0f);
	}
	plot.show();
}
