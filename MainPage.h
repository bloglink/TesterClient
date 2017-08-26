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
#include <QElapsedTimer>

#include "LoginPage.h"
#include "UdpSocket.h"
#include "HomePage.h"
#include "ConfPage.h"
#include "TestPage.h"

#include "conf_resistance.h"
#include "conf_insulation.h"
#include "conf_current_ac.h"
#include "conf_inductance.h"
#include "conf_noloadtest.h"
#include "conf_loadtesting.h"
#include "conf_backforce.h"
#include "ConfigFG.h"

#include "WinSyst.h"
#include "page_sqlite.h"

#include "ctrl_device_232.h"

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
    void initPLC();
    void initUdp(QJsonObject obj);
    void recvNetMsg(QString msg);
    void readButtons(QByteArray win);
    void testThread();
    void wait(int ms);
    void testInit();
    void testDCR();
    void testINR();
    void testACW();
    void testIND();
    void testPWR();
    void testLOD();
private:
    QList<int> previous_window;
    QStackedWidget *stack;
    UdpSocket udp;
    int preindex;

    ConfPage *conf;
    TestPage *test;

    ConfResistance *resistance;
    ConfCurrent_AC *current_ac;
    ConfInsulation *insulation;
    ConfInductance *inductance;
    ConfNoLoadTest *noloadtest;
    ConfLoadTesting *loadtesting;
    ConfBackForce *backforce;
    ConfigFG *fg;

    CtrlDevice_232 *plc;
};

#endif // MAINPAGE_H
