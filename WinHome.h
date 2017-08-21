/*******************************************************************************
 * Copyright [2016]   <  青岛艾普智能仪器有限公司>
 * All rights reserved.
 *
 * version:     2.1.0.170422
 * author:      zhaonanlin
 * brief:       开机主页
*******************************************************************************/
#ifndef WINHOME_H
#define WINHOME_H

#include <QTime>
#include <QFile>
#include <QTimer>
#include <QDebug>
#include <QWidget>
#include <QThread>
#include <QSettings>
#include <QMessageBox>
#include <QButtonGroup>
#include <QElapsedTimer>

#include "define.h"

#include "WinBack.h"
#include "WinSyst.h"
#include "WinType.h"
#include "WinData.h"
#include "WinTest.h"

#include "PageDcr.h"
#include "PageMag.h"
#include "PageInr.h"
#include "PageAcw.h"
#include "PageImp.h"
#include "PageInd.h"
#include "PagePwr.h"
#include "PageLvs.h"
#include "PageLck.h"
#include "PageOut.h"
#include "PageAmp.h"

#include "SqlClient.h"
#include "CanClient.h"
#include "TcpClient.h"
#include "UdpClient.h"
#include "SerialPort.h"
#include "MessageBox.h"

#define HOME_UNKOWN 0xff
#define HOME_FREE 0x00
#define HOME_TEST 0x01
#define HOME_STOP 0x02

namespace Ui {
class WinHome;
}

class WinHome : public QWidget
{
    Q_OBJECT

public:
    explicit WinHome(QWidget *parent = 0);
    ~WinHome();

private:
    Ui::WinHome *ui;

signals:
    void SendVariant(QVariant s);
    void message(QByteArray msg);
    void PutCanData(QByteArray msg);
    void WriteSql(QByteArray msg);
    void SendCommand(quint16 addr, quint16 cmd, QByteArray data);
private slots:
    void InitThreadAll(void);
    void InitWindows(void);
    void InitWindowsAll(void);
    void JumpToWindow(QByteArray win);
    void InitButtons(void);
    void ReadButtons(int id);
    void InitVersion(QString v);
    void InitCan(void);
    void InitSql(void);
    void InitTcp(void);
    void InitUdp(void);
    void InitSerial(void);
    void ReadCanCmd(QByteArray msg);

    void ReadMessage(quint16 addr, quint16 cmd, QByteArray data);
    void InitTestItems(void);
    void ReadStatusAll(void);
    void StartTest(QByteArray msg);
    void ReStartTest(void);
    void SaveTestJudge(void);
    void SaveItemJudge(QByteArray msg);
    void TestPause(void);
    void ShowLogMessage(QByteArray msg);
    bool WaitTimeOut(quint16 t);
    void Delay(int ms);

    QString CurrentSettings(void);
    QStringList CurrentItems(void);
    QStringList EnableItems(void);
    QStringList EnableOutput(void);
    int CurrentStartMode(void);
    int CurrentPauseMode(void);
    int CurrentAlarmTime(QString msg);
    int CurrentReStartMode(void);
    int CurrentDelay(void);

    void ReadVariant(QVariant s);
    void Error(QVariantHash hash);
    void Warnning(QVariantHash hash);
    void ReadButtonBox(QByteArray msg);
    void SendButtonBox(QString button);
private:
    QList<int> previous_window;
    QString ItemJudge;
    QStringList Items;

    QThread *thread_can;
    QThread *thread_sql;
    QThread *thread_tcp;
    QThread *thread_udp;
    QThread *thread_all;
    CanClient can;
    SqlClient sql;
    TcpClient tcp;
    UdpClient udp;
    SerialPort serial;
    quint8 HomeMode;
    QByteArray stat;
    PopupBox *msgBox;
    quint16 Current_Test_Item;
    QStringList TempItems;
    bool isPause;
};

#endif // WINHOME_H
