/*******************************************************************************
 * Copyright [2017]   <青岛艾普智能仪器有限公司>
 * All rights reserved.
 *
 * version:     0.1
 * author:      zhaonanlin
 * brief:       伺服控制模块
*******************************************************************************/
#include "servo.h"

Servo::Servo(QObject *parent) : QObject(parent)
{
    com = NULL;
    timeOut = 0;
    status = 0x0000;
    speed = 0x0000;
}

bool Servo::initPort(QString portName)
{
    if (portName.isNull() && com != NULL) {
        com->close();
        return true;
    }
    com = new QSerialPort(portName, this);
    if (com->open(QIODevice::ReadWrite)) {
        com->setBaudRate(9600);
        com->setParity(QSerialPort::NoParity);
        com->setDataBits(QSerialPort::Data8);
        com->setStopBits(QSerialPort::OneStop);
        com->setFlowControl(QSerialPort::NoFlowControl);
        com->setDataTerminalReady(true);
        com->setRequestToSend(false);
        return true;
    } else {
        return false;
    }
}

quint16 Servo::readPort()
{
    return speed;
}

bool Servo::readThread()
{
    if (com == NULL || !com->isOpen())
        return false;
    com->readAll();
    QByteArray cmd;
    quint32 timeOut = 0;
    cmd = QByteArray::fromHex("0005");
    com->write(cmd);
    while (com->bytesAvailable() < 1) {
        wait(10);
        timeOut++;
        if (timeOut > 100) {
            return false;
        }
    }
    com->readAll();

    cmd = QByteArray::fromHex("0001926D00");
    com->write(cmd);
    while (com->bytesAvailable() < 2) {
        wait(10);
        timeOut++;
        if (timeOut > 100) {
            return false;
        }
    }
    com->readAll();

    cmd = QByteArray::fromHex("0400");
    com->write(cmd);
    while (com->bytesAvailable() < 7) {
        wait(10);
        timeOut++;
        if (timeOut > 100) {
            return false;
        }
    }
    QByteArray msg = com->readAll();
    speed = quint8(msg.at(3)) + quint8(msg.at(4))*256;
    return true;
}

void Servo::wait(int ms)
{
    QElapsedTimer t;
    t.start();
    while (t.elapsed() < ms)
        QCoreApplication::processEvents();
}
