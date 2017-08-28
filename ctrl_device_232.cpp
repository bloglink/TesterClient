/*******************************************************************************
 * Copyright [2017]   <青岛艾普智能仪器有限公司>
 * All rights reserved.
 *
 * version:     0.1
 * author:      zhaonanlin
 * brief:       232控制模块
*******************************************************************************/
#include "ctrl_device_232.h"

CtrlDevice_232::CtrlDevice_232(QWidget *parent) : QWidget(parent)
{
//    initCom();
}

CtrlDevice_232::~CtrlDevice_232()
{
}
void CtrlDevice_232::send_IO_L(quint16 hex)
{
    quint8 crc = 0x00;
    QByteArray msg;
    QDataStream out(&msg, QIODevice::ReadWrite);
    out << quint8(0x7B) << quint8(0x00) << quint8(0xF2)
        << quint8(hex/256) << quint8(hex%256)
        << quint8(crc) << quint8(0x7D);
    out.device()->seek(1);
    out << quint8(msg.size());
    out.device()->seek(msg.size()-2);
    for (int i=1; i < msg.size()-2; i++)
        crc += quint8(msg.at(i));
    out << quint8(crc);
    com3->write(msg);
}

void CtrlDevice_232::send_IO_R(quint16 hex)
{
    quint8 crc = 0x00;
    QByteArray msg;
    QDataStream out(&msg, QIODevice::ReadWrite);
    out << quint8(0x7B) << quint8(0x00) << quint8(0xF2)
        << quint8(hex/256) << quint8(hex%256)
        << quint8(crc) << quint8(0x7D);
    out.device()->seek(1);
    out << quint8(msg.size());
    out.device()->seek(msg.size()-2);
    for (int i=1; i < msg.size()-2; i++)
        crc += quint8(msg.at(i));
    out << quint8(crc);
    com6->write(msg);
}

void CtrlDevice_232::pre_speed()
{
    quint32 timeOut = 0;
    QString temp = "%01#WCSR00010**";  //速度模式
    QByteArray cmd = temp.toUtf8();
    cmd.append(0x0D);
    com5->write(cmd);
    timeOut = 0;
    while (com5->bytesAvailable() < 7) {
        wait(10);
        timeOut++;
        if (timeOut > 100) {
            QMessageBox::warning(this, "速度模式", "PLC无回应", QMessageBox::Ok);
            return;
        }
    }
    qDebug() << com5->readAll();
//    QMessageBox::warning(this, "速度模式", com5->readAll(), QMessageBox::Ok);

    temp = "%01#WCSR00030**";  //方向0
    cmd = temp.toUtf8();
    cmd.append(0x0D);
    com5->write(cmd);
    timeOut = 0;
    while (com5->bytesAvailable() < 7) {
        wait(10);
        timeOut++;
        if (timeOut > 100) {
            QMessageBox::warning(this, "方向", "PLC无回应", QMessageBox::Ok);
            return;
        }
    }
    qDebug() << com5->readAll();
//    QMessageBox::warning(this, "方向", com5->readAll(), QMessageBox::Ok);

    temp = "%01#WDD001000010100000000**";  //设置速度
    cmd = temp.toUtf8();
    cmd.append(0x0D);
    com5->write(cmd);
    timeOut = 0;
    while (com5->bytesAvailable() < 7) {
        wait(10);
        timeOut++;
        if (timeOut > 100) {
            QMessageBox::warning(this, "设置速度", "PLC无回应", QMessageBox::Ok);
            return;
        }
    }
    qDebug() << com5->readAll();
//    QMessageBox::warning(this, "设置速度", com5->readAll(), QMessageBox::Ok);

    temp = "%01#WCSR00001**";  //启动
    cmd = temp.toUtf8();
    cmd.append(0x0D);
    com5->write(cmd);
    timeOut = 0;
    while (com5->bytesAvailable() < 7) {
        wait(10);
        timeOut++;
        if (timeOut > 100) {
            QMessageBox::warning(this, "启动", "PLC无回应", QMessageBox::Ok);
            return;
        }
    }
    qDebug() << com5->readAll();
//    QMessageBox::warning(this, "启动", com5->readAll(), QMessageBox::Ok);
}

void CtrlDevice_232::add_speed(quint16 spd)
{
    quint32 timeOut = 0;
    QByteArray cmd = "%01#WDD0010000101";
    QByteArray speed = QString("%1").arg(spd, 4, 16, QChar('0')).toUtf8();
    cmd.append(speed.toUpper());
    cmd.append("0000**");
    cmd = "%01#WDD0010000101F0000000**";  //设置速度
    cmd.append(0x0D);
    com5->write(cmd);
    timeOut = 0;
    while (com5->bytesAvailable() < 7) {
        wait(10);
        timeOut++;
        if (timeOut > 100) {
            QMessageBox::warning(this, "设置速度", "PLC无回应", QMessageBox::Ok);
            return;
        }
    }
    qDebug() << com5->readAll();
//    QMessageBox::warning(this, "设置速度", com5->readAll(), QMessageBox::Ok);
}

void CtrlDevice_232::end_speed()
{
    QByteArray cmd = "%01#WCSR00000**";
    cmd.append(0x0D);
    com5->write(cmd);
    wait(5);
    com5->readAll();
}

void CtrlDevice_232::readPlc()
{
    QByteArray cmd;
    quint32 timeOut = 0;
    cmd = QByteArray::fromHex("0005");
    com5->write(cmd);
    timeOut = 0;
    while (com5->bytesAvailable() < 7) {
        wait(10);
        timeOut++;
        if (timeOut > 100) {
            QMessageBox::warning(this, "", "伺服无回应", QMessageBox::Ok);
            return;
        }
    }
    qDebug() << com5->readAll();
    cmd = QByteArray::fromHex("0001926D00");
    com5->write(cmd);
    timeOut = 0;
    while (com5->bytesAvailable() < 7) {
        wait(10);
        timeOut++;
        if (timeOut > 100) {
            QMessageBox::warning(this, "", "伺服无回应", QMessageBox::Ok);
            return;
        }
    }
    qDebug() << com5->readAll();
    cmd = QByteArray::fromHex("0400");
    com5->write(cmd);
    timeOut = 0;
    while (com5->bytesAvailable() < 7) {
        wait(10);
        timeOut++;
        if (timeOut > 100) {
            QMessageBox::warning(this, "", "伺服无回应", QMessageBox::Ok);
            return;
        }
    }
    qDebug() << com5->readAll();
}

void CtrlDevice_232::initCom()
{
    com3 = new QSerialPort("COM3", this);
    if (com3->open(QIODevice::ReadWrite)) {
        com3->setBaudRate(9600);
        com3->setParity(QSerialPort::NoParity);
        com3->setDataBits(QSerialPort::Data8);
        com3->setStopBits(QSerialPort::OneStop);
        com3->setFlowControl(QSerialPort::NoFlowControl);
        com3->setDataTerminalReady(true);
        com3->setRequestToSend(false);
    } else {
        QMessageBox::warning(this, "", "COM3打开失败", QMessageBox::Ok);
    }
    com4 = new QSerialPort("COM4", this);
    if (com4->open(QIODevice::ReadWrite)) {
        com4->setBaudRate(9600);
        com4->setParity(QSerialPort::NoParity);
        com4->setDataBits(QSerialPort::Data8);
        com4->setStopBits(QSerialPort::OneStop);
        com4->setFlowControl(QSerialPort::NoFlowControl);
        com4->setDataTerminalReady(true);
        com4->setRequestToSend(false);
    } else {
        QMessageBox::warning(this, "", "COM4打开失败", QMessageBox::Ok);
    }
    com5 = new QSerialPort("COM5", this);
    if (com5->open(QIODevice::ReadWrite)) {
        com5->setBaudRate(9600);
        com5->setParity(QSerialPort::OddParity);
        com5->setDataBits(QSerialPort::Data8);
        com5->setStopBits(QSerialPort::OneStop);
        com5->setFlowControl(QSerialPort::NoFlowControl);
        com5->setDataTerminalReady(true);
        com5->setRequestToSend(false);
    } else {
        QMessageBox::warning(this, "", "COM5打开失败", QMessageBox::Ok);
    }
    com6 = new QSerialPort("COM6", this);
    if (com6->open(QIODevice::ReadWrite)) {
        com6->setBaudRate(9600);
        com6->setParity(QSerialPort::NoParity);
        com6->setDataBits(QSerialPort::Data8);
        com6->setStopBits(QSerialPort::OneStop);
        com6->setFlowControl(QSerialPort::NoFlowControl);
        com6->setDataTerminalReady(true);
        com6->setRequestToSend(false);
    } else {
        QMessageBox::warning(this, "", "COM6打开失败", QMessageBox::Ok);
    }
    com7 = new QSerialPort("COM7", this);
    if (com7->open(QIODevice::ReadWrite)) {
        com7->setBaudRate(9600);
        com7->setParity(QSerialPort::NoParity);
        com7->setDataBits(QSerialPort::Data8);
        com7->setStopBits(QSerialPort::OneStop);
        com7->setFlowControl(QSerialPort::NoFlowControl);
        com7->setDataTerminalReady(true);
        com7->setRequestToSend(false);
    } else {
        QMessageBox::warning(this, "", "COM7打开失败", QMessageBox::Ok);
    }
    com8 = new QSerialPort("COM8", this);
    if (com8->open(QIODevice::ReadWrite)) {
        com8->setBaudRate(9600);
        com8->setParity(QSerialPort::OddParity);
        com8->setDataBits(QSerialPort::Data8);
        com8->setStopBits(QSerialPort::OneStop);
        com8->setFlowControl(QSerialPort::NoFlowControl);
        com8->setDataTerminalReady(true);
        com8->setRequestToSend(false);
    } else {
        QMessageBox::warning(this, "", "COM8打开失败", QMessageBox::Ok);
    }
    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(readCom()));
    timer->start(50);
    hexL = 0x0000;
    hexR = 0x0000;
}

void CtrlDevice_232::wait(int ms)
{
    QElapsedTimer t;
    t.start();
    while (t.elapsed() < ms)
        QCoreApplication::processEvents();
}

void CtrlDevice_232::readCom()
{
    QByteArray msg;
    if (com3->bytesAvailable() > 7) {  //有数据读数据，无数据读状态
        msg = com3->readAll();
        hexL = quint16(msg.at(3)*256) + quint8(msg.at(4));
    } else {
        com3->write(QByteArray::fromHex("7B06F100F77D"));
    }
    if (com6->bytesAvailable() > 7) {  //有数据读数据，无数据读状态
        msg = com6->readAll();
        hexR = quint16(msg.at(3)*256) + quint8(msg.at(4));
    } else {
        com6->write(QByteArray::fromHex("7B06F100F77D"));
    }
    if ((hexL & X10) || (hexL & X11))
        QMessageBox::warning(this, "", "启动L", QMessageBox::Ok);
    if ((hexR & X10) || (hexR & X11))
        QMessageBox::warning(this, "", "启动R", QMessageBox::Ok);
    if ((hexL & X12) || (hexL & X13))
        QMessageBox::warning(this, "", "停止L", QMessageBox::Ok);
    if ((hexR & X12) || (hexR & X13))
        QMessageBox::warning(this, "", "停止R", QMessageBox::Ok);
}
