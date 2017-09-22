/*******************************************************************************
 * Copyright [2017]   <青岛艾普智能仪器有限公司>
 * All rights reserved.
 *
 * version:     0.1
 * author:      zhaonanlin
 * brief:       二维码生成界面
*******************************************************************************/
#include "lqrcode.h"

LQRcode::LQRcode(QWidget *parent) : QWidget(parent)
{
    qr = NULL;
}

LQRcode::~LQRcode()
{
    if (qr != NULL)
        QRcode_free(qr);
}

void LQRcode::generateString(QString str)
{
    string = str;
    if (qr != NULL)
        QRcode_free(qr);

    qr = QRcode_encodeString(string.toStdString().c_str(),
                             1,
                             QR_ECLEVEL_L,
                             QR_MODE_8,
                             1);
    this->update();
}

bool LQRcode::saveImage(QString fileName, int size)
{
    if (size != 0 && !fileName.isEmpty()) {
        QImage image(size, size, QImage::Format_Mono);
        QPainter *painter = new QPainter(&image);
        QColor background(Qt::white);
        painter->setBrush(background);
        painter->setPen(Qt::NoPen);
        painter->drawRect(0, 0, size, size);
        if (qr != NULL)
            draw(painter, size, size);
        bool b = image.save(fileName);
        painter->end();
        return b;
    } else {
        return false;
    }
}

void LQRcode::draw(QPainter *painter, int width, int height)
{
    QColor foreground(Qt::black);
    painter->setBrush(foreground);
    const int qr_width = qr->width > 0 ? qr->width : 1;
    double scale_x = width / qr_width;
    double scale_y = height / qr_width;
    for (int y = 0; y < qr_width; y++) {
        for (int x = 0; x < qr_width; x++) {
            unsigned char b = qr->data[y * qr_width + x];
            if (b & 0x01) {
                QRectF r(x * scale_x, y * scale_y, scale_x, scale_y);
                painter->drawRects(&r, 1);
            }
        }
    }
}

void LQRcode::paintEvent(QPaintEvent *e)
{
    QPainter *painter = new QPainter(this);
    QColor background(Qt::white);
    painter->setBrush(background);
    painter->setPen(Qt::NoPen);
    painter->drawRect(0, 0, width(), height());
    if (qr != NULL)
        draw(painter, width(), height());
    painter->end();
    e->accept();
}

QSize LQRcode::sizeHint() const
{
    QSize s;
    if (qr != NULL) {
        int qr_width = qr->width > 0 ? qr->width : 1;
        s = QSize(qr_width * 4, qr_width * 4);
    } else {
        s = QSize(50, 50);
    }
    return s;
}

QSize LQRcode::minimumSizeHint() const
{
    QSize s;
    if (qr != NULL) {
        int qr_width = qr->width > 0 ? qr->width : 1;
        s = QSize(qr_width, qr_width);
    } else {
        s = QSize(50, 50);
    }
    return s;
}

