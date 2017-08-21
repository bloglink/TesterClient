/*******************************************************************************
 * Copyright [2016]   <青岛艾普智能仪器有限公司>
 * All rights reserved.
 *
 * version:     2.1.0.170427
 * author:      zhaonanlin
 * brief:       端口输入模块
*******************************************************************************/
#ifndef PAGENUM_H
#define PAGENUM_H

#include <QDebug>
#include <QLayout>
#include <QWidget>
#include <QShowEvent>
#include <QPushButton>
#include <QButtonGroup>

namespace Ui {
class PageNum;
}

class PageNum : public QWidget
{
    Q_OBJECT

public:
    explicit PageNum(QWidget *parent = 0);
    ~PageNum();

private:
    Ui::PageNum *ui;

signals:
    void ItemChange(QString msg);
public slots:
    void InitButtons(QStringList t);
public:
    QString Output;
    QButtonGroup *btnGroup;
private slots:
    void ReadButtons(int id);
    virtual void showEvent(QShowEvent *);
};

#endif // PAGENUM_H
