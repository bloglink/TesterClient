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
    initUI();
    initButtons();
    readSettings();
}

WinSyst::~WinSyst()
{
    delete ui;
}

void WinSyst::initUI()
{
    ui->BoxUser->setView(new QListView(this));
    ui->BoxMode->setView(new QListView(this));
    ui->BoxStyle->setView(new QListView(this));
    ui->BoxPower->setView(new QListView(this));
    password.clear();
    file = new QFile("log.txt");
    file->open(QFile::ReadWrite);
}

void WinSyst::initButtons()
{
    QButtonGroup *btnGroup = new QButtonGroup;
    btnGroup->addButton(ui->BtnOk, Qt::Key_0);
    btnGroup->addButton(ui->BtnCancel, Qt::Key_1);
    btnGroup->addButton(ui->BtnPassword, Qt::Key_2);
    btnGroup->addButton(ui->BtnSystPasswordOK, Qt::Key_3);
    btnGroup->addButton(ui->BtnSystPasswordExit, Qt::Key_4);
    btnGroup->addButton(ui->BtnExit, Qt::Key_4);
    btnGroup->addButton(ui->btnBack, Qt::Key_5);
    connect(btnGroup, SIGNAL(buttonClicked(int)), this, SLOT(ReadButtons(int)));
}

void WinSyst::ReadButtons(int id)
{
    switch (id) {
    case Qt::Key_0:
        if (ui->EditPassword->text() == "AIP9918")
            emit buttonClicked("BackPage");
        else if (ui->EditPassword->text() == password)
            ui->StackWinSyst->setCurrentIndex(0);
        else if (ui->EditPassword->text() == "aip9918")
            ui->StackWinSyst->setCurrentIndex(0);
        else
            ui->EditPassword->clear();
        break;
    case Qt::Key_1:
        emit buttonClicked(NULL);
        break;
    case Qt::Key_2:
        ui->StackWinSyst->setCurrentIndex(2);
        break;
    case Qt::Key_3:
        SetPassword();
        break;
    case Qt::Key_4:
        ui->StackWinSyst->setCurrentIndex(1);
        SaveSettings();
        emit buttonClicked(NULL);
        break;
    case Qt::Key_5:
        ui->StackWinSyst->setCurrentIndex(1);
        break;
    default:
        break;
    }
}

void WinSyst::readSettings()
{
    QSettings *g_ini = new QSettings("./nandflash/global.ini", QSettings::IniFormat);
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

void WinSyst::recvAppShow(QString win)
{
    if (win != this->objectName())
        return;
    readSettings();
}

/*********************************END OF FILE**********************************/
