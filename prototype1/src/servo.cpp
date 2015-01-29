#include "servo.h"

#include "serial.h"

//#define MID_ANGEL (85)
#define MID_ANGEL (90)

void Servo::setAngle(int a)
{
	char msg[2] = {'m', (char)(MID_ANGEL + a)};
	Serial::getSingleton()->send(msg, 2);
}
