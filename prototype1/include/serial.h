#ifndef SERIAL_H
#define SERIAL_H

#include <QSerialPort>

#include "singleton.h"

class Serial : public Singleton<Serial>
{
public:
	void send(const char *msg, size_t len);

private:
	Serial();

	QSerialPort *mSerial;

	friend class Singleton;

};

#endif
