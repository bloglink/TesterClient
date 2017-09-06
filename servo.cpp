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

        QTimer *timer = new QTimer(this);
        connect(timer, SIGNAL(timeout()), this, SLOT(readThread()));
        timer->start(50);
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
    switch (status) {
    case SERVO_INIT:
        if (timeOut == 0)
            com->write("0005");
        timeOut++;
        if (com->bytesAvailable() >= 1) {
            status = SERVO_HANK;
            com->readAll();
            timeOut = 0;
            com->write("0001926D00");
        }
        break;
    case SERVO_HANK:
        timeOut++;
        if (com->bytesAvailable() >= 2) {
            status = SERVO_READ;
            com->readAll();
            timeOut = 0;
            com->write("0400");
        }
        break;
    case SERVO_READ:
        timeOut++;
        if (com->bytesAvailable() >= 7) {
            status = SERVO_OVER;
            QByteArray msg = com->readAll();
            speed = quint8(msg.at(3)) + quint8(msg.at(4))*256;
            timeOut = 0;
        }
        break;
    case SERVO_OVER:
        status = SERVO_INIT;
        com->readAll();
        break;
    default:
        break;
    }
    return true;
}
