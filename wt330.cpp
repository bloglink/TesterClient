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
    timeOut = 0;
    wt330_mode = WT330_INIT;
}

bool WT330::initPort(QString portName)
{
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

bool WT330::readThread(int waitTime)
{
    if (com == NULL || !com->isOpen())
        return false;
    QByteArray num = ":NUMERIC:NORMAL:NUMBER 30";
    num.append(0x0A);
    QByteArray cmd = ":NUMERIC:NORMAL:VALUE?";
    cmd.append(0x0A);
    switch (wt330_mode) {
    case WT330_INIT:
        com->write(num);  // 设置读取长度
        wt330_mode = WT330_WAIT;
        break;
    case WT330_WAIT:
        if (timeOut == 0)
            com->write(cmd);  // 读取电参
        timeOut++;
        if (timeOut > waitTime) {  // 读取频率
            timeOut = 0;
            wt330_mode = WT330_READ;
        }
        break;
    case WT330_READ:
        if (com->bytesAvailable() > 30) {
            power = com->readAll();
            wt330_mode = WT330_WAIT;
        }
        break;
    default:
        break;
    }
    return true;
}

bool WT330::quitPort()
{
    if (com == NULL || !com->isOpen())
        return false;
    com->close();
    return true;
}
