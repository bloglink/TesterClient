/*******************************************************************************
 * Copyright [2017]   <青岛艾普智能仪器有限公司>
 * All rights reserved.
 *
 * version:     0.1
 * author:      zhaonanlin
 * brief:       横河电参表读取程序
*******************************************************************************/
#include "wt330.h"

WT330::WT330(QObject *parent) : QObject(parent)
{
    com = NULL;
}

bool WT330::initPort(QString portName)
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
        setNumber();
        QTimer *timer = new QTimer(this);
        connect(timer, SIGNAL(timeout()), this, SLOT(readThread()));
        timer->start(50);
        return true;
    } else {
        return false;
    }
}

bool WT330::readThread()
{
    if (com == NULL || !com->isOpen())
        return false;
    if (com->bytesAvailable() == 0) {
        QByteArray cmd = ":NUMERIC:NORMAL:VALUE?";
        cmd.append(0x0A);
        com->write(cmd);  // 读取电参
        return true;
    }

    tempByte.append(com->readAll());
    QStringList temp = QString(tempByte).split(",");
    if (temp.size() == 30) {
        meter = temp;
        tempByte.clear();
    }

    return true;
}

QStringList WT330::readMeter()
{
    return meter;
}

bool WT330::setNumber()
{
    if (com == NULL || !com->isOpen())
        return false;
    QByteArray num = ":NUMERIC:NORMAL:NUMBER 30";
    num.append(0x0A);
    com->write(num);  // 设置读取长度
    return true;
}
