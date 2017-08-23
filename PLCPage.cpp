/*******************************************************************************
 * Copyright [2017]   <青岛艾普智能仪器有限公司>
 * All rights reserved.
 *
 * version:     0.0.0.1
 * author:      zhaonanlin
 * brief:       PLC控制模块
*******************************************************************************/
#include "PLCPage.h"

PLCPage::PLCPage(QWidget *parent) : QWidget(parent)
{
    initCom();
}

void PLCPage::sendIO(quint16 hex)
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

quint16 PLCPage::readIO()
{
    com3->write(QByteArray::fromHex("7B06F100F77D"));
    quint32 timeOut = 0;
    while (com3->bytesAvailable() < 7) {
        wait(10);
        timeOut++;
        if (timeOut > 100) {
            QMessageBox::warning(this, "","IO板无回应", QMessageBox::Ok);
            return 0;
        }
    }
    QByteArray msg = com3->readAll();
    quint16 hex = quint16(msg.at(3)*256) + quint8(msg.at(4));
    return hex;
}

void PLCPage::sendPlc(QString cmd)
{
    QByteArray cmds = cmd.toUtf8();
    cmds.append(0x0D);
    com4->write(cmds);
    quint32 timeOut = 0;
    while (com4->bytesAvailable() < 7) {
        wait(10);
        timeOut++;
        if (timeOut > 100) {
            QMessageBox::warning(this, "","PLC无回应", QMessageBox::Ok);
            return;
        }
    }
    qDebug() << com4->readAll();
}

void PLCPage::readPlc()
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
            QMessageBox::warning(this, "","伺服无回应", QMessageBox::Ok);
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
            QMessageBox::warning(this, "","伺服无回应", QMessageBox::Ok);
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
            QMessageBox::warning(this, "","伺服无回应", QMessageBox::Ok);
            return;
        }
    }
    qDebug() << com5->readAll();
}

void PLCPage::initCom()
{
    com3 = new QSerialPort("COM3", this);
    if (com3->open(QIODevice::ReadWrite)) {
        com3->setBaudRate(9600);     //波特率
        com3->setParity(QSerialPort::NoParity);
        com3->setDataBits(QSerialPort::Data8);
        com3->setStopBits(QSerialPort::OneStop);
        com3->setFlowControl(QSerialPort::NoFlowControl);
        com3->setDataTerminalReady(true);
        com3->setRequestToSend(false);
    }
    com4 = new QSerialPort("COM4", this);
    if (com4->open(QIODevice::ReadWrite)) {
        com4->setBaudRate(9600);     //波特率
        com4->setParity(QSerialPort::OddParity);
        com4->setDataBits(QSerialPort::Data8);
        com4->setStopBits(QSerialPort::OneStop);
        com4->setFlowControl(QSerialPort::NoFlowControl);
        com4->setDataTerminalReady(true);
        com4->setRequestToSend(false);
    }
    com5 = new QSerialPort("COM5", this);
    if (com5->open(QIODevice::ReadWrite)) {
        com5->setBaudRate(9600);     //波特率
        com5->setParity(QSerialPort::NoParity);
        com5->setDataBits(QSerialPort::Data8);
        com5->setStopBits(QSerialPort::OneStop);
        com5->setFlowControl(QSerialPort::NoFlowControl);
        com5->setDataTerminalReady(true);
        com5->setRequestToSend(false);
    }
    timer = new QTimer(this);
}

void PLCPage::wait(int ms)
{
    QElapsedTimer t;
    t.start();
    while (t.elapsed() < ms)
        QCoreApplication::processEvents();
}

