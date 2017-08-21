/*******************************************************************************
 * Copyright [2016]   <青岛艾普智能仪器有限公司>
 * All rights reserved.
 *
 * version:     2.1.0.170427
 * author:      zhaonanlin
 * brief:       UDP处理模块
*******************************************************************************/
#ifndef UDPCLIENT_H
#define UDPCLIENT_H

#include <QDir>
#include <QFile>
#include <QTime>
#include <QObject>
#include <QSettings>
#include <QUdpSocket>
#include <QStringList>

#include "define.h"

class UdpClient : public QUdpSocket
{
    Q_OBJECT
public:
    explicit UdpClient(QObject *parent = 0);

signals:
    void SendCommand(quint16 addr, quint16 cmd, QByteArray msg);

private slots:
    void Init(void);
    void Quit(void);
    void InitSettings(void);
    void SaveSettings(void);
    void ReadAll(void);
    void ReadMessage(quint16 addr,  quint16 cmd,  QByteArray msg);
private:
    QString Number;
    QString Types;
    quint16 Command;
    QByteArray Param;
    QString FileInUse;
    QString TxMsg;
    QStringList Items;
    QString Status;
};

#endif // UDPCLIENT_H
