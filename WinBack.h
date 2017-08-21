/*******************************************************************************
 * Copyright [2016]   <青岛艾普智能仪器有限公司>
 * All rights reserved.
 *
 * version:     2.1.0.170427
 * author:      zhaonanlin
 * brief:       后台处理模块
*******************************************************************************/
#ifndef WINBACK_H
#define WINBACK_H

#include <QTime>
#include <QDebug>
#include <QWidget>
#include <QProcess>
#include <QSettings>
#include <QShowEvent>
#include <QDoubleSpinBox>
#include "define.h"

namespace Ui {
class WinBack;
}

class WinBack : public QWidget
{
    Q_OBJECT

public:
    explicit WinBack(QWidget *parent = 0);
    ~WinBack();

private:
    Ui::WinBack *ui;
signals:
    void SendVariant(QVariant s);
    void SendCommand(quint16 addr, quint16 cmd, QByteArray msg);
private slots:
    void InitWindows(void);
    void InitButtons(void);
    void BtnJudge(int id);
    void JudgeDcrBtn(int id);
    void JudgeInrBtn(int id);
    void JudgeImpBtn(int id);
    void InitSettings(void);
    void SaveSettings(void);
    void ClickItem(int r, int c);
    void ClickOutput(int r, int c);
    void ReadMessage(quint16 addr, quint16 cmd, QByteArray msg);
    void ExcuteCanCmd(quint16 addr, QByteArray msg);
    void SendCanCmdVersion(quint16 id);
    void SendCanCmdParam(quint16 id);
    void SendCanCmdParamDcr(void);
    void SendCanCmdParamInr(void);
    void SendCanCmdParamImp(void);
    void SendCanCmdStartDcr(quint8 gear);
    void ReadCanCmdDcr(QByteArray msg);
    void ReadCanCmdInr(QByteArray msg);
    void ReadCanCmdImp(QByteArray msg);
    void ReadCanCmdInd(QByteArray msg);
    void ReadCanCmdPwr(QByteArray msg);
    void ReadCanCmdOut13(QByteArray msg);
    void ReadCanCmdOut14(QByteArray msg);
    void ReadCanCmdAmp(QByteArray msg);
    void ClearParamDcr(void);
    void ClearParamInr(void);
    void ClearParamImp(void);
    virtual void showEvent(QShowEvent *);
private:
    bool Testing;
    QList<QDoubleSpinBox*> BoxDcr;
    QList<QDoubleSpinBox*> BoxInr;
    QList<QDoubleSpinBox*> BoxImp;
};

#endif // WINBACK_H
