/*******************************************************************************
 * Copyright [2017]   <青岛艾普智能仪器有限公司>
 * All rights reserved.
 *
 * version:     0.1
 * author:      zhaonanlin
 * brief:       MBDKT2510 PLC控制模块
*******************************************************************************/
#ifndef MBDKT_H
#define MBDKT_H

#include <QDebug>
#include <QObject>
#include <QElapsedTimer>
#include <QCoreApplication>
#include <QtSerialPort/QSerialPort>

class MBDKT : public QObject
{
    Q_OBJECT
public:
    explicit MBDKT(QObject *parent = 0);

public slots:
    bool initPort(QString portName);
    bool setMode(int mode);
    bool setTurn(int turn);
    bool setStart(int start);
    bool setSpeed(int speed);
    bool setTorque(int torque);
private slots:
    void wait(int ms);
    bool sendCommand(QByteArray cmd);
private:
    QSerialPort *com;
};

#endif // MBDKT_H
