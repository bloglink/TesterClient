/*******************************************************************************
 * Copyright [2016]   <青岛艾普智能仪器有限公司>
 * All rights reserved.
 *
 * version:     2.1.0.170427
 * author:      zhaonanlin
 * brief:       电感模块
*******************************************************************************/
#ifndef PAGEIND_H
#define PAGEIND_H

#include <qmath.h>
#include <QDate>
#include <QDebug>
#include <QWidget>
#include <QLineEdit>
#include <QComboBox>
#include <QSettings>
#include <QListView>
#include <QShowEvent>
#include <QHeaderView>
#include <QButtonGroup>
#include <QElapsedTimer>
#include <QDoubleSpinBox>
#include <QTableWidgetItem>
#include <stdint.h>

#include "define.h"
#include "PageNum.h"

#define MAX_ROW 8

#define IND_FREE        0
#define IND_INIT        4
#define IND_TEST        1
#define IND_OFFSET      2

union  Resultunion { // 接收电感数据
    float   Result;
    uint8_t dat[4];
};

namespace Ui {
class PageInd;
}

class PageInd : public QWidget
{
    Q_OBJECT

public:
    explicit PageInd(QWidget *parent = 0);
    ~PageInd();

private:
    Ui::PageInd *ui;

signals:
    void SendVariant(QVariant s);
    void SendCommand(quint16 addr, quint16 cmd, QByteArray data);
private slots:
    void InitWindows(void);
    void InitButtons(void);
    void ReadButtons(int id);
    void InitSettings(void);
    void SaveSettings(void);
    void CalculateAuto(void);
    void ItemClick(int r, int c);
    void ItemChange(QString msg);
    int CalculateGear(int row);
    int CalculateMode(int row);

    void ReadMessage(quint16 addr, quint16 cmd, QByteArray msg);
    void ExcuteCanCmd(QByteArray msg);
    void SendTestItemsAllEmpty(void);
    void SendTestItemsAllError(void);
    void SendTestJudge(void);
    void SendCanCmdStatus(void);
    void ReadCanCmdStatus(QByteArray msg);
    void SendCanCmdStart(quint8 pos);
    void ReadCanCmdResult(QByteArray msg);
    void ReadCanCmdOffset(QByteArray msg);
    void SendCanCmdStop(void);
    void SendCanCmdConfig(void);

    bool WaitTimeOut(quint16 t);
    void Delay(int ms);
    QString CurrentSettings(void);
    void showEvent(QShowEvent *);
    void SendWarnning(QString s);
private:
    QSettings *set;
    quint16 TimeOut;
    PageNum *input;
    QStringList Items;
    QList<double> Results;
    QString Judge;

    QList<QTableWidgetItem*> Enable;
    QList<QTableWidgetItem*> Terminal1;
    QList<QTableWidgetItem*> Terminal2;
    QList<QComboBox*> Unit;
    QList<QDoubleSpinBox*> Min;
    QList<QDoubleSpinBox*> Max;
    QList<QDoubleSpinBox*> QMin;
    QList<QDoubleSpinBox*> QMax;
    QList<QDoubleSpinBox*> Std;
    QList<QDoubleSpinBox*> Offset;

    Resultunion  Result1;
    Resultunion  Result2;
    quint8 Mode;
};

#endif // PAGEIND_H
