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

#include <QFile>
#include <QDir>
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
#include <QSettings>
#include <QJsonObject>
#include <QMessageBox>

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
    void sendAppCmd(QJsonObject obj);
    void typeUpdate();
public slots:
    void initSettings(QJsonObject obj);
    void initSysItems(QJsonObject obj);
    void readSettings();
    void readSysItems();
    QStringList testItems();
private slots:
    void initUI();
    void clickButton();
    void clickView(QModelIndex index);
    void windowChange();
    void selectColor();
    void deleteItem();
    void autoPixmap(QString name);
    void recvAppShow(QString win);
    void appendType();
    void deleteType();
    void changeType();
    void updateType();
    QString CurrentSettings();
    QString CurrentUser();
    void goBack();
private:
    QTableView *view;
    StandardItem *mView;

    QTableView *pView;
    StandardItem *pModel;

    QLineEdit *typeLineEdit;
    QComboBox *typeComboBox;
    QComboBox *testComboBox;
    QLabel *typePixmap;

    QList<QPushButton *> buttons;
    QList<QPushButton *> colors;

    QFrame *btnFrame;
    QStringList btnNames;
    QStringList testItem;
    QStringList FileNames;
};

#endif // CONFPAGE_H
