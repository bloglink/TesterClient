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
#include <QJsonArray>
#include <QJsonObject>
#include <QJsonDocument>
#include <QLayout>
#include <QElapsedTimer>
#include <QMessageBox>

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

#include "iobrd.h"
#include "servo.h"
#include "mbdkt.h"

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
#define STATUS_EMF 0x0E

#define STATUS_FREE 0x00
#define STATUS_OVER 0xFF
#define STATUS_PREP 0xFE

#define X01_TARGET 0x0100
#define X01_ORIGIN 0x0200
#define X02_TARGET 0x0400
#define X02_ORIGIN 0x0800
#define X03_TARGET 0x0800
#define X03_ORIGIN 0x0400
#define X04_TARGET 0x4000
#define X04_ORIGIN 0x8000

#define X10 0x0001  // 启动
#define X11 0x0002  // 启动
#define X12 0x0004  // 停止
#define X13 0x0008  // 停止

#define Y00 0x0001  // 台下起降气缸1
#define Y01 0x0002  // 台下夹紧气缸2
#define Y02 0x0004  // 台上压紧气缸3
#define Y03 0x0008  // 台上耐压气缸4
#define Y08 0x0100  // 蜂鸣器
#define Y09 0x0200  // 红灯
#define Y10 0x0400  // 黄灯
#define Y11 0x0800  // 绿灯

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
    void readSettings();
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
    void testHAL();
    void testNLD();
    void testLOD();
    void testEMF();
    void testStop();
    void testStopAction();
    void testTimeOut();
    void recvIOMsg(QString msg);
    bool readCylinderL(quint16 s);
    bool waitTimeOut(quint16 s);

    void saveSettings();
    QString CurrentSettings();
    void recvAppCmd(QJsonObject obj);
    void sendXmlCmd(QJsonObject obj);
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

//    CtrlDevice_232 *plc;

    IOBrd iobrd;
    Servo servo;
    MBDKT plc;

    int status;
    int station;
    int timeOut;

    QJsonObject conf_array;
};

#endif // MAINPAGE_H
