/*******************************************************************************
 * Copyright [2017]   <青岛艾普智能仪器有限公司>
 * All rights reserved.
 *
 * version:     0.1
 * author:      zhaonanlin
 * brief:       电感配置
*******************************************************************************/
#ifndef CONFINDUCTANCE_H
#define CONFINDUCTANCE_H

#include <QDebug>
#include <QLabel>
#include <QWidget>
#include <QLayout>
#include <QLineEdit>
#include <QCheckBox>
#include <QTableView>
#include <QMessageBox>
#include <QHeaderView>
#include <QPushButton>
#include <QtXml/QDomDocument>

#include "conf_delegation.h"

class ConfInductance : public QWidget
{
    Q_OBJECT
public:
    explicit ConfInductance(QWidget *parent = 0);
    ~ConfInductance();

signals:
    void sendNetMsg(QByteArray dat);
    void buttonClicked(QByteArray win);
public slots:
    void initData(QString dat);
private slots:
    void initUI();
    void saveData();
    void autoInput();
    void appendXmlData(int column, QString name);
    void recvAppShow(QString win);
private:
    QTableView *view;
    StandardItem *model;

    QLineEdit *nounLineEdit;
    QSpinBox *avrSpinBox;
    QComboBox *freqComboBox;
    QComboBox *connComboBox;
    QComboBox *modeComboBox;

    QDomDocument doc;
    QDomElement root;
    QStringList itemNames;
};

#endif // CONFINDUCTANCE_H
