/*******************************************************************************
 * Copyright [2017]   <青岛艾普智能仪器有限公司>
 * All rights reserved.
 *
 * version:     0.1
 * author:      zhaonanlin
 * brief:       交耐配置
*******************************************************************************/
#ifndef CONFCURRENTAC_H
#define CONFCURRENTAC_H

#include <QDebug>
#include <QWidget>
#include <QLayout>
#include <QSettings>
#include <QTableView>
#include <QHeaderView>
#include <QPushButton>
#include <QJsonObject>

#include "conf_delegation.h"
#include "dspinbox.h"

class ConfCurrent_AC : public QWidget
{
    Q_OBJECT
public:
    explicit ConfCurrent_AC(QWidget *parent = 0);
    ~ConfCurrent_AC();
signals:
    void buttonClicked(QByteArray win);
    void sendAppCmd(QJsonObject obj);
public slots:
    void initSettings(QJsonObject obj);
    void readSettings();
private slots:
    void initUI();
    void back();
private:
    QTableView *view;
    StandardItem *mView;
    QStringList itemNames;
};

#endif // CONFCURRENTAC_H
