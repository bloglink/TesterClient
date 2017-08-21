/*******************************************************************************
 * Copyright [2017]   <青岛艾普智能仪器有限公司>
 * All rights reserved.
 *
 * version:     0.0.0.1
 * author:      zhaonanlin
 * brief:       功放板
*******************************************************************************/
#ifndef PAGEAMP_H
#define PAGEAMP_H

#include <QObject>
#include <QVariant>
#include <QDataStream>
#include <QElapsedTimer>
#include "define.h"

#define AMP_FREE        0
#define AMP_INIT        4
#define AMP_TEST        1

class PageAmp : public QObject
{
    Q_OBJECT
public:
    explicit PageAmp(QObject *parent = 0);

signals:
    void SendVariant(QVariant s);
    void SendCommand(quint16 addr, quint16 cmd, QByteArray data);
private slots:
    void ReadMessage(quint16 addr, quint16 cmd, QByteArray msg);
    void ExcuteCanCmd(QByteArray msg);
    void SendCanCmdStatus(void);
    void SendWarnning(QString s);
    void SendError(QString s);
    void ReadCanCmdStatus(QByteArray msg);
    bool WaitTimeOut(quint16 t);
    void Delay(int ms);
private:
    quint8 Mode;
    quint16 TimeOut;
};

#endif // PAGEAMP_H
