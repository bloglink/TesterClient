/*******************************************************************************
 * Copyright [2016]   <  青岛艾普智能仪器有限公司>
 * All rights reserved.
 *
 * version:     2.1.0.170417
 * author:      zhaonanlin
 * brief:       型号管理模块
*******************************************************************************/
#ifndef WINTYPE_H
#define WINTYPE_H

#include <QDir>
#include <QTime>
#include <QDebug>
#include <QWidget>
#include <QString>
#include <QProcess>
#include <QShowEvent>
#include <QButtonGroup>
#include <QListView>
#include <QSettings>
#include <QScrollBar>

#include "define.h"

namespace Ui {
class WinType;
}

class WinType : public QWidget
{
    Q_OBJECT

public:
    explicit WinType(QWidget *parent = 0);
    ~WinType();

private:
    Ui::WinType *ui;

signals:
    void SendVariant(QVariant s);
    void SendCommand(quint16 addr, quint16 cmd, QByteArray data);
private slots:
    void InitWindows(void);
    void InitButtons(void);
    void ReadButtons(int id);
    void ReadItemButtons(int id);
    void AddTestItem(int id);
    void ReadAllSettings(void);
    void CopySettings(QString name);
    void ReadAvailableItems(void);
    void InitSettings(void);
    void SaveSettings(void);
    void SelectWireColor(int row, int column);
    void ShowWireColorWindow(void);
    void ShowAvailableItem(int, int);
    void JumptoSetWindows(void);

    void RemoveSettings(void);
    void ChangeSettings(void);
    void QuerySettings(void);
    void ChangeMotorTypes();

    QString CurrentSettings(void);
    QStringList CurrentItems(void);
    QStringList EnableItems(void);
    int CurrentUser(void);

    virtual void showEvent(QShowEvent *);
    void on_TabFile_cellClicked(int row, int column);
    void SendWarnning(QString s);

private:
    QButtonGroup *btnGroup;
    QButtonGroup *itemButtons;
    QStringList prev;
    QStringList ItemNames;
    QStringList FileNames;
};

#endif // WINTYPE_H
