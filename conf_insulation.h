/*******************************************************************************
 * Copyright [2017]   <青岛艾普智能仪器有限公司>
 * All rights reserved.
 *
 * version:     0.1
 * author:      zhaonanlin
 * brief:       绝缘配置
*******************************************************************************/
#ifndef CONFINSULATION_H
#define CONFINSULATION_H

#include <QDebug>
#include <QWidget>
#include <QLayout>
#include <QSettings>
#include <QTableView>
#include <QHeaderView>
#include <QPushButton>
#include <QtXml/QDomDocument>

#include "conf_delegation.h"

class ConfInsulation : public QWidget
{
    Q_OBJECT
public:
    explicit ConfInsulation(QWidget *parent = 0);
    ~ConfInsulation();

signals:
    void sendNetMsg(QByteArray dat);
    void buttonClicked(QByteArray win);
public slots:
    void readSettings();
    void saveSettings();
private slots:
    void initUI();

    QString appendXmlData(int column, QString name);
    void recvAppShow(QString win);
    QString CurrentSettings();
private:
    QTableView *view;
    StandardItem *model;

    QDomDocument doc;
    QDomElement root;
    QStringList itemNames;
};

#endif // CONFINSULATION_H
