#ifndef CONFQRENCODE_H
#define CONFQRENCODE_H

#include <QWidget>
#include <QPainter>
#include "qrencode.h"

class ConfQrenCode : public QWidget
{
    Q_OBJECT
public:
    explicit ConfQrenCode(QWidget *parent = 0);
    ~ConfQrenCode();
    void generateString(QString str);
    bool saveImage(QString fileName, int size=250);
private:
    void draw(QPainter &painter, int width, int height);
    QString string;
    QRcode *qr;

protected:
    void paintEvent(QPaintEvent *);
    QSize sizeHint() const;
    QSize minimumSizeHint() const;
};

#endif // CONFQRENCODE_H
