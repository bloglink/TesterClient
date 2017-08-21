/*******************************************************************************
 * Copyright [2016]   <青岛艾普智能仪器有限公司>
 * All rights reserved.
 *
 * version:     2.1.0.170427
 * author:      zhaonanlin
 * brief:       反嵌模块
*******************************************************************************/
#ifndef PAGEMAG_H
#define PAGEMAG_H

#include <QDate>
#include <QWidget>
#include <QListView>
#include <QSettings>
#include <QComboBox>
#include <QShowEvent>
#include <QButtonGroup>
#include <QElapsedTimer>
#include <QDoubleSpinBox>
#include <QTableWidgetItem>
#include "define.h"
#include "Waveform.h"
#include "PageNum.h"

#define MAX_ROW 8

#define MAG_FREE        0
#define MAG_INIT        4
#define MAG_TEST        1
#define MAG_SAMPLE      2

namespace Ui {
class PageMag;
}

class PageMag : public QWidget
{
    Q_OBJECT

public:
    explicit PageMag(QWidget *parent = 0);
    ~PageMag();

private:
    Ui::PageMag *ui;

signals:
    void SendVariant(QVariant s);
    void SendCommand(quint16 addr, quint16 cmd, QByteArray data);
public:
    QList<Waveform *> WaveMag;
private slots:
    void InitWindows(void);
    void InitButtons(void);
    void ReadButtons(int id);
    void InitSettings(void);
    void SaveSettings(void);
    void ItemClick(int r,  int c);
    void ItemChange(QString msg);

    void ReadMessage(quint16 addr, quint16 cmd, QByteArray msg);
    void ExcuteCanCmd(int id, QByteArray msg);
    void SendTestItemsAllEmpty(void);
    void SendTestItemsAllError(void);
    void SendCanCmdSample(quint8 s);
    void SendCanCmdStart(quint8 s);
    void SendCanCmdStop(void);
    void SendCanCmdConfig(quint8 s);
    void SendTestJudge(void);
    void ReadCanCmdStatus(QByteArray msg);
    void ReadCanCmdResult(QByteArray msg);
    void ReadCanCmdWaveStart(QByteArray msg);
    void ReadCanCmdWave(QByteArray msg);
    void ReadCanCmdWaveOk(QByteArray msg);
    void SendWave(QByteArray msg);
    void CalculateDir();

    void SendWarnning(QString s);

    bool WaitTimeOut(quint16 t);
    void Delay(int ms);
    QString CurrentSettings(void);
    virtual void showEvent(QShowEvent*);
private:
    QSettings *set;

    quint16 TimeOut;
    PageNum *input;
    quint8 CurrentWave;
    QStringList Items;
    QString Judge;

    QList<QTableWidgetItem*> Enable;
    QList<QTableWidgetItem*> Terminal1;
    QList<QTableWidgetItem*> Terminal2;
    QList<QDoubleSpinBox*> Max;
    QList<int> WaveNumber;
    QList<int> AreaL;
    QList<int> FreqL;
    QList<int> AreaR;
    QList<int> FreqR;

    quint8 station;
    quint8 MagMode;
};

#endif // PAGEMAG_H
