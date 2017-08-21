/*******************************************************************************
 * Copyright [2016]   <  青岛艾普智能仪器有限公司>
 * All rights reserved.
 *
 * version:     2.1.0.170418
 * author:      zhaonanlin
 * brief:       串口通信模块
*******************************************************************************/
#include "SerialPort.h"

SerialPort::SerialPort(QObject *parent) : QObject(parent)
{
    fd = -1;
    com = NULL;
    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(ReadSerial()));
}

void SerialPort::OpenSerial()
{
    if (com  ==  NULL)
        com = new QSerialPort("/dev/ttyS3", this);
    if (com->isOpen())
        com->close();
    if (com->open(QIODevice::ReadWrite)) {
        com->setBaudRate(115200);     //波特率
        com->setParity(QSerialPort::NoParity);
        com->setDataBits(QSerialPort::Data8);
        com->setStopBits(QSerialPort::OneStop);
        com->setFlowControl(QSerialPort::NoFlowControl);
        com->setDataTerminalReady(true);
        com->setRequestToSend(false);
        timer->start(50);
    }
#ifdef __arm__
    fd = open("/dev/em335x_pwm2", O_RDWR);
    if (fd < 0) {
        qDebug() << "pwm2 open fail";
        return;
    }
#endif
}

void SerialPort::CloseSerial()
{
    timer->stop();
    if (com != NULL && com->isOpen())
        com->close();
}

void SerialPort::ReadSerial()
{
    QByteArray cmd = com->readAll();
    if (cmd.isEmpty())
        return;
    QStringList msg;
    msg.append(QString::number(0x13));
    msg.append(QString::number(0x00));
    if (cmd.size() == 4 && quint8(cmd.at(1) == 0x32))
        SendCommand(ADDR, CMD_START, msg.join(" ").toUtf8());
    if (cmd.size() == 4 && quint8(cmd.at(1) == 0x31)) {
        SendCommand(ADDR, CMD_STOP, msg.join(" ").toUtf8());
        com->write("LED1");
    }
}

void SerialPort::ReadMessage(quint16 addr, quint16 cmd, QByteArray msg)
{
    if (addr != ADDR)
        return;
    switch (cmd) {
    case CMD_ALARM:
        SendAlarm(msg);
        break;
    }
}

void SerialPort::SendAlarm(QByteArray msg)
{
    if (!com->isOpen() || msg.isEmpty())
        return;
    if (msg.at(0) & 0x02) {
        com->write("LEDY");
        com->write("LEDY");
        com->write("LEDY");
    }
    if (msg.at(0) & 0x04) {
        com->write("LEDG");
        com->write("LEDG");
        com->write("LEDG");
    }
    if (msg.at(0) & 0x08) {
        com->write("LEDR");
        com->write("LEDR");
        com->write("LEDR");
    }
    if (msg.at(0) & 0x01)
        StartBeep();
    else
        StopBeep();
}

void SerialPort::StartBeep()
{
#ifdef __arm__
    struct pwm_config_info conf;
    conf.freq = 2000;
    conf.duty = 100;
    conf.polarity = POLARITY;
    conf.count = 0;
    write(fd, &conf, sizeof(struct pwm_config_info));
#endif
}

void SerialPort::StopBeep()
{
#ifdef __arm__
    struct pwm_config_info conf;
    memset(&conf, 0, sizeof(struct pwm_config_info));
    write(fd, &conf, sizeof(struct pwm_config_info));
#endif
}

