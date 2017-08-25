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
#include <QtXml/QDomDocument>

#include "conf_delegation.h"
#include "qcustomplot.h"

class ConfBackForce : public QWidget
{
    Q_OBJECT
public:
    explicit ConfBackForce(QWidget *parent = 0);
    ~ConfBackForce();

signals:
    void sendNetMsg(QByteArray dat);
    void buttonClicked(QByteArray win);
public slots:
    void initData(QString dat);
private slots:
    void initUI();
    void saveData();
    void appendXmlData(int column, QString name);
    void recvAppShow(QString win);
private:
    QTableView *view;
    StandardItem *model;

    QSpinBox *nounSpinBox;

    QDomDocument doc;
    QDomElement root;
    QStringList itemNames;

    QCustomPlot *customplot;
};

#endif // CONFBACKFORCE_H
