/*******************************************************************************
 * Copyright [2017]   <青岛艾普智能仪器有限公司>
 * All rights reserved.
 *
 * version:     0.0.0.1
 * author:      zhaonanlin
 * brief:       负载配置
*******************************************************************************/
#ifndef CONFIGLOAD_H
#define CONFIGLOAD_H

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
#include <QSpinBox>
#include <QLabel>

#include "conf_delegation.h"
#include "qcustomplot.h"

class ConfigLoad : public QWidget
{
    Q_OBJECT
public:
    explicit ConfigLoad(QWidget *parent = 0);
    ~ConfigLoad();

signals:
    void sendNetMsg(QByteArray dat);
    void buttonClicked(QByteArray win);
private slots:
    void initUI();
    void initData(QByteArray dat);
    void saveData();
    void appendXmlData(int column, QString name);
    void sequence(void);
    void ruler(double x);
    void wavePacket(double x1,double x2, QString name);
    void recvAppShow(QString win);
private:
    QTableView *view;
    StandardItem *model;

    QCheckBox *turnCheckBox;
    QComboBox *turnComboBox;

    QDomDocument doc;
    QDomElement root;

    QTableView *tView;
    StandardItem *tModel;

    QCustomPlot *customplot;
};

#endif // CONFIGLOAD_H
