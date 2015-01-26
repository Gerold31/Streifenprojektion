#include <QCoreApplication>

#include <QSerialPort>
#include <QSerialPortInfo>

#include <QDebug>
#include <QThread>

#include <iostream>
#include <fstream>
#include <thread>
#include <chrono>

int main(int argc, char **argv)
{
    QCoreApplication a(argc, argv);

    QSerialPort serial;

    foreach(const QSerialPortInfo &info, QSerialPortInfo::availablePorts())
    {
        if(info.description() == "Spark Core with WiFi")
        {
            serial.setPort(info);
            serial.open(QIODevice::ReadWrite);
            break;
        }
    }

    if(serial.isOpen())
    {
        char cmd;
        int val;
        while(std::cin >> cmd && std::cin >> val)
        {
            std::cout << "cmd: " << cmd << ", val: " << val << std::endl;
            switch(cmd)
            {
            case 'm':
                serial.write("m", 1);
                serial.write(QByteArray(1, (unsigned char)val));
                break;
            case 'l':
                serial.write("l");
                serial.write(QByteArray(1, val == 0 ? '0' : '1'));
                break;
            case 's':
                while(1)
                {
                    serial.write("l1");
                    serial.flush();
                    std::this_thread::sleep_for(std::chrono::milliseconds(500/val));
                    serial.write("l0");
                    serial.flush();
                    std::this_thread::sleep_for(std::chrono::milliseconds(500/val));
                }
            }
            serial.flush();
        }
    }


    return 0;
}
