/*******************************************************************************
 * Copyright [2017]   <青岛艾普智能仪器有限公司>
 * All rights reserved.
 *
 * version:     0.0.0.1
 * author:      zhaonanlin
 * brief:       二维码
*******************************************************************************/
#include "QRCodeTest.h"
#include "ui_QRCodeTest.h"

QRCodeTest::QRCodeTest(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::QRCodeTest)
{
    ui->setupUi(this);
}

QRCodeTest::~QRCodeTest()
{
    delete ui;
}
