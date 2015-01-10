#ifndef DEFAULTRECONSTRUCTOR_H
#define DEFAULTRECONSTRUCTOR_H

#include <deviceconfiguration.h>
#include <reconstructor.h>


class DefaultReconstructor : public Reconstructor
{
public:
	DefaultReconstructor();
	virtual ~DefaultReconstructor();

	virtual void setDeviceConfiguration(const DeviceConfiguration& conf);
	virtual void processLine(const Line& line, Reconstruction& cloud);
private:
	DeviceConfiguration dconf;
};

#endif // DEFAULTRECONSTRUCTOR_H
