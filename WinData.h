/*******************************************************************************
 * Copyright [2016]   <青岛艾普智能仪器有限公司>
 * All rights reserved.
 *
 * version:     2.1.0.170424
 * author:      zhaonanlin
 * brief:       数据管理模块
*******************************************************************************/
#ifndef WINDATA_H
#define WINDATA_H

#include <QDir>
#include <QFileInfo>
#include <QMessageBox>
#include <QShowEvent>
#include <QTime>
#include <QFile>
#include <QDebug>
#include <QWidget>
#include <QSqlQuery>
#include <QSqlDatabase>
#include <QSqlTableModel>
#include <QTextCodec>

#include "define.h"

namespace Ui {
class WinData;
}

class WinData : public QWidget
{
    Q_OBJECT

public:
    explicit WinData(QWidget *parent = 0);
    ~WinData();

private:
    Ui::WinData *ui;
signals:
    void SendVariant(QVariant s);
    void SendCommand(quint16 addr, quint16 cmd, QByteArray data);
    void buttonClicked(QByteArray win);
private slots:
    void InitWindows(void);
    void InitButtons(void);
    void ReadButtons(int id);
    void InitSqlite(void);
    void ReadSqliteAll(void);
    void ReadSqlite(QString item);
    void ClearSqlite(void);
    void ExportToUsb(void);
    QString ReadUsb(void);
    virtual void showEvent(QShowEvent *);

    QByteArray ToGbk(const QString &inStr);
private:
    QSqlDatabase db;
    QSqlTableModel *model;
};

#endif // WINDATA_H
