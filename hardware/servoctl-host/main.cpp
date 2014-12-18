#include <QCoreApplication>

#include <QSerialPort>
#include <QSerialPortInfo>

#include <QDebug>
#include <QThread>

#include <iostream>
#include <fstream>

int main(int argc, char **argv)
{
    if(argc != 2)
        return 1;


    QCoreApplication a(argc, argv);

    foreach (const QSerialPortInfo &info, QSerialPortInfo::availablePorts()) {
        qDebug() << "Name        : " << info.portName();
        qDebug() << "Description : " << info.description();
        qDebug() << "Manufacturer: " << info.manufacturer();

        // Example use QSerialPort
        QSerialPort serial;
        serial.setPort(info);
        if (serial.open(QIODevice::ReadWrite)) // Hang of the program
        {
            int pos;
            //while(std::cin >> pos)
            while(1)
            {
                serial.write("l1", 2);
                if(!serial.waitForBytesWritten(500))
                    std::cout << "timeout" << std::endl;
                for(pos = 0; pos < 90; pos++)
                {
                    serial.write("m", 1);
                    int p = 90 + pos; //+(pos > 90 ? 180-pos : pos);
                    std::cout << p << std::endl;
                    serial.write(QByteArray(1, (unsigned char)p));
                    if(!serial.waitForBytesWritten(500))
                        std::cout << "timeout" << std::endl;
                    if(serial.bytesAvailable())
                        std::cout << serial.readAll().toStdString() << std::endl;

                    QThread::msleep(250);
                }
                serial.write("l0", 2);
                serial.write("mZ", 2);
                if(!serial.waitForBytesWritten(500))
                    std::cout << "timeout" << std::endl;
                QThread::msleep(1000);
            }
        }
    }

    /*
    QSerialPort serialPort;

    serialPort.setPortName(argv[1]);
    serialPort.setBaudRate(QSerialPort::Baud19200);
    if(!serialPort.open(QSerialPort::ReadWrite))
    {
        std::cout << serialPort.errorString().toStdString() << std::endl;
    }

    int pos;
    while(std::cin >> pos)
    {
        std::cout << serialPort.write(QByteArray(1, (unsigned char)pos)) << std::endl;
        if(serialPort.bytesAvailable())
            std::cout << serialPort.readAll().toStdString() << std::endl;
    }
    */

    return 0;
}
