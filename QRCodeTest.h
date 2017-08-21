/*******************************************************************************
 * Copyright [2017]   <青岛艾普智能仪器有限公司>
 * All rights reserved.
 *
 * version:     0.0.0.1
 * author:      zhaonanlin
 * brief:       二维码
*******************************************************************************/
#ifndef QRCODETEST_H
#define QRCODETEST_H

#include <QWidget>

namespace Ui {
class QRCodeTest;
}

class QRCodeTest : public QWidget
{
    Q_OBJECT

public:
    explicit QRCodeTest(QWidget *parent = 0);
    ~QRCodeTest();

private:
    Ui::QRCodeTest *ui;
};

#endif // QRCODETEST_H
