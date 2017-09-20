/*******************************************************************************
 * Copyright [2017]   <青岛艾普智能仪器有限公司>
 * All rights reserved.
 *
 * version:     0.1
 * author:      zhaonanlin
 * brief:       负载配置
*******************************************************************************/
#ifndef CONFLOADTESTING_H
#define CONFLOADTESTING_H

#include <QDebug>
#include <QLabel>
#include <QWidget>
#include <QLayout>
#include <QSpinBox>
#include <QCheckBox>
#include <QComboBox>
#include <QTableView>
#include <QMessageBox>
#include <QHeaderView>
#include <QPushButton>
#include <QJsonObject>

#include "conf_delegation.h"
#include "qcustomplot.h"

class ConfLoadTesting : public QWidget
{
    Q_OBJECT
public:
    explicit ConfLoadTesting(QWidget *parent = 0);
    ~ConfLoadTesting();
signals:
    void buttonClicked(QByteArray win);
    void sendAppCmd(QJsonObject obj);
public slots:
    void initSettings(QJsonObject obj);
    void readSettings();
    double readTorque();
private slots:
    void initUI();
    void sequence(void);
    void ruler(double x);
    void wavePacket(double x1,double x2, QString name);
    void back();
private:
    QTableView *view;
    StandardItem *mView;
    QCheckBox *turnCheckBox;
    QComboBox *turnComboBox;
    QTableView *tView;
    StandardItem *tModel;
    QStringList itemNames;
    QCustomPlot *customplot;
};

#endif // CONFLOADTESTING_H
