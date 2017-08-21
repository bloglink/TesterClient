/*******************************************************************************
 * Copyright [2016]   <青岛艾普智能仪器有限公司>
 * All rights reserved.
 *
 * version:     2.1.0.170427
 * author:      zhaonanlin
 * brief:       端口输入模块
*******************************************************************************/
#include "PageNum.h"
#include "ui_PageNum.h"

PageNum::PageNum(QWidget *parent) :
    QWidget(parent, Qt::Popup),
    ui(new Ui::PageNum)
{
    ui->setupUi(this);
}

PageNum::~PageNum()
{
    delete ui;
}
void PageNum::InitButtons(QStringList t)
{
    QGridLayout *layout = new QGridLayout;
    btnGroup = new QButtonGroup;
    for (int i=0; i < t.size(); i++) {
        btnGroup->addButton(new QPushButton(this), i);
        btnGroup->buttons().at(i)->setText(t.at(i));
        layout->addWidget(btnGroup->buttons().at(i), i/4, i%4);
        btnGroup->buttons().at(i)->setMinimumSize(97, 44);
    }
    btnGroup->addButton(new QPushButton(this), t.size());
    btnGroup->buttons().last()->setText(tr("返回"));
    btnGroup->buttons().last()->setMinimumSize(97, 44);
    layout->addWidget(btnGroup->buttons().last(), t.size()/4+1, 3);
    ui->BoxButton->setLayout(layout);
    connect(btnGroup, SIGNAL(buttonClicked(int)), this, SLOT(ReadButtons(int)));
}

void PageNum::ReadButtons(int id)
{
    if (id != btnGroup->buttons().size()-1)
        emit ItemChange(btnGroup->button(id)->text());
    this->hide();
}

void PageNum::showEvent(QShowEvent *e)
{
    QWidget *widget = this;
    while (widget->parentWidget() != NULL) {
        widget = widget->parentWidget();
    }
    int w = widget->width();
    int h = widget->height();
    int x = (w-480)/2;
    int y = (h-240)/2;
    this->move(x, y);
    e->accept();
}
