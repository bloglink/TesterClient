/*******************************************************************************
 * Copyright [2017]   <青岛艾普智能仪器有限公司>
 * All rights reserved.
 *
 * version:     0.1
 * author:      zhaonanlin
 * brief:       电阻配置
*******************************************************************************/
#ifndef CONFRESISTANCE_H
#define CONFRESISTANCE_H

#include <QDebug>
#include <QLabel>
#include <QWidget>
#include <QLayout>
#include <QSpinBox>
#include <QCheckBox>
#include <QSettings>
#include <QTableView>
#include <QHeaderView>
#include <QPushButton>
#include <QMessageBox>
#include <QJsonObject>

#include "conf_delegation.h"

class ConfResistance : public QWidget
{
    Q_OBJECT
public:
    explicit ConfResistance(QWidget *parent = 0);
    ~ConfResistance();
signals:
    void buttonClicked(QByteArray win);
    void sendAppCmd(QJsonObject obj);
public slots:
    void initSettings(QJsonObject obj);
    void readSettings();
private slots:
    void initUI();
    void autoInput();
private:
    QTableView *view;
    StandardItem *mView;

    QSpinBox *nounSpinBox;
    QSpinBox *tempSpinBox;
    QCheckBox *compensation;

    QStringList itemNames;
};

#endif // CONFRESISTANCE_H
