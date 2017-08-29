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
#include "conf_hall.h"

#include "WinSyst.h"
#include "page_sqlite.h"

#include "ctrl_device_232.h"


#define STATUS_DCR 0x01
#define STATUS_MAG 0x02
#define STATUS_INR 0x03
#define STATUS_ACW 0x04
#define STATUS_DCW 0x05
#define STATUS_IMP 0x06
#define STATUS_PWR 0x07
#define STATUS_IND 0x08
#define STATUS_LCK 0x09
#define STATUS_LVS 0x0A
#define STATUS_HAL 0x0B
#define STATUS_LOD 0x0C
#define STATUS_NLD 0x0D
#define STATUS_BMF 0x0E

#define STATUS_FREE 0x00
#define STATUS_OVER 0xFF
#define STATUS_PREP 0xFE

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
    void testNLD();
    void testLOD();
    void testStop();
    void testTimeOut();
    void recvIOMsg(QString msg);
    bool readCylinderL(quint16 s);
    bool waitTimeOut(quint16 s);
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
    ConfBackEMFTest *backemftest;
    ConfHall *halltesting;

    CtrlDevice_232 *plc;

    int status;
    int station;
    int timeOut;
};

#endif // MAINPAGE_H
