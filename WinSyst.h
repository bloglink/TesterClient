/*******************************************************************************
 * Copyright [2016]   <  青岛艾普智能仪器有限公司>
 * All rights reserved.
 *
 * version:     2.1.0.170427
 * author:      zhaonanlin
 * brief:       系统设置模块
*******************************************************************************/
#ifndef WINSYST_H
#define WINSYST_H

#include <QFile>
#include <QTime>
#include <QTimer>
#include <QDesktopWidget>
#include <QDebug>
#include <QWidget>
#include <QProcess>
#include <QShowEvent>
#include <QListView>
#include <QSettings>
#include <QButtonGroup>
#include <QTextStream>
#include <QHostAddress>
#include <QNetworkInterface>

#include "define.h"

namespace Ui {
class WinSyst;
}

class WinSyst : public QWidget
{
    Q_OBJECT

public:
    explicit WinSyst(QWidget *parent = 0);
    ~WinSyst();

private:
    Ui::WinSyst *ui;

signals:
    void SendVariant(QVariant s);
    void SendCommand(quint16 addr, quint16 cmd, QByteArray data);
    void buttonClicked(QByteArray win);
private slots:
    void initUI(void);
    void initButtons(void);
    void ReadButtons(int id);
    void readSettings(void);
    void SaveSettings(void);
    void SetPassword(void);
    QString GetLocalHostIP(void);
    void recvAppShow(QString win);
private:
    QString password;
    QFile *file;
    QDateTime dateTime;
};

#endif // WINSYST_H
