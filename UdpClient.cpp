/*******************************************************************************
 * Copyright [2016]   <青岛艾普智能仪器有限公司>
 * All rights reserved.
 *
 * version:     2.1.0.170427
 * author:      zhaonanlin
 * brief:       UDP处理模块
*******************************************************************************/
#include "UdpClient.h"

UdpClient::UdpClient(QObject *parent) : QUdpSocket(parent)
{
    Status = "free";
}

void UdpClient::Init()
{
    this->bind(6000);
    connect(this, SIGNAL(readyRead()), this, SLOT(ReadAll()));
}

void UdpClient::Quit()
{
    this->close();
}

void UdpClient::InitSettings()
{
    QDir dir("./config");
    dir.setFilter(QDir::Files | QDir::Hidden | QDir::NoSymLinks);
    dir.setSorting(QDir::Size | QDir::Reversed);

    QStringList FileNames;
    QFileInfoList list = dir.entryInfoList();
    for (int i=0; i < list.size(); i++)
        FileNames.append(list.at(i).fileName().remove(".ini"));

    QSettings *g_ini = new QSettings(INI_PATH, QSettings::IniFormat);
    g_ini->setIniCodec("GB18030");
    g_ini->beginGroup("GLOBAL");
    //当前使用的测试项目
    FileInUse = g_ini->value("FileInUse", INI_DEFAULT).toString();
    FileInUse.remove(".ini");
    Types = FileNames.join(" ");
    Number = g_ini->value("UserNumber").toString();
    if (Number.isEmpty())
        Number = g_ini->value("Number", "168912000X").toString();
}

void UdpClient::SaveSettings()
{
    QSettings *g_ini = new QSettings(INI_PATH, QSettings::IniFormat);
    g_ini->setIniCodec("GB18030");
    g_ini->beginGroup("GLOBAL");
    g_ini->setValue("UserNumber", Number);
    g_ini->setValue("FileInUse", FileInUse.append(".ini"));
}

void UdpClient::ReadAll()
{
    while (this->hasPendingDatagrams()) {
        QByteArray msg;
        msg.resize(this->pendingDatagramSize());
        QHostAddress sender;
        quint16 senderPort;
        this->readDatagram(msg.data(), msg.size(), &sender, &senderPort);

        QStringList n = QString(msg).split(" ");
        Command = quint16(n.at(0).toInt());
        n.removeFirst();
        Param = n.join(" ").toUtf8();

        switch (Command) {
        case 3000: //查询在线主机
            InitSettings();
            TxMsg = "3001 ";
            TxMsg.append(Number);
            this->writeDatagram(TxMsg.toUtf8(), sender, senderPort);
            break;
        case 3002: //修改主机编号
            if (!Param.isEmpty())
                Number = Param;
            SaveSettings();
            TxMsg = "3002 ";
            TxMsg.append(Number);
            this->writeDatagram(TxMsg.toUtf8(), sender, senderPort);
            break;
        case 3004: //获取测试型号
            InitSettings();
            TxMsg = "3005 ";
            TxMsg.append(Types);
            this->writeDatagram(TxMsg.toUtf8(), sender, senderPort);
            break;
        case 3006: //设置测试型号
            if (Status == "buzy") {
                TxMsg = "3013 ";
                TxMsg.append(Status);
                this->writeDatagram(TxMsg.toUtf8(), sender, senderPort);
                break;
            }
            if (!Param.isEmpty())
                FileInUse = Param;
            SaveSettings();
            emit SendCommand(ADDR, CMD_INIT, NULL);
            TxMsg = "3007 ";
            TxMsg.append(Number);
            this->writeDatagram(TxMsg.toUtf8(), sender, senderPort);
            break;
        case 3008: //启动测试
            if (Status == "buzy") {
                TxMsg = "3013 ";
                TxMsg.append(Status);
                this->writeDatagram(TxMsg.toUtf8(), sender, senderPort);
                break;
            }
            emit SendCommand(ADDR, CMD_START, QString("%1 %2").arg(0x13).arg(0x03).toUtf8());
            TxMsg = "3009 ";
            TxMsg.append(Number);
            this->writeDatagram(TxMsg.toUtf8(), sender, senderPort);
            break;
        case 3010: //停止测试
            emit SendCommand(ADDR, CMD_STOP, QString("%1 %2").arg(0x13).arg(0x03).toUtf8());
            TxMsg = "3011 ";
            TxMsg.append(Number);
            this->writeDatagram(TxMsg.toUtf8(), sender, senderPort);
            break;
        case 3012: //获取状态
            TxMsg = "3013 ";
            TxMsg.append(Status);
            this->writeDatagram(TxMsg.toUtf8(), sender, senderPort);
            break;
        case 3014:
            if (Status != "ready") {
                TxMsg = "3013 ";
                TxMsg.append(Status);
                this->writeDatagram(TxMsg.toUtf8(), sender, senderPort);
                break;
            }
            TxMsg = "3015 ";
            TxMsg.append(Items.join("\n"));
            this->writeDatagram(TxMsg.toUtf8(), sender, senderPort);
            break;
        default:
            break;
        }
    }
}

void UdpClient::ReadMessage(quint16 addr,  quint16 cmd,  QByteArray msg)
{
    if (addr != ADDR)
        return;
    switch (cmd) {
    case CMD_ITEM:
        Items.append(msg);
        break;
    case CMD_STATUS:
        Status = msg;
        if (Status == "buzy")
            Items.clear();
        break;
    default:
        break;
    }
}
/*********************************END OF FILE**********************************/
