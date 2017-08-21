/*******************************************************************************
 * Copyright [2016]   <  青岛艾普智能仪器有限公司>
 * All rights reserved.
 *
 * version:     2.1.0.170427
 * author:      zhaonanlin
 * brief:       测试显示模块
*******************************************************************************/
#ifndef WINTEST_H
#define WINTEST_H

#include <QTime>
#include <QTimer>
#include <QDebug>
#include <QWidget>
#include <QSettings>
#include <QKeyEvent>
#include <QHeaderView>
#include <QButtonGroup>
#include <QDataStream>
#include <QElapsedTimer>
#include <QScrollBar>

#include "define.h"
#include "Waveform.h"

namespace Ui {
class WinTest;
}

class WinTest : public QWidget
{
    Q_OBJECT

public:
    explicit WinTest(QWidget *parent = 0);
    ~WinTest();

private:
    Ui::WinTest *ui;

signals:
    void SendVariant(QVariant s);
    void SendCommand(quint16 addr, quint16 cmd, QByteArray data);
private slots:
    void InitWindows(void);
    void InitButtons(void);
    void ReadButtons(int win);
    void InitSettings(void);
    void SaveSettings(void);
    void InitItem(QByteArray msg);
    void ShowItem(QString item);
    void ReplaceItem(QString item);
    void ShowJudge(QString judge);
    void ShowWaveItem(QByteArray msg);
    void ShowWaveByte(QByteArray msg);
    void ShowWaveTest(QByteArray msg);
    void ShowTemperature(QByteArray msg);
    void ShowStation(QByteArray msg);
    void ShowTime(void);
    void ShowCode(void);
    void ClearWave(void);
    void ClickItem(int r, int c);
    void ClickWave(QModelIndex);
    void ClickWave(int x);
    void ClickWave0(int x);
    void ClickWave1(int x);
    void ClickWave2(int x);
    QString BarCode1(void);
    QString BarCode2(void);
    void ReadMessage(quint16 addr, quint16 cmd, QByteArray dataA);
    void showEvent(QShowEvent *);
    void keyPressEvent(QKeyEvent *e);
    void keyReleaseEvent(QKeyEvent *e);
private:
    QTimer *timer;
    QList<Waveform *>wave;
    QString motor_type;
    QStringList Items;
    QString code;
    QTimer *codeTimer;
};

#endif // WINTEST_H
