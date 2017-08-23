/*******************************************************************************
 * Copyright [2017]   <青岛艾普智能仪器有限公司>
 * All rights reserved.
 *
 * version:     0.0.0.1
 * author:      zhaonanlin
 * brief:       电感配置
*******************************************************************************/
#ifndef CONFIGIND_H
#define CONFIGIND_H

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

class ConfigIND : public QWidget
{
    Q_OBJECT
public:
    explicit ConfigIND(QWidget *parent = 0);
    ~ConfigIND();

signals:
    void sendNetMsg(QByteArray dat);
    void buttonClicked(QByteArray win);
private slots:
    void initUI();
    void initData(QByteArray dat);
    void saveData();
    void autoInput();
    void autoCalculate();
    void recvAppShow(QString win);
private:
    QTableView *view;
    ItemModel *model;

    QCheckBox *nounCheckBox;
    QLineEdit *nounLineEdit;
    QComboBox *nounComboBox;
    QSpinBox *avrSpinBox;
    QComboBox *freqComboBox;
    QComboBox *connComboBox;
    QComboBox *modeComboBox;
};

#endif // CONFIGIND_H
