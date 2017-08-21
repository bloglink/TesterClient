/*******************************************************************************
 * Copyright [2017]   <青岛艾普智能仪器有限公司>
 * All rights reserved.
 *
 * version:     0.0.0.1
 * author:      zhaonanlin
 * brief:       主界面
*******************************************************************************/
#ifndef MAINPAGE_H
#define MAINPAGE_H

#include <QWidget>
#include <QStackedWidget>
#include <QJsonObject>
#include <QJsonDocument>
#include <QLayout>

#include "LoginPage.h"
#include "UdpSocket.h"
#include "AboutPage.h"
#include "HomePage.h"
#include "ConfPage.h"
#include "TestPage.h"

#include "ConfigDCR.h"
#include "ConfigACW.h"
#include "ConfigIR.h"
#include "ConfigIND.h"
#include "ConfigPWR.h"
#include "ConfigLoad.h"
#include "ConfigFG.h"

#include "WinData.h"
#include "WinSyst.h"

class MainPage : public QWidget
{
    Q_OBJECT
signals:
    void transmitJson(QJsonObject obj);
    void transmitShow(QString win);
    void sendNetMsg(QString msg);
public:
    explicit MainPage(QWidget *parent = 0);
    ~MainPage();

    bool login();
private slots:
    void initUI();
    void initUdp(QJsonObject obj);
    void recvNetMsg(QString msg);
    void readButtons(QByteArray win);
private:
    QList<int> previous_window;
    QStackedWidget *stack;
    UdpSocket udp;
    int preindex;
};

#endif // MAINPAGE_H
