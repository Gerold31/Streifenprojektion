#include "servo.h"

#include "serial.h"

#define MID_ANGEL (87)

void Servo::setAngle(int a)
{
	char msg[2] = {'m', (char)(MID_ANGEL + a)};
	Serial::getSingleton()->send(msg, 2);
}
