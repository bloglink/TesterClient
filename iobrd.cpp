#include "iobrd.h"

IOBrd::IOBrd(QObject *parent) : QObject(parent)
{
    com = NULL;
    isQuit = false;
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

bool IOBrd::waitPort(quint16 hex)
{
    quint16 s = 0x0000;
    if (hex & Y00)
        s |= X01_TARGET;
    else
        s |= X01_ORIGIN;
    if (hex & Y01)
        s |= X02_TARGET;
    else
        s |= X02_ORIGIN;
    if (hex & Y02)
        s |= X03_TARGET;
    else
        s |= X03_ORIGIN;
    if (hex & Y03)
        s |= X04_TARGET;
    else
        s |= X04_ORIGIN;

    qDebug() << "recv" << QString::number(s, 16);

    quint16 timeOut = 0x0000;
    quint16 count = 0;
    while (1) {
        if ((status & 0xFF00) == s) {
            count++;
            if (count > 5)
                return true;
        }
        wait(10);
        timeOut++;
        if (timeOut > 500) {
            QMessageBox::warning(NULL, "气缸", "气缸超时", QMessageBox::Ok);
            return false;
        }
        if (isQuit) {
            isQuit = false;
            return false;
        }
    }
}

void IOBrd::quitPort(bool s)
{
    isQuit = s;
}

bool IOBrd::readThread()
{
    if (com == NULL || !com->isOpen())
        return false;
    if (com->bytesAvailable() > 7) {
        QByteArray msg = com->readAll();
        status = quint16(msg.at(3)*256) + quint8(msg.at(4));
        if (status & X10)
            emit sendStart(1);
        if (status & X12)
            emit sendStart(0);
        return true;
    }
    com->write(QByteArray::fromHex("7B06F100F77D"));  // 读取IO板状态
    return true;
}

bool IOBrd::wait(int ms)
{
    QElapsedTimer t;
    t.start();
    while (t.elapsed() < ms)
        QCoreApplication::processEvents();
    return true;
}

