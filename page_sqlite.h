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
#include <QSqlQuery>
#include <QSqlDatabase>
#include <QDateTime>
#include <QDateEdit>

#include "conf_delegation.h"

#include "qcustomplot.h"
#include "Snowflake.h"

class PageSqlite : public QWidget
{
    Q_OBJECT
public:
    explicit PageSqlite(QWidget *parent = 0);
    ~PageSqlite();
signals:
    void buttonClicked(QByteArray win);
public slots:
    void saveSql(QJsonObject obj);
private slots:
    void initUI();
    void initSql();
    void initSqlTableModel();
    void readViews(QModelIndex index);
    void drawHistogram(QStringList names);
    void saveData();
    void querySql();
    void clearSql();
    void exportSql();
    QByteArray ToGbk(const QString &inStr);
private:
    QTableView *view;
    SqlTableModel *mView;
    QTableView *views;
    SqlTableModel *mViews;
    QCustomPlot *customplot;
    QSqlDatabase db;
    Snowflake snow;
    QDateEdit *date;
    double current_id;
};

#endif // PAGESQLITE_H
