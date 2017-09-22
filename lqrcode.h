/*******************************************************************************
 * Copyright [2017]   <青岛艾普智能仪器有限公司>
 * All rights reserved.
 *
 * version:     0.1
 * author:      zhaonanlin
 * brief:       二维码生成界面
*******************************************************************************/
#ifndef QRCODEPAGE_H
#define QRCODEPAGE_H

#include <QWidget>
#include <QPainter>
#include <QPaintEvent>
#include <string>
#include "qrencode.h"

class LQRcode : public QWidget
{
    Q_OBJECT
public:
    explicit LQRcode(QWidget *parent = 0);
    ~LQRcode();

    void generateString(QString str);
    bool saveImage(QString fileName, int size = 250);
private:
    void draw(QPainter *painter, int width, int height);
    QString string;
    QRcode *qr;
protected:
    virtual void paintEvent(QPaintEvent *);
    virtual QSize sizeHint() const;
    virtual QSize minimumSizeHint() const;
};

#endif // QRCODEPAGE_H
