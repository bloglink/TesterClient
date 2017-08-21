/*******************************************************************************
 * Copyright [2016]   <青岛艾普智能仪器有限公司>
 * All rights reserved.
 *
 * version:     2.1.0.170427
 * author:      zhaonanlin
 * brief:       输出模块
*******************************************************************************/
#include "PageOut.h"
#include "ui_PageOut.h"

PageOut::PageOut(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::PageOut)
{
    ui->setupUi(this);

    Timer = new QTimer(this);
    connect(Timer, SIGNAL(timeout()), this, SLOT(SendWinCmdStart()));

    Mode = OUT_FREE;
    StartMode = 2;
}

PageOut::~PageOut()
{
    delete ui;
}

void PageOut::ReadMessage(quint16 addr, quint16 cmd, QByteArray data)
{
    if (addr != ADDR && addr != WIN_ID_OUT13 && addr != CAN_ID_13OUT &&
            addr != CAN_ID_14OUT && addr != CAN_ID_15OUT)
        return;
    switch (cmd) {
    case CMD_CAN:
        ExcuteCanCmd(addr, data);
        break;
    case CMD_CHECK:
        Mode = OUT_INIT;
        SendCanCmdStatus(data.toInt());
        Mode = OUT_FREE;
        break;
    case CMD_INIT:
        StartMode = data.toInt();
        SendCanCmdConfig();
        break;
    case CMD_ALARM:
        SendAlarm(data);
        break;
    default:
        break;
    }
}

void PageOut::ExcuteCanCmd(quint16 id, QByteArray msg)
{
    if (quint8(msg.at(0)) == 0x00)
        ReadCanCmdStatus(id, msg);

    if (quint8(msg.at(0)) == 0x01 && quint8(msg.at(1) == 0x01)) {
        ReadCanCmdStart(id);
    }
    if (quint8(msg.at(0)) == 0x01 && quint8(msg.at(1) == 0x00)) {
        ReadCanCmdStop(id);
    }
}

void PageOut::SendCanCmdStatus(quint16 pos)
{
    QByteArray msg;
    QDataStream out(&msg, QIODevice::ReadWrite);
    out.setVersion(QDataStream::Qt_4_8);
    QString w;
    switch (pos) {
    case 0:
        out << quint16(0x13) << quint8(0x01) << quint8(0x00);
        w = tr("左工位超时");
        break;
    case 1:
        out << quint16(0x14) << quint8(0x01) << quint8(0x00);
        w = tr("右工位超时");
        break;
    default:
        break;
    }
    emit SendCommand(ADDR, CMD_CAN, msg);
    if (!WaitTestOver(30)) {
        SendWarnning(w);
    }
}

void PageOut::SendWinCmdStart()
{
    QStringList msg;
    msg.append(QString::number(Pos));
    msg.append(QString::number(StartMode));
    Timer->stop();
    emit SendCommand(ADDR, CMD_START, msg.join(" ").toUtf8());
}

void PageOut::SendCanCmdConfig()
{
    QByteArray msg;
    QDataStream out(&msg, QIODevice::ReadWrite);
    out.setVersion(QDataStream::Qt_4_8);
    out << quint16(0x13) << quint8(0x02) << quint8(0x03) << quint8(StartMode);
    out << quint16(0x14) << quint8(0x02) << quint8(0x03) << quint8(StartMode);
    emit SendCommand(ADDR, CMD_CAN, msg);
}

void PageOut::ReadCanCmdStatus(quint16 addr, QByteArray msg)
{
    QString t;
    if (addr == WIN_ID_OUT13)
        t = tr("左工位");
    if (addr == WIN_ID_OUT14)
        t = tr("右工位");
    int s = quint8(msg.at(1));
    switch (s) {
    case 0x00:
        break;
    case 0x01:
        SendWarnning(t + " UNIVALID");
        return;
    default:
        SendWarnning(t + " UNKONW_ERROR");
        break;
    }
    Mode = OUT_FREE;
}

void PageOut::ReadCanCmdStart(quint16 addr)
{
    if (Mode != OUT_FREE)
        return;
    if (addr == CAN_ID_13OUT) {
        Pos = WIN_ID_OUT13;
    }
    if (addr == CAN_ID_14OUT) {
        Pos = WIN_ID_OUT14;
    }
    Mode = OUT_FREE;
    Timer->start(10);
}

void PageOut::ReadCanCmdStop(quint16 addr)
{
    if (addr == CAN_ID_13OUT && Pos == 0x13) {
        Mode = OUT_FREE;
        emit SendCommand(ADDR, CMD_STOP, NULL);
    }
    if (addr == CAN_ID_14OUT && Pos == 0x14) {
        Mode = OUT_FREE;
        emit SendCommand(ADDR, CMD_STOP, NULL);
    }
}

void PageOut::SendAlarm(QByteArray addr)
{
    quint8 t = 0x00;
    if (addr.at(0) & 0x02)
        t = 0x01;
    if (addr.at(0) & 0x04)
        t = 0x02;
    if (addr.at(0) & 0x08)
        t = 0x03;
    QByteArray msg;
    QDataStream out(&msg, QIODevice::ReadWrite);
    out.setVersion(QDataStream::Qt_4_8);
    out << quint16(0x13) << quint8(0x02) << quint8(0x02) << quint8(t);
    out << quint16(0x14) << quint8(0x02) << quint8(0x02) << quint8(t);
    emit SendCommand(ADDR, CMD_CAN, msg);
}

bool PageOut::WaitTestOver(quint16 t)
{
    TimeOut = 0;
    while (Mode != OUT_FREE) {
        Delay(10);
        TimeOut++;
        if (TimeOut > t)
            return false;
    }
    return true;
}

void PageOut::Delay(int ms)
{
    QElapsedTimer t;
    t.start();
    while (t.elapsed() < ms)
        QCoreApplication::processEvents();
}

void PageOut::SendWarnning(QString s)
{
    QVariantHash hash;
    hash.insert("TxAddress", "WinHome");
    hash.insert("TxCommand", "Warnning");
    hash.insert("TxMessage", tr("输出异常:\n%1").arg(s));
    emit SendVariant(QVariant::fromValue(hash));
}
/*********************************END OF FILE**********************************/
