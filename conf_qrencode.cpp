/*******************************************************************************
 * Copyright [2017]   <青岛艾普智能仪器有限公司>
 * All rights reserved.
 *
 * version:     0.1
 * author:      zhaonanlin
 * brief:       二维码生成
*******************************************************************************/
#include "conf_qrencode.h"

ConfQrenCode::ConfQrenCode(QWidget *parent) : QWidget(parent)
{
    qr = NULL;
}

ConfQrenCode::~ConfQrenCode()
{
    if (qr != NULL)
        QRcode_free(qr);
}

void ConfQrenCode::generateString(QString str)
{
    string = str;
    if (qr != NULL)
        QRcode_free(qr);
    qr = QRcode_encodeString(string.toStdString().c_str(),
                             1,
                             QR_ECLEVEL_L,
                             QR_MODE_8,
                             1);
    update();
}

bool ConfQrenCode::saveImage(QString fileName, int size)
{
    if (size != 0 && !fileName.isEmpty()) {
        QImage image(size, size, QImage::Format_Mono);
        QPainter painter(&image);
        QColor background(Qt::white);
        painter.setBrush(background);
        painter.setPen(Qt::NoPen);
        painter.drawRect(0, 0, size, size);
        if (qr != NULL)
            draw(painter, size, size);
        return image.save(fileName);
    } else {
        return false;
    }
}

void ConfQrenCode::draw(QPainter &painter, int width, int height)
{
    QColor foreground(Qt::black);
    painter.setBrush(foreground);
    const int qr_width = qr->width > 0 ? qr->width : 1;
    double scale_x = width / qr_width;
    double scale_y = height / qr_width;
    for (int y = 0; y < qr_width; y++) {
        for (int x = 0; x < qr_width; x++) {
            unsigned char b = qr->data[y * qr_width + x];
            if (b & 0x01) {
                QRectF r(x * scale_x, y * scale_y, scale_x, scale_y);
                painter.drawRects(&r, 1);
            }
        }
    }
}

void ConfQrenCode::paintEvent(QPaintEvent *e)
{
    QPainter painter(this);
    QColor background(Qt::white);
    painter.setBrush(background);
    painter.setPen(Qt::NoPen);
    painter.drawRect(0, 0, width(), height());
    if (qr != NULL)
        draw(painter, width(), height());
    e->accept();
}

QSize ConfQrenCode::sizeHint() const
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

QSize ConfQrenCode::minimumSizeHint() const
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

