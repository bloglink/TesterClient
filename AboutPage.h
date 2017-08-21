/*******************************************************************************
 * Copyright [2017]   <青岛艾普智能仪器有限公司>
 * All rights reserved.
 *
 * version:     0.1
 * author:      zhaonanlin
 * brief:       关于界面
*******************************************************************************/
#ifndef CWINABOUT_H
#define CWINABOUT_H

#include <QWidget>
#include <QLabel>
#include <QLayout>
#include <QMargins>

class AboutPage : public QWidget
{
    Q_OBJECT
public:
    explicit AboutPage(QWidget *parent = 0);

signals:

public slots:
    void initUI();
};

#endif // CWINABOUT_H
