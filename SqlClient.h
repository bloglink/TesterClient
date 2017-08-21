/*******************************************************************************
 * Copyright [2016]   <青岛艾普智能仪器有限公司>
 * All rights reserved.
 *
 * version:     2.1.0.170427
 * author:      zhaonanlin
 * brief:       SQL处理模块
*******************************************************************************/
#ifndef SQLCLIENT_H
#define SQLCLIENT_H

#include <QFile>
#include <QDate>
#include <QTime>
#include <QDebug>
#include <QObject>
#include <QVariant>
#include <QSqlQuery>
#include <QSqlError>
#include <QSqlDatabase>
#include <QStringList>

#include "define.h"

class SqlClient : public QObject
{
    Q_OBJECT
public:
    explicit SqlClient(QObject *parent = 0);

private slots:
    void DeviceOpen(void);
    void DeviceQuit(void);
    void Write(QByteArray msg);
private:
    QSqlDatabase db;
    int count;
    QString code;
};

#endif // SQLCLIENT_H
