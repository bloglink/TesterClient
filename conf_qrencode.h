/*******************************************************************************
 * Copyright [2017]   <青岛艾普智能仪器有限公司>
 * All rights reserved.
 *
 * version:     0.1
 * author:      zhaonanlin
 * brief:       二维码生成
*******************************************************************************/
#ifndef CONFQRENCODE_H
#define CONFQRENCODE_H


#include <QWidget>
#include <QPainter>
#include <QPaintEvent>
#include "qrencode.h"
#include "string"

class ConfQrenCode : public QWidget
{
    Q_OBJECT
public:
    explicit ConfQrenCode(QWidget *parent = 0);
    ~ConfQrenCode();
    void generateString(QString str);
    bool saveImage(QString fileName, int size = 250);
private:
    void draw(QPainter &painter, int width, int height);
    QString string;
    QRcode *qr;

protected:
    virtual void paintEvent(QPaintEvent *e);
    QSize sizeHint() const;
    QSize minimumSizeHint() const;
};

#endif // CONFQRENCODE_H
