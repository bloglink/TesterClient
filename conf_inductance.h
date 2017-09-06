/*******************************************************************************
 * Copyright [2017]   <青岛艾普智能仪器有限公司>
 * All rights reserved.
 *
 * version:     0.1
 * author:      zhaonanlin
 * brief:       电感配置
*******************************************************************************/
#ifndef CONFINDUCTANCE_H
#define CONFINDUCTANCE_H

#include <QDebug>
#include <QLabel>
#include <QWidget>
#include <QLayout>
#include <QSpinBox>
#include <QCheckBox>
#include <QSettings>
#include <QTableView>
#include <QMessageBox>
#include <QHeaderView>
#include <QPushButton>
#include <QJsonObject>

#include "conf_delegation.h"

class ConfInductance : public QWidget
{
    Q_OBJECT
public:
    explicit ConfInductance(QWidget *parent = 0);
    ~ConfInductance();

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
    QSpinBox *avrgSpinBox;
    QComboBox *freqComboBox;
    QComboBox *connComboBox;
    QComboBox *modeComboBox;
    QStringList itemNames;
};

#endif // CONFINDUCTANCE_H
