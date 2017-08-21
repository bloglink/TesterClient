/*******************************************************************************
 * Copyright [2016]   <  青岛艾普智能仪器有限公司>
 * All rights reserved.
 *
 * version:     2.1.0.170418
 * author:      zhaonanlin
 * brief:       串口通信模块
*******************************************************************************/
#ifndef SERIALPORT_H
#define SERIALPORT_H

#ifdef __arm__
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <unistd.h>
#endif

#include <QTime>
#include <QTimer>
#include <QDebug>
#include <QObject>
#include <QStringList>
#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>
#include "define.h"

#define PWM_POLARITY_NORMAL (0 << 0)
#define PWM_POLARITY_INVERTED (1 << 0)
#define POLARITY PWM_POLARITY_INVERTED;

struct pwm_config_info {
    unsigned int freq; /* in Hz */
    unsigned int duty; /* in % */
    unsigned int polarity;
    unsigned int count;
};

class SerialPort : public QObject
{
    Q_OBJECT
public:
    explicit SerialPort(QObject *parent = 0);

signals:
    void SendCommand(quint16 addr, quint16 cmd, QByteArray msg);
public slots:
    void OpenSerial();
private slots:
    void CloseSerial(void);
    void ReadSerial(void);
    void ReadMessage(quint16 addr, quint16 cmd, QByteArray msg);
    void SendAlarm(QByteArray msg);
    void StartBeep(void);
    void StopBeep(void);
private:
    QSerialPort *com;
    QTimer *timer;
    int fd;
    quint8 BeepMode;
};

#endif // SERIALPORT_H
