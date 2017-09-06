/*******************************************************************************
 * Copyright [2016]   <  青岛艾普智能仪器有限公司>
 * All rights reserved.
 *
 * version:     2.1.0.170427
 * author:      zhaonanlin
 * brief:       系统设置模块
*******************************************************************************/
#include "WinSyst.h"
#include "ui_WinSyst.h"

WinSyst::WinSyst(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::WinSyst)
{
    ui->setupUi(this);
    this->setObjectName("SystPage");
    InitWindows();
    InitButtons();
    QTimer *timer = new QTimer(this);
    timer->singleShot(10, this, SLOT(ReadHardWareSpace()));
}

WinSyst::~WinSyst()
{
    delete ui;
}

void WinSyst::InitWindows()
{
    ui->BoxUser->setView(new QListView(this));
    ui->BoxMode->setView(new QListView(this));
    ui->BoxStyle->setView(new QListView(this));
    ui->BoxPower->setView(new QListView(this));
    password.clear();
    file = new QFile("log.txt");
    file->open(QFile::ReadWrite);

    dateTime = ui->EditTime->dateTime();
#ifdef __arm__
    ui->BtnScreen->setEnabled(true);
#endif
}

void WinSyst::InitButtons()
{
    QButtonGroup *btnGroup = new QButtonGroup;
    btnGroup->addButton(ui->BtnOk, Qt::Key_0);
    btnGroup->addButton(ui->BtnCancel, Qt::Key_1);
    btnGroup->addButton(ui->BtnPassword, Qt::Key_2);
    btnGroup->addButton(ui->BtnSystPasswordOK, Qt::Key_3);
    btnGroup->addButton(ui->BtnSystPasswordExit, Qt::Key_4);
    btnGroup->addButton(ui->BtnExit, Qt::Key_4);
    btnGroup->addButton(ui->btnTime, Qt::Key_5);
    btnGroup->addButton(ui->BtnStatus, Qt::Key_6);
    btnGroup->addButton(ui->BtnScreen, Qt::Key_7);
    connect(btnGroup, SIGNAL(buttonClicked(int)), this, SLOT(ReadButtons(int)));
}

void WinSyst::ReadButtons(int id)
{
    switch (id) {
    case Qt::Key_0:
        if (ui->EditPassword->text() == "AIP9918")
            emit SendCommand(ADDR, CMD_JUMP, "WinBack");
        else if (ui->EditPassword->text() == password)
            ui->StackWinSyst->setCurrentIndex(0);
        else if (ui->EditPassword->text() == "aip9918")
            ui->StackWinSyst->setCurrentIndex(0);
        else
            ui->EditPassword->clear();
        break;
    case Qt::Key_1:
        emit buttonClicked(NULL);
        emit SendCommand(ADDR, CMD_JUMP, NULL);
        break;
    case Qt::Key_2:
        ui->StackWinSyst->setCurrentIndex(2);
        break;
    case Qt::Key_3:
        SetPassword();
        break;
    case Qt::Key_4:
        SaveSettings();
        emit buttonClicked(NULL);
        emit SendCommand(ADDR, CMD_JUMP, NULL);
        break;
    case Qt::Key_5:
        SetDateTime();
        break;
    case Qt::Key_6:
        emit SendCommand(ADDR, CMD_STATUS, NULL);
        break;
    case Qt::Key_7:
//        system("/bin/ts_calibrate -&");
        break;
    default:
        break;
    }
}

void WinSyst::readSettings()
{
    QSettings *g_ini = new QSettings(INI_PATH, QSettings::IniFormat);
    g_ini->setIniCodec("GB18030");
    g_ini->beginGroup("GLOBAL");

    ui->BoxUser->setCurrentIndex(g_ini->value("User","0").toInt());
    ui->BoxMode->setCurrentIndex(g_ini->value("Mode","0").toInt());
    ui->BoxStyle->setCurrentIndex(g_ini->value("Style","0").toInt());
    ui->BoxTimeNG->setValue(g_ini->value("TimeNG","0.5").toDouble());
    ui->BoxTimeOK->setValue(g_ini->value("TimeOK","0.2").toDouble());
    ui->BoxPower->setCurrentIndex(g_ini->value("PowerSupply","0").toInt());
    ui->EditCode1->setText(g_ini->value("Barcode1","0").toString());
    ui->EditCode2->setText(g_ini->value("Barcode2","9").toString());

    password = g_ini->value("Password","").toString();
    ui->EditPassword->clear();
    ui->EditPwdNew->clear();
    ui->EditPwdNewR->clear();
    ui->EditPwdOld->clear();
#ifdef __arm__
    int t = g_ini->value("AddSeconds","0").toInt();
    if (dateTime.secsTo(QDateTime::currentDateTime()) < t) {
        QDateTime tt = dateTime;
        tt = tt.addSecs(t);
        ui->EditTime->setDateTime(tt);
        SetDateTime();
        SendWarnning(tr("日期丢失"));
    } else {
        ui->EditTime->setDateTime(QDateTime::currentDateTime());
    }
#endif
    if (ui->LocalHostIP->text().isEmpty())
        ui->LocalHostIP->setText(GetLocalHostIP());
    qDebug() << QTime::currentTime().toString() << "WinSyst read OK";
}

void WinSyst::SaveSettings()
{
    QSettings *g_ini = new QSettings(INI_PATH, QSettings::IniFormat);
    g_ini->setIniCodec("GB18030");
    g_ini->beginGroup("GLOBAL");
    g_ini->setValue("User", QString::number(ui->BoxUser->currentIndex()));
    g_ini->setValue("Mode", QString::number(ui->BoxMode->currentIndex()));
    g_ini->setValue("Style", QString::number(ui->BoxStyle->currentIndex()));
    g_ini->setValue("Password", password);
    g_ini->setValue("AddSeconds", dateTime.secsTo(QDateTime::currentDateTime()));
    g_ini->setValue("TimeNG", ui->BoxTimeNG->value());
    g_ini->setValue("TimeOK", ui->BoxTimeOK->value());
    g_ini->setValue("PowerSupply", ui->BoxPower->currentIndex());
    g_ini->setValue("Barcode1", ui->EditCode1->text());
    g_ini->setValue("Barcode2", ui->EditCode2->text());

    qDebug() << QTime::currentTime().toString() << "WinSyst save OK";
}

void WinSyst::SetDateTime()
{
#ifdef __arm__
    QString time = ui->EditTime->dateTime().toString("yyyy.MM.dd-hh:mm:ss");
    QProcess::execute(QString("date %1").arg(time));
    QProcess::execute(QString("hwclock -w"));
#endif
}

void WinSyst::SetPassword()
{
    QString old = ui->EditPwdOld->text();
    QString new1 = ui->EditPwdNew->text();
    QString new2 = ui->EditPwdNewR->text();
    if (old == password && new1 == new2) {
        password = new1;
        SaveSettings();
        ui->StackWinSyst->setCurrentIndex(0);
    }
}

void WinSyst::ReadMessage(quint16 addr,  quint16 cmd,  QByteArray msg)
{
    if (addr != ADDR && addr != WIN_ID_SYS)
        return;
    switch (cmd) {
    case CMD_DEBUG:
        WriteLog(msg);
        break;
    case CMD_INIT:
        SendWinCmdStartMode();
        break;
    default:
        break;
    }
}

void WinSyst::SendWinCmdStartMode()
{
    QByteArray msg;
    msg.append(quint8(ui->BoxMode->currentIndex()));
    emit SendCommand(WIN_ID_OUT13, CMD_INIT, msg);
}

void WinSyst::WriteLog(QByteArray msg)
{
    ui->TextDebug->insertPlainText(msg);
    ui->TextDebug->moveCursor(QTextCursor::End);
    QTextStream out(file);
    out.seek(file->size());
    out << QDateTime::currentDateTime().toString("yyyyMMdd hh:mm ");
    out << msg;
}

QString WinSyst::GetLocalHostIP()
{
    QList<QHostAddress> AddressList = QNetworkInterface::allAddresses();
    QHostAddress result;
    foreach(QHostAddress address,  AddressList) {
        if (address.protocol() == QAbstractSocket::IPv4Protocol &&
                address != QHostAddress::Null &&
                address != QHostAddress::LocalHost) {
            if (address.toString().contains("127.0."))
                continue;
            result = address;
            break;
        }
    }
    return result.toString();
}

void WinSyst::ReadHardWareSpace()
{
#ifdef __arm__
    QProcess *cmd = new QProcess(this);
    cmd->start("df -h");
    cmd->waitForFinished();
    QStringList s = QString(cmd->readAll()).split("\n");
    for (int i=0; i < s.size(); i++) {
        if (s.at(i).contains("/mnt/nandflash")) {
            QStringList t = s.at(i).split(" ", QString::SkipEmptyParts);
            QString a = t.at(qMin(4, t.size()));
            ui->TextSpace->setText(a);
            int b = a.remove(a.size()-1, 1).toInt();
            if (b > 80)
                SendWarnning(tr("系统空间不足%1%,请清理空间").arg(100-b));
        }
    }
#endif
}

void WinSyst::SendWarnning(QString s)
{
    QVariantHash hash;
    hash.insert("TxAddress","WinHome");
    hash.insert("TxCommand","Warnning");
    hash.insert("TxMessage", tr("系统异常:\n%1").arg(s));
    emit SendVariant(QVariant::fromValue(hash));
}

void WinSyst::recvAppShow(QString win)
{
    if (win != this->objectName())
        return;
    readSettings();
}

/*********************************END OF FILE**********************************/
