/*******************************************************************************
 * Copyright [2017]   <青岛艾普智能仪器有限公司>
 * All rights reserved.
 *
 * version:     0.0.0.1
 * author:      zhaonanlin
 * brief:       开机主页
*******************************************************************************/
#include "HomePage.h"

HomePage::HomePage(QWidget *parent) : QWidget(parent)
{
    initUI();
}

HomePage::~HomePage()
{
}

void HomePage::initUI()
{
    this->setObjectName("HomePage");
    QString style = ".QFrame{border-image:url(:/source/CoolBlack.bmp)}";
    this->setStyleSheet(style);

    QPushButton *btn_syst = new QPushButton(this);
    btn_syst->setText("系统设置");
    btn_syst->setMinimumHeight(65);
    btn_syst->setObjectName("SystPage");
    connect(btn_syst, SIGNAL(clicked(bool)), this, SLOT(clickButton()));

    QPushButton *btn_conf = new QPushButton(this);
    btn_conf->setText("型号管理");
    btn_conf->setMinimumHeight(65);
    btn_conf->setObjectName("ConfPage");
    connect(btn_conf, SIGNAL(clicked(bool)), this, SLOT(clickButton()));

    QPushButton *btn_data = new QPushButton(this);
    btn_data->setText("数据管理");
    btn_data->setMinimumHeight(65);
    btn_data->setObjectName("DataPage");
    connect(btn_data, SIGNAL(clicked(bool)), this, SLOT(clickButton()));

    QPushButton *btn_test = new QPushButton(this);
    btn_test->setText("进入测试");
    btn_test->setMinimumHeight(65);
    btn_test->setObjectName("TestPage");
    connect(btn_test, SIGNAL(clicked(bool)), this, SLOT(clickButton()));

    QHBoxLayout *btn_layout = new QHBoxLayout;
    btn_layout->addStretch();
    btn_layout->addWidget(btn_syst);
    btn_layout->addWidget(btn_conf);
    btn_layout->addWidget(btn_data);
    btn_layout->addWidget(btn_test);
    btn_layout->addStretch();
    for (int i=0; i < btn_layout->count(); i++)
        btn_layout->setStretch(i, 1);

    QVBoxLayout *layout = new QVBoxLayout;
    layout->addStretch();
    layout->addLayout(btn_layout);
    layout->addStretch();
    layout->setStretch(0, 1);

    QFrame *frame = new QFrame(this);
    frame->setLayout(layout);

    QVBoxLayout* main_layout = new QVBoxLayout;
    main_layout->addWidget(frame);

    this->setLayout(main_layout);
}

void HomePage::clickButton()
{
    emit buttonClicked(QObject::sender()->objectName().toUtf8());
}

