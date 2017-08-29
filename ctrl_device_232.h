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
#define X03_TARGET 0x2000
#define X03_ORIGIN 0x1000
#define X04_TARGET 0x4000
#define X04_ORIGIN 0x8000

#define X10 0x0001  // 启动
#define X11 0x0002  // 启动
#define X12 0x0004  // 停止
#define X13 0x0008  // 停止

#define Y00 0x0001  // 台下起降气缸1
#define Y01 0x0002  // 台下夹紧气缸2
#define Y02 0x0004  // 台上压紧气缸3
#define Y03 0x0008  // 台上耐压气缸4
#define Y08 0x0100  // 蜂鸣器
#define Y09 0x0200  // 红灯
#define Y10 0x0400  // 黄灯
#define Y11 0x0800  // 绿灯

class CtrlDevice_232 : public QWidget
{
    Q_OBJECT
public:
    explicit CtrlDevice_232(QWidget *parent = 0);
    ~CtrlDevice_232();
signals:
    void recvMsg(QString msg);
public slots:
    void send_IO(quint16 station, quint16 hex);
    void pre_speed();
    void add_speed(quint16 spd);
    void end_speed();
    void readPlc();
public:
    quint16 hexL;
    quint16 hexR;
    quint16 speed;
    quint16 torque;
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
