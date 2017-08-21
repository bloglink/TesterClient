/*******************************************************************************
 * Copyright [2017]   <青岛艾普智能仪器有限公司>
 * All rights reserved.
 *
 * version:     0.0.0.1
 * author:      zhaonanlin
 * brief:       交耐配置
*******************************************************************************/
#ifndef CONFIGACW_H
#define CONFIGACW_H

#include <QWidget>
#include <QTableView>
#include <QHeaderView>
#include <QLayout>
#include <QCheckBox>
#include <QLineEdit>
#include <QComboBox>
#include <QPushButton>
#include <QDebug>
#include <QtXml/QDomDocument>
#include <QXmlStreamWriter>
#include <QXmlStreamReader>

#include "delegate.h"

class ConfigACW : public QWidget
{
    Q_OBJECT
public:
    explicit ConfigACW(QWidget *parent = 0);
    ~ConfigACW();
signals:
    void saveConfig(QByteArray dat);
    void buttonClicked(QByteArray win);
private slots:
    void initUI();
    void initData(QByteArray dat);
    void saveData();
private:
    QTableView *view;
    ItemModel *model;
};

#endif // CONFIGACW_H
