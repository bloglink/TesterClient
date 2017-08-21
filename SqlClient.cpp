/*******************************************************************************
 * Copyright [2016]   <青岛艾普智能仪器有限公司>
 * All rights reserved.
 *
 * version:     2.1.0.170427
 * author:      zhaonanlin
 * brief:       SQL处理模块
*******************************************************************************/
#include "SqlClient.h"

SqlClient::SqlClient(QObject *parent) : QObject(parent)
{
    count = 0;
}

void SqlClient::DeviceOpen()
{
    QFile file(SQL_PATH);
    if (!file.exists()) {
        file.open(QIODevice::ReadWrite);
        file.close();
    }

    db = QSqlDatabase::addDatabase("QSQLITE", "SQL");
    db.setDatabaseName(SQL_PATH);
    if (!db.open())
        qDebug() << QTime::currentTime().toString() << "SQL Error";

    QSqlQuery query(db);
    query.prepare("select count(*) from sqlite_master where type='table' and name='TestData'");
    query.exec();
    if (query.next() && query.value(0).toInt() == 0) {
        QString s = "create table TestData";
        s += " (id int primary key, item text, para text, result text, judge text,";
        s += " b1 text, b2 text, b3 text)";
        query.prepare(s);
        query.exec();
    }
    query.exec("select max(b1) from TestData");
    query.next();
    count = query.value(0).toInt();
    count++;
}

void SqlClient::DeviceQuit()
{
    db.close();
}

void SqlClient::Write(QByteArray msg)
{
    if (msg.startsWith("code")) {
        code = msg.remove(0, 4);
        qDebug() << code;
        return;
    }
    QStringList dat = QString(msg).split("@");
    if (dat.size() < 3)
        return;

    QSqlQuery query(db);

    query.exec("select max(id) from TestData");
    query.next();
    int max = query.value(0).toInt();

    query.prepare("insert into TestData (id, item, para, result, judge, b1, b2, b3)"\
                  "values(:t0, :t1, :t2, :t3, :t4, :t5, :t6, :t7)");
    query.bindValue(":t0", max+1);
    if (dat.size() == 3) {
        query.bindValue(":t1", dat.at(0));
        query.bindValue(":t2", QDate::currentDate());
        query.bindValue(":t3", dat.at(1));
        query.bindValue(":t4", dat.at(2));
        query.bindValue(":t5", count);
        if (code.isEmpty())
            query.bindValue(":t6", " ");
        else
            query.bindValue(":t6", code);
        query.bindValue(":t7", " ");
    }
    if (dat.size() == 4) {
        query.bindValue(":t1", dat.at(0));
        query.bindValue(":t2", dat.at(1));
        query.bindValue(":t3", dat.at(2));
        query.bindValue(":t4", dat.at(3));
        query.bindValue(":t5", count);
        if (code.isEmpty())
            query.bindValue(":t6", " ");
        else
            query.bindValue(":t6", code);
        query.bindValue(":t7", " ");
    }
    query.exec();
    if (dat.at(0) == "总数")
        count++;
}
/*********************************END OF FILE**********************************/


