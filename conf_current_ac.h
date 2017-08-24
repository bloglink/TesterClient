/*******************************************************************************
 * Copyright [2017]   <青岛艾普智能仪器有限公司>
 * All rights reserved.
 *
 * version:     0.1
 * author:      zhaonanlin
 * brief:       交耐配置
*******************************************************************************/
#ifndef CONFCURRENTAC_H
#define CONFCURRENTAC_H

#include <QDebug>
#include <QWidget>
#include <QLayout>
#include <QTableView>
#include <QHeaderView>
#include <QPushButton>
#include <QtXml/QDomDocument>

#include "delegate.h"

class ConfCurrent_AC : public QWidget
{
    Q_OBJECT
public:
    explicit ConfCurrent_AC(QWidget *parent = 0);
    ~ConfCurrent_AC();
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
    ItemModel *model;

    QDomDocument doc;
    QDomElement root;
    QStringList itemNames;
};

#endif // CONFCURRENTAC_H
