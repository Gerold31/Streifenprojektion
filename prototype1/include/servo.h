#ifndef SERVO_H
#define SERVO_H

#include "singleton.h"

class Servo : public Singleton<Servo>
{
public:
	void setAngle(int a);

private:
	friend class Singleton;

};

#endif
