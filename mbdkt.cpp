/*******************************************************************************
 * Copyright [2017]   <青岛艾普智能仪器有限公司>
 * All rights reserved.
 *
 * version:     0.1
 * author:      zhaonanlin
 * brief:       MBDKT2510 PLC控制模块
*******************************************************************************/
#include "mbdkt.h"

MBDKT::MBDKT(QObject *parent) : QObject(parent)
{
    com = NULL;
}

bool MBDKT::initPort(QString portName)
{
    if (portName.isNull() && com != NULL) {
        com->close();
        return true;
    }
    com = new QSerialPort(portName, this);
    if (com->open(QIODevice::ReadWrite)) {
        com->setBaudRate(9600);
        com->setParity(QSerialPort::OddParity);
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

bool MBDKT::setMode(int mode)
{
    QByteArray cmd;
    if (mode == 0)
        cmd = "%01#WCSR00011**";  // 转矩模式
    if (mode == 1)
        cmd = "%01#WCSR00010**";  // 速度模式
    if (sendCommand(cmd))
        return true;
    else
        return false;
}

bool MBDKT::setTurn(int turn)
{
    QByteArray cmd;
    if (turn == 0)
        cmd = "%01#WCSR00030**";  // 方向0
    if (turn == 1)
        cmd = "%01#WCSR00031**";  // 方向1
    if (sendCommand(cmd))
        return true;
    else
        return false;
}

bool MBDKT::setStart(int start)
{
    QByteArray cmd;
    if (start == 0)
        cmd = "%01#WCSR00000**";  // 停止
    if (start == 1)
        cmd = "%01#WCSR00001**";  // 启动
    if (sendCommand(cmd))
        return true;
    else
        return false;
}

bool MBDKT::setSpeed(int speed)
{
    speed = speed*2;
    quint16 temp = speed/256 + (speed%256)*256;
    QByteArray cmd = "%01#WDD0010000101";
    cmd.append(QString("%1").arg(temp, 4, 16, QChar('0')).toUtf8().toUpper());
    cmd.append("0000**");
    if (sendCommand(cmd))
        return true;
    else
        return false;
}

bool MBDKT::setTorque(int torque)
{
    quint16 temp = torque/256 + (torque%256)*256;
    QByteArray cmd = "%01#WDD00100001010000";
    cmd.append(QString("%1").arg(temp, 4, 16, QChar('0')).toUtf8().toUpper());
    cmd.append("**");
    if (sendCommand(cmd))
        return true;
    else
        return false;
}

void MBDKT::wait(int ms)
{
    QElapsedTimer t;
    t.start();
    while (t.elapsed() < ms)
        QCoreApplication::processEvents();
}

bool MBDKT::sendCommand(QByteArray cmd)
{
//    wait(10);
//    return true;
    if (com == NULL || !com->isOpen())
        return false;
    quint32 timeOut = 0;
    cmd.append(0x0D);
    com->write(cmd);
    timeOut = 0;
    while (com->bytesAvailable() < 7) {
        wait(10);
        timeOut++;
        if (timeOut > 100)
            return false;
    }
    QByteArray msg = com->readAll();
    if (msg.at(3) == '!')
        return false;
    else
        return true;
}
