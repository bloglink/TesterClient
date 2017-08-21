/*******************************************************************************
 * Copyright [2016]   <青岛艾普智能仪器有限公司>
 * All rights reserved.
 *
 * version:     2.1.0.170427
 * author:      zhaonanlin
 * brief:       CAN模块
*******************************************************************************/
#include "CanClient.h"

CanClient::CanClient(QObject *parent) : QObject(parent)
{
    s = 0;
    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(readAll()));
}

void CanClient::DeviceOpen()
{
#ifdef __arm__
    char str[256];

    struct can_filter rfilter[14];      //  rfilter 记录总数
    rfilter[0].can_id = 0x41;           // 电阻板ID
    rfilter[0].can_mask = CAN_SFF_MASK;
    rfilter[1].can_id = 0x441;          // 反嵌板波形ID
    rfilter[1].can_mask = CAN_SFF_MASK;
    rfilter[2].can_id = 0x61;           // 绝缘板ID
    rfilter[2].can_mask = CAN_SFF_MASK;
    rfilter[3].can_id = 0x261;          // 输出板ID  13
    rfilter[3].can_mask = CAN_SFF_MASK;
    rfilter[4].can_id = 0x81;           // 匝间板ID
    rfilter[4].can_mask = CAN_SFF_MASK;
    rfilter[5].can_id = 0x481;          // 匝间板波形ID
    rfilter[5].can_mask = CAN_SFF_MASK;

    rfilter[6].can_id = 0x281;          // 输出板ID  14
    rfilter[6].can_mask = CAN_SFF_MASK;
    rfilter[7].can_id = 0x2A1;          // 输出板ID  15
    rfilter[7].can_mask = CAN_SFF_MASK;
    rfilter[8].can_id = 0x2C1;          // 输出板ID  16
    rfilter[8].can_mask = CAN_SFF_MASK;
    rfilter[9].can_id = 0x2E1;          // 输出板ID  17
    rfilter[9].can_mask = CAN_SFF_MASK;

    rfilter[10].can_id = 0xC1;           // 电感板ID
    rfilter[10].can_mask = CAN_SFF_MASK;

    rfilter[11].can_id = 0xE1;           // 电参板ID 1   27
    rfilter[11].can_mask = CAN_SFF_MASK;

    rfilter[12].can_id = 0x101;          // 电参板ID 2   28
    rfilter[12].can_mask = CAN_SFF_MASK;

    rfilter[13].can_id = 0x4E1;          // 电参板 PG波形
    rfilter[13].can_mask = CAN_SFF_MASK;
    struct sockaddr_can     addr;
    struct ifreq            ifr;
    s = socket(PF_CAN,  SOCK_RAW,  CAN_RAW);  /*打开套接字*/
    if (s < 0)
        return;
    strcpy(ifr.ifr_name, "can0");
    if (ioctl(s,  SIOCGIFINDEX,  &ifr) < 0)
        return;

    addr.can_family = AF_CAN;
    addr.can_ifindex = ifr.ifr_ifindex;
    if (bind(s,  (struct sockaddr *)&addr,  sizeof(addr)) < 0)
        return;
#else
    s = CAN_DeviceOpen((DWORD)ACUSB_132B, (DWORD)0, NULL);
    if (s <= 0)
        return;
    tagCAN_InitConfig InitMsg;
    InitMsg.dwAccCode = 0;
    InitMsg.dwAccMask = 0xffffffff;
    InitMsg.nFilter = 0;
    InitMsg.bMode = 0;
    InitMsg.nBtrType = 1;
    InitMsg.dwBtr[0] = 0x00;
    InitMsg.dwBtr[1] = 0x1C;
    InitMsg.dwBtr[2] = 0;
    InitMsg.dwBtr[3] = 0;
    int ret = CAN_ChannelStart(s, 0, &InitMsg);
    if (ret <= 0)
        return;
#endif
    timer->start(50);
}

void CanClient::DeviceQuit()
{
    if (s <= 0)
        return;
    timer->stop();
#ifdef __arm__
    close(s);
#else
    CAN_ChannelStop(s, 0);
    CAN_DeviceClose(s);
#endif
}

bool CanClient::DeviceSend()
{
    if (s <= 0)
        return false;
#ifdef __arm__
    int nbytes;
    nbytes = write(s,  &TxMsg,  sizeof(struct can_frame));
    if (nbytes != sizeof(struct can_frame))
        return false;
    else
        return true;
#else
    if (CAN_ChannelSend(s, 0, &TxMsg, 1) > 0)
        return true;
    else
        return false;
#endif
    return true;
}

bool CanClient::DeviceRead()
{
    if (s <= 0)
        return false;
#ifdef __arm__
    int ret;
    fd_set rdfds;
    struct timeval tv;

    FD_ZERO(&rdfds);
    FD_SET(s,  &rdfds);
    tv.tv_sec  = 0;
    tv.tv_usec = 0;
    ret = select(s+1,  &rdfds,  NULL,  NULL,  &tv);
    if (ret <= 0) {
        return false;
    }
    if (FD_ISSET(s,  &rdfds)) {
        if (read(s,  &TxMsg,  sizeof(struct can_frame)) != sizeof(struct can_frame))
            return false;
        else
            return true;
    }
#else
    if (CAN_GetReceiveCount(s, 0) <= 0)
        return false;
    if (CAN_ChannelReceive(s, 0, &TxMsg, 1, 1) == 1)
        return true;
#endif
    return false;
}

void CanClient::readAll()
{
    QByteArray msg;
    QDataStream in(&msg,  QIODevice::ReadWrite);
    in.setVersion(QDataStream::Qt_4_8);
#ifdef __arm__
    while (DeviceRead()) {
        in << quint16(TxMsg.can_id) << quint8(TxMsg.can_dlc);
        for (int i=0; i < TxMsg.can_dlc; i++)
            in << TxMsg.data[i];
    }
#else
    while (DeviceRead()) {
        in << quint16(TxMsg.uID) << quint8(TxMsg.nDataLen);
        for (int i=0; i < TxMsg.nDataLen; i++)
            in << TxMsg.arryData[i];
    }
#endif
    if (!msg.isEmpty())
        emit GetCanData(msg);
}

void CanClient::WriteAll(QByteArray msg)
{
    quint16 id;
    quint8 dlc;
    quint8 dat;
    QDataStream out(&msg,  QIODevice::ReadWrite);
    out.setVersion(QDataStream::Qt_4_8);
    while (!out.atEnd()) {
        out >> id >> dlc;
        if (id > 0x800 || dlc > 8)
            break;
#ifdef __arm__
        TxMsg.can_id = id;
        TxMsg.can_dlc = dlc;
        for (int i=0; i < dlc; i++) {
            out >> dat;
            TxMsg.data[i] = dat;
        }
#else
        TxMsg.uID = id;
        TxMsg.nDataLen = dlc;
        TxMsg.bExternFlag = 0;
        TxMsg.bRemoteFlag = 0;
        TxMsg.nSendType = 0;
        for (int i=0; i < dlc; i++) {
            out >> dat;
            TxMsg.arryData[i] = dat;
        }
#endif
        //        if (!DeviceSend()) {
        //            DeviceQuit();
        //            DeviceOpen();
        //            break;
        //        }
        DeviceSend();
    }
}
