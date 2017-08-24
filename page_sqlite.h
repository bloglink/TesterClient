/*******************************************************************************
 * Copyright [2017]   <青岛艾普智能仪器有限公司>
 * All rights reserved.
 *
 * version:     0.1
 * author:      zhaonanlin
 * brief:       数据管理
*******************************************************************************/
#ifndef PAGESQLITE_H
#define PAGESQLITE_H

#include <QDebug>
#include <QLabel>
#include <QWidget>
#include <QLayout>
#include <QLineEdit>
#include <QCheckBox>
#include <QGroupBox>
#include <QTableView>
#include <QHeaderView>
#include <QPushButton>
#include <QtXml/QDomDocument>

#include "conf_delegation.h"

#include "qcustomplot.h"

class PageSqlite : public QWidget
{
    Q_OBJECT
public:
    explicit PageSqlite(QWidget *parent = 0);
    ~PageSqlite();
signals:
    void buttonClicked(QByteArray win);
private slots:
    void initUI();
    void drawHistogram(QStringList names);
    void saveData();
private:
    QTableView *view;
    StandardItem *model;
    QCustomPlot *customplot;
};

#endif // PAGESQLITE_H
