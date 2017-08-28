/*******************************************************************************
 * Copyright [2017]   <青岛艾普智能仪器有限公司>
 * All rights reserved.
 *
 * version:     0.1
 * author:      zhaonanlin
 * brief:       232控制模块
*******************************************************************************/
#ifndef CTRLDEVICE_232_H
#define CTRLDEVICE_232_H

#include <QTimer>
#include <QObject>
#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>
#include <QTextBrowser>
#include <QDataStream>
#include <QDebug>
#include <QElapsedTimer>
#include <QCoreApplication>
#include <QMessageBox>
#include <QFile>

#define X01_TARGET 0x0100
#define X01_ORIGIN 0x0200
#define X02_TARGET 0x0400
#define X02_ORIGIN 0x0800
#define X03_TARGET 0x1000
#define X03_ORIGIN 0x2000
#define X04_TARGET 0x4000
#define X04_ORIGIN 0x8000

#define X10 0x0001
#define X11 0x0002
#define X12 0x0004
#define X13 0x0008

#define Y00 0x0001
#define Y01 0x0002
#define Y02 0x0004
#define Y03 0x0008
#define Y08 0x0100
#define Y09 0x0200
#define Y10 0x0400
#define Y11 0x0800

class CtrlDevice_232 : public QWidget
{
    Q_OBJECT
public:
    explicit CtrlDevice_232(QWidget *parent = 0);
    ~CtrlDevice_232();
public slots:
    void send_IO_L(quint16 hex);
    void send_IO_R(quint16 hex);
    void pre_speed();
    void add_speed(quint16 spd);
    void end_speed();
    void readPlc();
public:
    quint16 hexL;
    quint16 hexR;
private slots:
    void initCom();
    void wait(int ms);
    void readCom();
private:
    QSerialPort *com3;
    QSerialPort *com4;
    QSerialPort *com5;
    QSerialPort *com6;
    QSerialPort *com7;
    QSerialPort *com8;
    QTimer *timer;

};

#endif // CTRLDEVICE_232_H
