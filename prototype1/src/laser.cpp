#include "laser.h"

#include "serial.h"

void Laser::toggle(bool toggle)
{
	char msg[2] = {'l', toggle ? '1' : '0'};
	Serial::getSingleton()->send(msg, 2);
}
