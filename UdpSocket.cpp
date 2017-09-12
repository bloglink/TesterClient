/*******************************************************************************
 * Copyright [2017]   <青岛艾普智能仪器有限公司>
 * All rights reserved.
 *
 * version:     0.0.0.1
 * author:      zhaonanlin
 * brief:       UDP处理模块
*******************************************************************************/
#include "UdpSocket.h"

UdpSocket::UdpSocket(QObject *parent) : QUdpSocket(parent)
{
    timeCount = 0;
}

void UdpSocket::initSocket(QString host, quint16 port)
{
    txPort = port;
    txHost.setAddress(host);

    this->bind(QHostAddress::AnyIPv4, port);
    connect(this, SIGNAL(readyRead()), this, SLOT(recvNetMsg()));

    QTimer *t_send = new QTimer(this);  // 定时处理消息队列
    connect(t_send, SIGNAL(timeout()), this, SLOT(transmitJson()));
    t_send->start(5);
}

void UdpSocket::quitSocket()
{
    this->close();
}

void UdpSocket::recvNetMsg()
{
    while (this->hasPendingDatagrams()) {
        QByteArray msg;
        msg.resize(this->pendingDatagramSize());
        QHostAddress sender;
        quint16 senderPort;
        this->readDatagram(msg.data(), msg.size(), &sender, &senderPort);
        recv_queue.enqueue(msg);
        timeCount = 0;
        qDebug() << "recv" << msg;
    }
}

void UdpSocket::recvAppJson(QJsonObject obj)
{
    send_queue.enqueue(obj.value("TxMessage").toString());
}

void UdpSocket::recvAppMsg(QByteArray msg)
{
    send_queue.enqueue(msg);
}

void UdpSocket::transmitJson()
{
    if (!send_queue.isEmpty()) {
        QByteArray msg = send_queue.dequeue().toUtf8();
        this->writeDatagram(msg, txHost, txPort);
        this->waitForBytesWritten();
        qDebug() << "send" << msg;
    }
    if (!recv_queue.isEmpty())
        emit sendNetMsg(recv_queue.dequeue());
    timeCount++;
    if (timeCount%200 == 0) {
        this->writeDatagram("6030", txHost, txPort);
        if (timeCount > 1000) {
            timeCount = 0;
            emit sendNetMsg("9032");
        }
    }
}

/*********************************END OF FILE**********************************/
