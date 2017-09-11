/*******************************************************************************
 * Copyright [2017]   <青岛艾普智能仪器有限公司>
 * All rights reserved.
 *
 * version:     0.1
 * author:      zhaonanlin
 * brief:       BEMF配置
*******************************************************************************/
#ifndef CONFBACKFORCE_H
#define CONFBACKFORCE_H

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

#include "ddoublespinbox.h"

#include "conf_delegation.h"
#include "qcustomplot.h"

class ConfBackEMFTest : public QWidget
{
    Q_OBJECT
public:
    explicit ConfBackEMFTest(QWidget *parent = 0);
    ~ConfBackEMFTest();
signals:
    void buttonClicked(QByteArray win);
    void sendAppCmd(QJsonObject obj);
public slots:
    void initSettings(QJsonObject obj);
    void readSettings();
    int readSpeed();
    int readTurn();
    QStringList readLimit();
private slots:
    void initUI();
    void back();
private:
    QTableView *view;
    StandardItem *mView;
    QSpinBox *nounSpinBox;
    QStringList itemNames;
    QCustomPlot *customplot;
};

#endif // CONFBACKFORCE_H
