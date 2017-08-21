/*******************************************************************************
 * Copyright [2017]   <青岛艾普智能仪器有限公司>
 * All rights reserved.
 *
 * version:     0.0.0.1
 * author:      zhaonanlin
 * brief:       综合测试系统登录界面
*******************************************************************************/
#ifndef CLOGIN_H
#define CLOGIN_H

#include <QFile>
#include <QTimer>
#include <QDebug>
#include <QLabel>
#include <QLayout>
#include <QWidget>
#include <QDialog>
#include <QListView>
#include <QLineEdit>
#include <QComboBox>
#include <QGroupBox>
#include <QToolButton>
#include <QPushButton>
#include <QMessageBox>
#include <QSizePolicy>
#include <QApplication>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSettings>

#include <QSqlError>
#include <QJsonObject>
#include <QJsonDocument>

#define SAVE_MAX 5

class LoginPage : public QDialog
{
    Q_OBJECT
public:
    explicit LoginPage(QWidget *parent = 0);
    ~LoginPage();

signals:
    void sendJson(QJsonObject obj);
private slots:
    void initUI();
    void initData();
    void saveData();
    void login();
    void loginTimeOut();
    void recvNetMsg(QString msg);
private:
    QSqlDatabase db;
    QComboBox *svr;
    QComboBox *prt;
    QComboBox *usr;
    QLineEdit *pwd;
    QSettings *ini;
};

#endif // CLOGIN_H
