/*******************************************************************************
 * Copyright [2017]   <青岛艾普智能仪器有限公司>
 * All rights reserved.
 *
 * version:     0.0.0.1
 * author:      zhaonanlin
 * brief:       SQL模型
*******************************************************************************/
#ifndef SQLTABLEMODEL_H
#define SQLTABLEMODEL_H

#include <QSqlTableModel>

class SqlTableModel:public QSqlTableModel
{
    Q_OBJECT

public:
    explicit SqlTableModel(QObject * parent = 0, QSqlDatabase db = QSqlDatabase());
    bool setData(const QModelIndex &index, const QVariant &value, int role);
    QVariant data(const QModelIndex &index, int role) const;
    Qt::ItemFlags flags(const QModelIndex &index) const;
};

#endif // SQLTABLEMODEL_H
