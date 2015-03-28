#include <defaultreconstructor.h>

#include <cmath>
#include <iostream>

#include "common.h"
#include "configuration.h"

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

void DefaultReconstructor::processLine(const Line &line, Reconstruction &reconstruction)
{
	//    Object point        |   image plane
	//         C              |     |  2      (rpH/V are image coordinates
	//         /\      :      |    <-------->  as floating point.
	//        /  \     :      |   :\        /  rpV is between -1 and 1.)
	//     b /    \ a  :h     |   : \      /
	//      /      \   :      | fl:  \fov°/
	//     /        \  :      |   :   \  /
	//    /__________\ :      |   :    \/
	//    A      c    B       |      Camera
	//   \/           |       |
	// Camera       Laser     |

	// Prepare interim results
	double fH = cos(dconf.projectorSkew);
	double fV = -sin(dconf.projectorSkew);

	// Get sample independent values (c, beta, fl)
	double c = fH*dconf.projectorPos[0] + fV*dconf.projectorPos[1] + tan(dconf.projectorPitch)*dconf.projectorPos[2];
	double beta = M_PI_2 + dconf.projectorPitch;
	double fl = 1.0 / tan(dconf.fov / 2.0);

	// Print some information
	if (Configuration::verbose) {
		cerr << "Focal length is " << fl << endl;
	}

	// Run through all samples
	for (const Line::Sample& sample : line.getSamples()) {
		// Get normalized image coordinates
		double rpH = ((double) (2 * sample.pos[0]) / (line.getResolution()[0]-1) - 1.)*((double) line.getResolution()[0]/line.getResolution()[1]);
		double rpV = -((double) (2 * sample.pos[1]) / (line.getResolution()[1]-1) - 1.);

		// Get sample dependent values (alpha)
		double alpha = M_PI_2 - atan((fH * rpH + fV * rpV) / fl);

		// Triangulate
		double a = c * sin(beta) / sin(M_PI - beta - alpha);
		double h = sin(alpha) * a;

		if (Configuration::verbose) {
			cerr << "Triangulate point: " << sample.pos[0] << ", "
				 << sample.pos[1] << " (" << rpH << ", " << rpV << ")" << endl;
			cerr << "alpha: " << alpha * 180 / M_PI
				 << ", beta: " << beta * 180 / M_PI
				 << ", c: " << c << " -> h: " << h << endl;
		}

		cv::Vec3d pos{rpH, rpV, -fl};
		pos = pos * (h / fl);

		// Apply transformation from device configuration
		pos = applyTransformation(dconf.transformation, pos);

		// Add point to reconstruction
		reconstruction.addPoint(pos, sample.pos);
	}
}
