/*******************************************************************************
 * Copyright [2017]   <青岛艾普智能仪器有限公司>
 * All rights reserved.
 *
 * version:     0.1
 * author:      zhaonanlin
 * brief:       横河电参表读取程序
*******************************************************************************/
#ifndef WT330_H
#define WT330_H

#include <QTimer>
#include <QDebug>
#include <QObject>
#include <QElapsedTimer>
#include <QCoreApplication>
#include <QtSerialPort/QSerialPort>

class WT330 : public QObject
{
    Q_OBJECT
public:
    explicit WT330(QObject *parent = 0);

public slots:
    bool initPort(QString portName);
    QStringList readMeter();
private slots:
    bool setNumber();
    bool readThread();
    void wait(int ms);
private:
    QSerialPort *com;
    QStringList curr;
    QByteArray tmp;
    QTimer *timer;
};

#endif // WT330_H
