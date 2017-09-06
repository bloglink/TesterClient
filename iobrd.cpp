#include "iobrd.h"

IOBrd::IOBrd(QObject *parent) : QObject(parent)
{
    com = NULL;
    status = 0x0000;
}

bool IOBrd::initPort(QString portName)
{
    if (portName.isNull() && com != NULL) {
        com->close();
        return true;
    }
    com = new QSerialPort(portName, this);
    if (com->open(QIODevice::ReadWrite)) {
        com->setBaudRate(9600);
        com->setParity(QSerialPort::NoParity);
        com->setDataBits(QSerialPort::Data8);
        com->setStopBits(QSerialPort::OneStop);
        com->setFlowControl(QSerialPort::NoFlowControl);
        com->setDataTerminalReady(true);
        com->setRequestToSend(false);

        QTimer *timer = new QTimer(this);
        connect(timer, SIGNAL(timeout()), this, SLOT(readThread()));
        timer->start(50);
        return true;
    } else {
        return false;
    }
}

quint16 IOBrd::readPort()
{
    return status;
}

bool IOBrd::sendPort(quint16 hex)
{
    if (com == NULL || !com->isOpen())
        return false;
    quint8 crc = 0x00;
    QByteArray msg;
    QDataStream out(&msg, QIODevice::ReadWrite);
    out << quint8(0x7B) << quint8(0x00) << quint8(0xF2)
        << quint8(hex/256) << quint8(hex%256)
        << quint8(crc) << quint8(0x7D);
    out.device()->seek(1);
    out << quint8(msg.size());
    out.device()->seek(msg.size()-2);
    for (int i=1; i < msg.size()-2; i++)
        crc += quint8(msg.at(i));
    out << quint8(crc);
    com->write(msg);
    return true;
}

bool IOBrd::readThread()
{
    if (com == NULL || !com->isOpen())
        return false;
    if (com->bytesAvailable() > 7) {
        QByteArray msg = com->readAll();
        status = quint16(msg.at(3)*256) + quint8(msg.at(4));
        return true;
    }
    com->write(QByteArray::fromHex("7B06F100F77D"));  // 读取IO板状态
    return true;
}

