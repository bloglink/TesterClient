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

#include <QDebug>
#include <QObject>
#include <QtSerialPort/QSerialPort>

#define WT330_INIT 0x00
#define WT330_READ 0x01
#define WT330_WAIT 0x02

class WT330 : public QObject
{
    Q_OBJECT
public:
    explicit WT330(QObject *parent = 0);

signals:

public slots:
    bool initPort(QString portName);
    bool readThread(int waitTime);
    bool quitPort();
public:
    QByteArray power;
private:
    int timeOut;
    QSerialPort *com;
    quint8 wt330_mode;
};

#endif // WT330_H
