/*******************************************************************************
 * Copyright [2017]   <青岛艾普智能仪器有限公司>
 * All rights reserved.
 *
 * version:     0.0.0.1
 * author:      zhaonanlin
 * brief:       功放板
*******************************************************************************/
#include "PageAmp.h"

PageAmp::PageAmp(QObject *parent) : QObject(parent)
{
    Mode = AMP_FREE;
}

void PageAmp::ReadMessage(quint16 addr, quint16 cmd, QByteArray msg)
{
    if (addr != ADDR && addr != WIN_ID_AMP && addr != CAN_ID_AMP)
        return;
    switch (cmd) {
    case CMD_CAN:
        ExcuteCanCmd(msg);
        break;
    case CMD_CHECK:
        Mode = AMP_INIT;
        SendCanCmdStatus();
        WaitTimeOut(30);
        Mode = AMP_FREE;
        break;
    default:
        break;
    }
}

void PageAmp::ExcuteCanCmd(QByteArray msg)
{
    if (Mode == AMP_FREE)
        return;
    TimeOut = 0;

    if (msg.size() >= 8 && (quint8)msg.at(0) == 0x00) {
        ReadCanCmdStatus(msg);
    }
}

void PageAmp::SendCanCmdStatus()
{
    QByteArray msg;
    QDataStream out(&msg,  QIODevice::ReadWrite);
    out.setVersion(QDataStream::Qt_4_8);
    out << quint16(0x2A) << quint8(0x01) << quint8(0x00);
    emit SendCommand(ADDR, CMD_CAN, msg);
}

void PageAmp::SendWarnning(QString s)
{
    QVariantHash hash;
    hash.insert("TxAddress", "WinHome");
    hash.insert("TxCommand", "Warnning");
    hash.insert("TxMessage", tr("功放异常:\n%1").arg(s));
    emit SendVariant(QVariant::fromValue(hash));
}

void PageAmp::SendError(QString s)
{
    QVariantHash hash;
    hash.insert("TxAddress", "WinHome");
    hash.insert("TxCommand", "Error");
    hash.insert("TxMessage", tr("功放异常:\n%1").arg(s));
    emit SendVariant(QVariant::fromValue(hash));
}

void PageAmp::ReadCanCmdStatus(QByteArray msg)
{
    int s = quint8(msg.at(1));
    switch (s) {
    case 0x00:
        break;
    case 0x01:
        return;
    default:
        SendWarnning(tr("OTHER_ERROR %1").arg(s));
        break;
    }
    int e = quint8(msg.at(2));
    switch (e) {
    case 0x00:
        break;
    case 0x01:
        SendError(tr("未接地"));
        break;
    case 0x02:
        SendWarnning("L/N reversed");
        break;
    default:
        SendWarnning(tr("PWR_ERROR %1").arg(e));
        break;
    }
    Mode = AMP_FREE;
}

bool PageAmp::WaitTimeOut(quint16 t)
{
    TimeOut = 0;
    while (Mode != AMP_FREE) {
        Delay(10);
        TimeOut++;
        if (TimeOut > t)
            return false;
    }
    return true;
}

void PageAmp::Delay(int ms)
{
    QElapsedTimer t;
    t.start();
    while (t.elapsed() < ms)
        QCoreApplication::processEvents();
}
