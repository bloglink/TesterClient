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
#include <QtXml/QDomDocument>

#include "conf_delegation.h"
#include "qcustomplot.h"

class ConfLoadTesting : public QWidget
{
    Q_OBJECT
public:
    explicit ConfLoadTesting(QWidget *parent = 0);
    ~ConfLoadTesting();

signals:
    void sendNetMsg(QByteArray dat);
    void buttonClicked(QByteArray win);
public slots:
    void initData(QString dat);
private slots:
    void initUI();
    void saveData();
    void appendXmlData(int column, QString name);
    void sequence(void);
    void ruler(double x);
    void wavePacket(double x1,double x2, QString name);
    void recvAppShow(QString win);
private:
    QTableView *view;
    StandardItem *model;

    QCheckBox *turnCheckBox;
    QComboBox *turnComboBox;

    QDomDocument doc;
    QDomElement root;

    QTableView *tView;
    StandardItem *tModel;
    QStringList itemNames;
    QCustomPlot *customplot;
};

#endif // CONFLOADTESTING_H
