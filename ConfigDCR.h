/*******************************************************************************
 * Copyright [2017]   <青岛艾普智能仪器有限公司>
 * All rights reserved.
 *
 * version:     0.0.0.1
 * author:      zhaonanlin
 * brief:       电阻配置
*******************************************************************************/
#ifndef CONFIGDCR_H
#define CONFIGDCR_H

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
#include <QLabel>

#include "delegate.h"

class ConfigDCR : public QWidget
{
    Q_OBJECT

public:
    explicit ConfigDCR(QWidget *parent = 0);
    ~ConfigDCR();
signals:
    void sendNetMsg(QByteArray dat);
    void buttonClicked(QByteArray win);
public slots:
    void initData(QString dat);
private slots:
    void initUI();
    void saveData();
    void autoInput();
    void autoCalculate();
    void appendXmlData(int column, QString name);
    void recvAppShow(QString win);
private:
    QTableView *view;
    ItemModel *model;

    QLineEdit *nounLineEdit;
    QCheckBox *compensation;
    QLineEdit *tempLineEdit;

    QDomDocument doc;
    QDomElement root;
};

#endif // CONFIGDCR_H
