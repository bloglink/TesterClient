/*******************************************************************************
 * Copyright [2017]   <青岛艾普智能仪器有限公司>
 * All rights reserved.
 *
 * version:     0.0.0.1
 * author:      zhaonanlin
 * brief:       功率配置
*******************************************************************************/
#ifndef CONFIGPWR_H
#define CONFIGPWR_H

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

#include "delegate.h"
#include "qcustomplot.h"

class ConfigPWR : public QWidget
{
    Q_OBJECT
public:
    explicit ConfigPWR(QWidget *parent = 0);
    ~ConfigPWR();

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
    ItemModel *model;

    QTableView *tView;
    ItemModel *tModel;

    QCheckBox *turnCheckBox;
    QComboBox *turnComboBox;

    QDomDocument doc;
    QDomElement root;

    QCustomPlot *customplot;
};

#endif // CONFIGPWR_H
