/*******************************************************************************
 * Copyright [2017]   <青岛艾普智能仪器有限公司>
 * All rights reserved.
 *
 * version:     0.0.0.1
 * author:      zhaonanlin
 * brief:       绝缘配置
*******************************************************************************/
#ifndef CONFIGIR_H
#define CONFIGIR_H

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

class ConfigIR : public QWidget
{
    Q_OBJECT
public:
    explicit ConfigIR(QWidget *parent = 0);
    ~ConfigIR();

signals:
    void sendNetMsg(QByteArray dat);
    void buttonClicked(QByteArray win);
public slots:
    void initData(QString dat);
private slots:
    void initUI();
    void saveData();
    void recvAppShow(QString win);
private:
    QTableView *view;
    ItemModel *model;
};

#endif // CONFIGIR_H
