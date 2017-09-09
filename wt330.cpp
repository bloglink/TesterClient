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
    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(readThread()));
}

bool WT330::initPort(QString portName)
{
    if (portName.isNull() && com != NULL) {
        timer->stop();
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
        curr.clear();
        setNumber();
        timer->start(500);
        return true;
    } else {
        return false;
    }
}

bool WT330::readThreads()
{
    if (com == NULL || !com->isOpen())
        return false;
    if (com->bytesAvailable() > 0) {
        tmp.append(com->readAll());
        return true;
    }
    QStringList temp = QString(tmp).split(",");
    if (temp.size() >= 30) {
        curr = temp;
        tmp.clear();
        return true;
    }
    QByteArray cmd = ":NUMERIC:NORMAL:VALUE?";
    cmd.append(0x0A);
    com->write(cmd);  // 读取电参
    return true;
}

bool WT330::readThread()
{
    if (com == NULL || !com->isOpen())
        return false;
    QByteArray cmd = ":NUMERIC:NORMAL:VALUE?";
    cmd.append(0x0A);
    com->write(cmd);  // 读取电参
    quint32 timeOut =0;
    QByteArray tmp;
    while (1) {
        wait(50);
        if (com->bytesAvailable() > 0)
            tmp.append(com->readAll());
        if (QString(tmp).split(",").size() >= 30) {
            curr.append(tmp);
            tmp.clear();
            return true;
        }
        timeOut++;
        if (timeOut > 10)
            return false;
    }
}

void WT330::wait(int ms)
{
    QElapsedTimer t;
    t.start();
    while (t.elapsed() < ms)
        QCoreApplication::processEvents();
}

QStringList WT330::readMeter()
{
    QStringList xx;
    if (curr.isEmpty())
        return xx;
    QList<double> x;
    for (int i=0; i < curr.size(); i++) {
        QStringList t = curr.at(i).split(",");
        if (t.size() >= 30)
            x.append(t.at(1).toDouble());
        else
            x.append(0);
    }
    double max = 0;
    for (int i=0; i < x.size(); i++)
        max = qMax(max, x.at(i));
    int t = x.indexOf(max);
    if (t >= 0)
        xx = curr.at(t).split(",");
    return xx;
}

bool WT330::setNumber()
{
    if (com == NULL || !com->isOpen())
        return false;
    QByteArray num = ":NUMERIC:NORMAL:NUMBER 30";
    num.append(0x0A);
    com->write(num);  // 设置读取长度
    wait(50);
    return true;
}
