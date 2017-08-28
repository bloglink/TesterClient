/*******************************************************************************
 * Copyright [2017]   <青岛艾普智能仪器有限公司>
 * All rights reserved.
 *
 * version:     0.1
 * author:      zhaonanlin
 * brief:       型号管理
*******************************************************************************/
#ifndef CONFPAGE_H
#define CONFPAGE_H

#include <QWidget>
#include <QTableView>
#include <QHeaderView>
#include <QLayout>
#include <QCheckBox>
#include <QLineEdit>
#include <QComboBox>
#include <QPushButton>
#include <QDebug>
#include <QLabel>
#include <QGroupBox>
#include <QToolButton>
#include <QColorDialog>
#include <QPalette>
#include <QColor>
#include <QtXml/QDomDocument>

#include "conf_delegation.h"

class ConfPage : public QWidget
{
    Q_OBJECT
public:
    explicit ConfPage(QWidget *parent = 0);
    ~ConfPage();

signals:
    void sendNetMsg(QByteArray dat);
    void buttonClicked(QByteArray win);
public:
    void initTypes(QString dat);
    void initOther(QString dat);
    QStringList testItems();
private slots:
    void initUI();
    void saveData();
    void saveSys();
    void clickButton();
    void showButtons();
    void windowChange();
    void selectColor();
    void deleteItem();
    void autoPixmap(QString name);
    void recvAppShow(QString win);
    void appendType();
    void deleteType();
    void updateType();
private:
    QTableView *view;
    StandardItem *mView;

    QTableView *test;
    StandardItem *mTest;

    QTableView *pView;
    StandardItem *pModel;

    QLineEdit *typeLineEdit;
    QComboBox *typeComboBox;
    QLabel *typePixmap;

    QList<QPushButton *> buttons;
    QList<QPushButton *> colors;

    QFrame *btnFrame;
    QToolButton *btnHide;

    QStringList btnNames;

    QStringList testItem;
};

#endif // CONFPAGE_H
