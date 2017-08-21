/*******************************************************************************
 * Copyright [2017]   <青岛艾普智能仪器有限公司>
 * All rights reserved.
 *
 * version:     0.0.0.1
 * author:      zhaonanlin
 * brief:       开机主页
*******************************************************************************/
#ifndef HOMEPAGE_H
#define HOMEPAGE_H

#include <QWidget>
#include <QLabel>
#include <QLayout>
#include <QMargins>
#include <QTextBrowser>
#include <QGroupBox>
#include <QPushButton>

class HomePage : public QWidget
{
    Q_OBJECT
public:
    explicit HomePage(QWidget *parent = 0);
    ~HomePage();

signals:
    void buttonClicked(QByteArray win);
private slots:
    void initUI();
    void clickButton();
private:
    QTextBrowser *text;
};

#endif // HOMEPAGE_H
