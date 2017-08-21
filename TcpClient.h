/*******************************************************************************
 * Copyright [2016]   <青岛艾普智能仪器有限公司>
 * All rights reserved.
 *
 * version:     2.1.0.170427
 * author:      zhaonanlin
 * brief:       TCP处理模块
*******************************************************************************/
#ifndef TCPCLIENT_H
#define TCPCLIENT_H

#include <QDir>
#include <QFile>
#include <QTime>
#include <QTimer>
#include <QObject>
#include <QProcess>
#include <QSettings>
#include <QHostInfo>
#include <QTcpSocket>
#include <QDataStream>
#include <QApplication>
#include <QNetworkInterface>
#include <QCryptographicHash>

#include "define.h"

#define NET "./network/"
#define TMP "./temp/"
#define CON "./config/"

class TcpClient : public QTcpSocket
{
    Q_OBJECT
public:
    explicit TcpClient(QObject *parent = 0);

signals:
    void SendCommand(quint16 addr, quint16 cmd, QByteArray msg);
private slots:
    void TcpInit(void);
    void TcpQuit(void);
    void Connected(void);
    void KeepAlive(void);
    void GetBlock(void);
    void GetFileHead(quint16 addr, QByteArray msg);
    void GetFileData(quint16 addr, QByteArray msg);
    void PutFileData(qint64 numBytes);
    void PutBlock(quint16 addr, quint16 proc, QByteArray data);
    void PutFileHead(QByteArray data);
    void ExcuteCmd(quint16 addr, quint16 proc, QByteArray data);
    void ShellCmd(quint16 addr, QByteArray msg);
    QString getHardwareAddress(void);
    void Error(QAbstractSocket::SocketError);
    void Display(QByteArray msg);
private:
    QTimer *timer;
    QString InitString;

    QFile *file;
    QString fileName;
    QString fileDetail;

    quint16 TramsmitAddr;

    qint64 count;
    qint64 LoadSize;
    qint64 BlockSize;
    qint64 BytesRead;
    qint64 BytesToRead;
    qint64 BytesToWrite;
    qint64 BytesWritten;
    QByteArray SendVerify;
    QByteArray RecvVerify;
};

#endif // TCPCLIENT_H
