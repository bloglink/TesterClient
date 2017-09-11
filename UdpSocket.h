/*******************************************************************************
 * Copyright [2017]   <青岛艾普智能仪器有限公司>
 * All rights reserved.
 *
 * version:     0.0.0.1
 * author:      zhaonanlin
 * brief:       UDP处理模块
*******************************************************************************/
#ifndef SocketUdp_H
#define SocketUdp_H

#include <QUrl>
#include <QUuid>
#include <QTimer>
#include <QQueue>
#include <QObject>
#include <QUdpSocket>
#include <QElapsedTimer>
#include <QApplication>
#include <QNetworkInterface>
#include <QJsonObject>
#include <QJsonDocument>


#define LOCAL_PORT 10000
#define VERSION "erp-0.0.0.1"

class UdpSocket : public QUdpSocket
{
    Q_OBJECT
public:
    explicit UdpSocket(QObject *parent = 0);

signals:
    void sendNetMsg(QString msg);
public:
    void initSocket(QString host, quint16 port);
    void quitSocket(void);

private slots:
    void recvNetMsg(void);
    void recvAppJson(QJsonObject obj);
    void recvAppMsg(QByteArray msg);
    void transmitJson();
private:
    quint16 txPort;
    QHostAddress txHost;
    QQueue <QString> send_queue;
    QQueue <QString> recv_queue;
    quint32 timeCount;
};

#endif // SocketUdp_H
