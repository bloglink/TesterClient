/*******************************************************************************
 * Copyright [2016]   <青岛艾普智能仪器有限公司>
 * All rights reserved.
 *
 * version:     2.1.0.170427
 * author:      zhaonanlin
 * brief:       电阻模块
*******************************************************************************/
#ifndef PAGEDCR_H
#define PAGEDCR_H

#include <qmath.h>
#include <QDate>
#include <QTimer>
#include <QDebug>
#include <QWidget>
#include <QComboBox>
#include <QSettings>
#include <QListView>
#include <QShowEvent>
#include <QHeaderView>
#include <QButtonGroup>
#include <QElapsedTimer>
#include <QDoubleSpinBox>
#include <QTableWidgetItem>

#include "define.h"
#include "PageNum.h"

#define  ResCu    0.0039  //  铜材料
#define  ResCu_Al 0.0041  //  铜铝材料
#define  ResAl    0.0043  //  铝材料

#define MAX_ROW 8

#define DCR_FREE        0
#define DCR_INIT        4
#define DCR_TEST        1
#define DCR_OFFSET      2

namespace Ui {
class PageDcr;
}

class PageDcr : public QWidget
{
    Q_OBJECT

public:
    explicit PageDcr(QWidget *parent = 0);
    ~PageDcr();

private:
    Ui::PageDcr *ui;

signals:
    void SendVariant(QVariant s);
    void SendCommand(quint16 addr, quint16 cmd, QByteArray data);
private slots:
    void InitWindows(void);
    void InitButtons(void);
    void ReadButtons(int id);
    void InitSettings(void);
    void SaveSettings(void);
    void AutoCalculateMinAndMax(void);
    bool CheckSetting(void);
    void ItemClick(int r, int c);
    void ItemChange(QString msg);
    void ReadMessage(quint16 addr, quint16 cmd, QByteArray msg);
    void ExcuteCanCmd(QByteArray msg);
    void ExcuteCanCmdPwr(QByteArray msg);
    void ReadCanCmdStatus(QByteArray msg);
    void ReadCanCmdResult(QByteArray msg);
    void ReadOffset(QByteArray msg);
    void SendTestItemsAllEmpty(void);
    void SendTestItemsAllError(void);
    void SendTestJudge(void);
    void SendCanCmdStatus(void);
    void SendCanCmdStart(quint8 pos);
    void SendCanCmdStop(void);
    void SendCanCmdConfig(void);
    void SendAlarm(QByteArray addr);
    void SendCanCmdPwr(quint8 pos);
    void SendWarnning(QString s);
    double CalculateOffset(double t, quint8 num);
    void CalculateBalance(void);
    int CalculateGear(int row);
    bool WaitTimeOut(quint16 t);
    void Delay(int ms);
    QString CurrentSettings(void);
    QString CurrentPorwer(void);
    bool IsPowerOn(void);
    void AutoChangeMetal(int index);
    void AutoChangeUnit(int index);
    void AutoChangeMin(double x);
    void AutoChangeMax(double x);
    void AutoChangeStd(double x);
    virtual void showEvent(QShowEvent*);
private:
    quint16 TimeOut;
    PageNum *input;
    QStringList Items;
    QList<double> Results;
    QString Judge;

    QList<QTableWidgetItem*> Enable;
    QList<QTableWidgetItem*> Terminal1;
    QList<QTableWidgetItem*> Terminal2;
    QList<QComboBox*> Metal;
    QList<QComboBox*> Unit;
    QList<QDoubleSpinBox*> Min;
    QList<QDoubleSpinBox*> Max;
    QList<QDoubleSpinBox*> Std;
    QList<QDoubleSpinBox*> Offset;
    QList<QDoubleSpinBox*> OffsetR;
    quint8 Mode;
    quint8 stat;
    QList<double> pwr;
};

#endif // PAGEDCR_H
