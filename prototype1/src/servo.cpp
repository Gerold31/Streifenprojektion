#include "servo.h"

#include "serial.h"

#define MID_ANGLE (85)

void Servo::setAngle(int a)
{
	char msg[2] = {'m', (char)(MID_ANGLE + a)};
	Serial::getSingleton()->send(msg, 2);
}
