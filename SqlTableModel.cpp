/*******************************************************************************
 * Copyright [2017]   <青岛艾普智能仪器有限公司>
 * All rights reserved.
 *
 * version:     0.0.0.1
 * author:      zhaonanlin
 * brief:       SQL模型
*******************************************************************************/
#include "SqlTableModel.h"

QMap<int, Qt::CheckState> check_state_map;

SqlTableModel::SqlTableModel(QObject * parent, QSqlDatabase db):QSqlTableModel(parent, db)
{
}

bool SqlTableModel::setData(const QModelIndex &index, const QVariant &value, int role)
{  // setData()方法主要用来设置是否被选中，然后将对应的状态保存到QMap中
    if (!index.isValid())
        return false;

    if (role == Qt::CheckStateRole && index.column() == 0) {
        check_state_map[index.row()] = (value == Qt::Checked ? Qt::Checked : Qt::Unchecked);
        return true;
    } else {
        return QSqlTableModel::setData(index, value, role);
    }
}

QVariant SqlTableModel::data(const QModelIndex &index, int role) const
{  // data()方法主要用来显示，取出QMap中的值，返回对应的状态
    if (!index.isValid())
        return QVariant();

    if (index.column() != 0 )
        return QSqlTableModel::data(index, role);

    switch (role) {
    case Qt::CheckStateRole:
        if (index.column() == 0) {
            if (check_state_map.contains(index.row()))
                return check_state_map[index.row()] == Qt::Checked ? Qt::Checked : Qt::Unchecked;
            return Qt::Unchecked;
        }
    default:
        return QVariant();
    }
    return QVariant();
}

Qt::ItemFlags SqlTableModel::flags(const QModelIndex &index) const
{
    if (!index.isValid())
        return 0;

    if (index.column() == 0)
        return Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsUserCheckable;
    else
        return QSqlTableModel::flags(index);

    return  Qt::ItemIsEnabled | Qt::ItemIsSelectable;
}

