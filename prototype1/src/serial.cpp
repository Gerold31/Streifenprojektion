#include "serial.h"

#include <QSerialPortInfo>

Serial::Serial()
{
	mSerial = new QSerialPort;
	foreach(const QSerialPortInfo &info, QSerialPortInfo::availablePorts())
	{
		if(info.description() == "Spark Core with WiFi")
		{
			mSerial->setPort(info);
			mSerial->open(QIODevice::ReadWrite);
			break;
		}
	}
}

void Serial::send(const char *msg, size_t len)
{
	if(mSerial->isOpen())
	{
		mSerial->write(msg, len);
		mSerial->flush();
	}
}
