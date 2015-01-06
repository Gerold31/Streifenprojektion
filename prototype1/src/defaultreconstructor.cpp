#include <defaultreconstructor.h>

#include <cmath>
#include <iostream>

using std::cerr;
using std::endl;


DefaultReconstructor::DefaultReconstructor()
{

}

DefaultReconstructor::~DefaultReconstructor()
{

}

void DefaultReconstructor::setDeviceConfiguration(const DeviceConfiguration &conf)
{
	this->dconf = conf;
}

void DefaultReconstructor::processLine(const Line &line, PointCloud &cloud)
{
	double c = dconf.projectorOffset;
	double alpha = M_PI_2 - dconf.projectorAngle;

	for (const Line::Sample& sample : line.getSamples()) {
		double rhp = (2.0 * ((double) sample.pos[0] / line.getResolution()[0]) - 1.0 + (1.0 / (2*line.getResolution()[0])))*(line.getResolution()[0]/line.getResolution()[1]);
		double rvp = -(2.0 * ((double) sample.pos[1] / line.getResolution()[1]) - 1.0 + (1.0 / (2*line.getResolution()[1])));
		double fw = 1.0/std::tan(dconf.fov / 2.0);
		double beta = M_PI_2 + std::atan(rvp / fw);
		double a = c * std::sin(alpha) / std::sin(M_PI - alpha - beta);
		cerr << "rp: (" << rhp << "," << rvp << "), fw: " << fw << ", beta: " << beta << ", a: " << a << endl;
		cv::Vec3d pos{rhp, rvp, fw};
		pos = pos * (a / fw);
		cloud.addPoint(pos, sample.pos);
	}
}
