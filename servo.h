/*******************************************************************************
 * Copyright [2017]   <青岛艾普智能仪器有限公司>
 * All rights reserved.
 *
 * version:     0.1
 * author:      zhaonanlin
 * brief:       伺服控制模块
*******************************************************************************/
#ifndef SERVO_H
#define SERVO_H

#include <QTimer>
#include <QObject>
#include <QElapsedTimer>
#include <QCoreApplication>
#include <QtSerialPort/QSerialPort>

class Servo : public QObject
{
    Q_OBJECT
public:
    explicit Servo(QObject *parent = 0);
public slots:
    bool initPort(QString portName);
    quint16 readPort();
    bool readThread();
private slots:
    void wait(int ms);
private:
    QSerialPort *com;
    quint16 status;
    int timeOut;
    quint16 speed;
};

#endif // SERVO_H
