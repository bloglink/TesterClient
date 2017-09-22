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
#include <QCloseEvent>

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
#include "backpage.h"

#include "iobrd.h"
#include "servo.h"
#include "mbdkt.h"
#include "wt330.h"

#include "MessageBox.h"

#include "lqmath.h"

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
#define STATUS_STOP 0xFE
#define STATUS_TIME 0xFD

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
public slots:
    bool check();
    bool login();
    void readSettings();
    void showWarnning();
private slots:
    void initUI();
    void initCom();
    void initUdp(QJsonObject obj);
    void recvNetMsg(QString msg);
    void readButtons(QByteArray win);
    void wait(int ms);
    void testInit();
    void testDCR();
    void testINR();
    void testACW();
    void testIND();
    void testHAL();
    void readHall();
    void testNLD();
    void readNLD();
    void testLOD();
    void readLOD();
    void testStop();
    void testTimeOut();
    bool waitTimeOut(quint16 s);
    bool testPause();

    void saveSettings();
    QString CurrentSettings();
    QString currentPassword();
    QString currentUser();
    QString readScale();
    QString readVoltScale();
    QString readHighVolt();
    QString readTorqueComp();
    int currentPauseMode();
    int currentAlarmTime(QString msg);
    void setCurrentUser(QString s);
    void recvAppCmd(QJsonObject obj);
    void sendXmlCmd(QJsonObject obj);
    void readNoLoadStart();
    void readNoLoadStop();
    void readStart(bool s, quint16 station);
    void readStartL(bool s);
    void readStartR(bool s);
    void readSelfCheck(QString s);
    QStringList readOffset();
    void sendUdpCommand(QString cmd);
    bool cylinderAction(quint16 cylinder, quint16 s);
    bool mbdktPrevAction(quint16 s);
    bool mbdktPrevActionStop(quint16 s);
    bool mbdktAction(int torque, quint16 s);
    bool mbdktActionStop(int torque, quint16 s);
    void recvAppShow(QString win);
    void showDebug(QString s);
    void closeEvent(QCloseEvent *e);
    void iobrdReset();
    void waitSendStop();

    void thread_system(void);
    void thread_udp();
    void thread_iobrd();
    void thread_servo();
    void thread_mbdkt();
    void thread_dcr();
    void thread_ir();
    void thread_acw();
    void thread_noload();
    void thread_load();
    void thread_hall();
private:
    QList<int> previous_window;
    QStackedWidget *stack;
    UdpSocket udp;
    int preindex;

    ConfPage *conf;
    TestPage *test;
    PageSqlite *winData;
    BackPage *winBack;

    ConfResistance *resistance;
    ConfCurrent_AC *current_ac;
    ConfInsulation *insulation;
    ConfInductance *inductance;
    ConfNoLoadTest *noloadtest;
    ConfLoadTesting *loadtesting;
    ConfBackEMFTest *backemftest;
    ConfHall *halltesting;

    IOBrd iobrdL;
    Servo servoL;
    MBDKT mbdktL;
    IOBrd iobrdR;
    Servo servoR;
    MBDKT mbdktR;

    int status;
    int station;
    int timeOut;

    QJsonObject conf_array;
    QStringList meter;
    QStringList power;

    bool testing;
    bool isNG;
    bool isServo;

    PopupBox *warnnig;
    PopupBox *testBox;

    QStringList waveU;
    QStringList waveV;
    QStringList waveW;
    QStringList waveHu;
    QStringList waveHv;
    QStringList waveHw;

    LQMath math;
    PopupBox *box2;
    bool loadStopEnable;
    bool isLoadStop;
    bool stopping;

    QTimer *load_timer;

};

#endif // MAINPAGE_H
