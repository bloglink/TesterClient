/*******************************************************************************
 * Copyright [2016]   <  青岛艾普智能仪器有限公司>
 * All rights reserved.
 *
 * version:     2.1.0.170419
 * author:      zhaonanlin
 * brief:       交耐配置与测试流程
*******************************************************************************/
#ifndef PAGEACW_H
#define PAGEACW_H

#include <qmath.h>
#include <QDate>
#include <QDebug>
#include <QWidget>
#include <QListView>
#include <QSettings>
#include <QComboBox>
#include <QShowEvent>
#include <QButtonGroup>
#include <QElapsedTimer>
#include <QDoubleSpinBox>
#include <QTableWidgetItem>
#include <QCheckBox>
#include "define.h"

#define ACW_FREE        0
#define ACW_INIT        4
#define ACW_TEST        1
#define ACW_ROW         4
#define ACW_OFFSET      5

namespace Ui {
class PageAcw;
}

class PageAcw : public QWidget
{
    Q_OBJECT

public:
    explicit PageAcw(QWidget *parent = 0);
    ~PageAcw();

private:
    Ui::PageAcw *ui;

signals:
    void SendVariant(QVariant s);
    void SendCommand(quint16 addr, quint16 cmd, QByteArray data);
private slots:
    void InitWindows(void);
    void InitButtons(void);
    void ReadButtons(int id);
    void InitSettings(void);
    void SaveSettings(void);
    void ItemClick(int r, int c);
    void ReadMessage(quint16 addr, quint16 cmd, QByteArray msg);
    void ExcuteCanCmd(QByteArray msg);
    void ReadCanCmdStatus(QByteArray msg);
    void ReadCanCmdResult(QByteArray msg);
    void ReadCanCmdOffset(QByteArray msg);
    void SendTestItemsAllEmpty(void);
    void SendTestItemsAllError(void);
    void SendTestItemTemp(void);
    void SendTestItem(void);
    void SendTestItemError(QString s);
    void SendTestJudge(void);
    void SendCanCmdStart(quint8 pos);
    void SendCanCmdStop(void);
    void SendCanCmdConfig(int row);
    void ClearResults(void);
    void CalOffset(void);
    bool WaitTimeOut(quint16 t);
    void Delay(int ms);
    quint16 GetTeminal(int r, int c);
    void InitInput(int r, int c);
    void EnsureInput(void);
    QString CurrentSettings(void);
    bool EnablePhase(void);
    virtual void showEvent(QShowEvent*);
    void SendWarnning(QString s);
private:
    quint16 TimeOut;
    QList<double> Volt;
    QList<double> Curr;
    QStringList Items;
    QString Judge;
    quint8 Mode;

    QList<QTableWidgetItem*> Enable;
    QList<QTableWidgetItem*> Terminal1;
    QList<QTableWidgetItem*> Terminal2;
    QList<QDoubleSpinBox*> Vol;
    QList<QDoubleSpinBox*> Min;
    QList<QDoubleSpinBox*> Max;
    QList<QDoubleSpinBox*> Time;
    QList<QComboBox*> Freq;
    QList<QComboBox*> Arc;
    QList<QDoubleSpinBox*> Offset;
    quint8 TestRow;
    QList<QCheckBox*> Check;
};

#endif // PAGEACW_H
