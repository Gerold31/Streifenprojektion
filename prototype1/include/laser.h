#ifndef LASER_H
#define LASER_H

#include "singleton.h"

class Laser : public Singleton<Laser>
{
public:
	void toggle(bool toggle);

private:
	friend class Singleton;

};

#endif
