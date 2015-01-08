#include <defaultreconstructor.h>

#include <cmath>
#include <iostream>

#include <configuration.h>

using std::atan;
using std::cerr;
using std::cos;
using std::endl;
using std::sin;
using std::tan;


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
	//    Object point        |   image plane
	//         C              |     |  2      (rpH/V are image coordinates
	//         /\             |    <-------->  as float from -1.0 to 1.0)
	//        /  \            |   :\        /
	//     b /    \ a         |   : \      /
	//      /      \          | fw:  \fov°/
	//     /        \         |   :   \  /
	//    /__________\        |   :    \/
	//   A      c     B       |      Camera
	//   |            \/      |
	// Laser        Camera    |

	double c = dconf.projectorOffset;
	double alpha = M_PI_2 + dconf.projectorAngle;
	double fw = 1.0 / tan(dconf.fov / 2.0);

	cerr << "Focal length is " << fw << endl;

	for (const Line::Sample& sample : line.getSamples()) {
		double rpH = (2.0 * ((double) sample.pos[0] / line.getResolution()[0]) - 1.0 + (1.0 / (2*line.getResolution()[0])))*((double) line.getResolution()[0]/line.getResolution()[1]);
		double rpV = -(2.0 * ((double) sample.pos[1] / line.getResolution()[1]) - 1.0 + (1.0 / (2*line.getResolution()[1])));

		double beta = M_PI_2 + atan(rpH / fw);
		double a = c * sin(alpha) / sin(M_PI - alpha - beta);
		double h = sin(beta) * a;

		if (Configuration::verbose) {
			cerr << "Triangulate point from image: (" << sample.pos[0] << ", "
				 << sample.pos[1] << "), relative: (" << rpH << ", " << rpV
				 << ")" << endl;
			cerr << "distance: " << h << ", beta: " << beta*180/M_PI
				 << ", alpha: " << alpha*180/M_PI << ", c: " << c << ", a: "
				 << a << endl;
		}

		cv::Vec3d pos{rpH, rpV, fw};
		pos = pos * (h / fw);
		cloud.addPoint(pos, sample.pos);
	}
}
