#ifndef RECONSTRUCTOR_H
#define RECONSTRUCTOR_H

#include "deviceconfiguration.h"
#include "line.h"
#include "reconstruction.h"


class Reconstructor
{
public:
	Reconstructor() {}
	virtual ~Reconstructor() {}

	virtual void setDeviceConfiguration(const DeviceConfiguration& conf) = 0;
	virtual void processLine(const Line& line, Reconstruction& cloud) = 0;
};

#endif // RECONSTRUCTOR_H
