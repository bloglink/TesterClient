/*******************************************************************************
 * Copyright [2017]   <青岛艾普智能仪器有限公司>
 * All rights reserved.
 *
 * version:     0.1
 * author:      zhaonanlin
 * brief:       绝缘配置
*******************************************************************************/
#ifndef CONFINSULATION_H
#define CONFINSULATION_H

#include <QDebug>
#include <QWidget>
#include <QLayout>
#include <QSettings>
#include <QTableView>
#include <QHeaderView>
#include <QPushButton>
#include <QJsonObject>

#include "conf_delegation.h"

class ConfInsulation : public QWidget
{
    Q_OBJECT
public:
    explicit ConfInsulation(QWidget *parent = 0);
    ~ConfInsulation();
signals:
    void buttonClicked(QByteArray win);
    void sendAppCmd(QJsonObject obj);
public slots:
    void initSettings(QJsonObject obj);
    void readSettings();
private slots:
    void initUI();
private:
    QTableView *view;
    StandardItem *mView;
    QStringList itemNames;
};

#endif // CONFINSULATION_H
