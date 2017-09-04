/*******************************************************************************
 * Copyright [2017]   <青岛艾普智能仪器有限公司>
 * All rights reserved.
 *
 * version:     0.0.0.1
 * author:      zhaonanlin
 * brief:       空载配置
*******************************************************************************/
#ifndef CONFNOLOADTEST_H
#define CONFNOLOADTEST_H

#include <QDebug>
#include <QLabel>
#include <QWidget>
#include <QLayout>
#include <QSpinBox>
#include <QCheckBox>
#include <QComboBox>
#include <QTableView>
#include <QMessageBox>
#include <QHeaderView>
#include <QPushButton>
#include <QtXml/QDomDocument>

#include "conf_delegation.h"
#include "qcustomplot.h"

class ConfNoLoadTest : public QWidget
{
    Q_OBJECT
public:
    explicit ConfNoLoadTest(QWidget *parent = 0);
    ~ConfNoLoadTest();

signals:
    void sendNetMsg(QByteArray dat);
    void buttonClicked(QByteArray win);
public slots:
    void readSettings();
    void saveSettings();
private slots:
    void initUI();
    QString appendXmlData(int column, QString name);
    void sequence(void);
    void ruler(double x);
    void wavePacket(double x1, double x2, QString name);
    void recvAppShow(QString win);
    QString CurrentSettings();
private:
    QTableView *view;
    StandardItem *model;

    QTableView *tView;
    StandardItem *tModel;

    QCheckBox *turnCheckBox;
    QComboBox *turnComboBox;

    QDomDocument doc;
    QDomElement root;
    QStringList itemNames;

    QCustomPlot *customplot;
};

#endif // CONFNOLOADTEST_H
