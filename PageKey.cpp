/*******************************************************************************
 * Copyright [2016]   <青岛艾普智能仪器有限公司>
 * All rights reserved.
 *
 * version:     2.1.0.170427
 * author:      zhaonanlin
 * brief:       按钮模块
*******************************************************************************/
#include "PageKey.h"
#include "ui_PageKey.h"

PageKey *PageKey::_instance = 0;

PageKey::PageKey(QWidget *parent) :
    QWidget(parent, Qt::Tool | Qt::WindowStaysOnTopHint | Qt::FramelessWindowHint),
    ui(new Ui::PageKey)
{
    ui->setupUi(this);
    InitWindows();
    InitButtons();
}

PageKey::~PageKey()
{
    delete ui;
}

void PageKey::InitWindows()
{
    //绑定全局改变焦点信号槽
    connect(qApp,  SIGNAL(focusChanged(QWidget *,  QWidget *)),
            this,  SLOT(focusChanged(QWidget *,  QWidget *)));
    //绑定按键事件过滤器
    qApp->installEventFilter(this);
    upper = true;
    currentWidget = NULL;
}

void PageKey::InitButtons()
{
    QButtonGroup *btn_group = new QButtonGroup;
    btn_group->addButton(ui->Key_0, Qt::Key_0);
    btn_group->addButton(ui->Key_1, Qt::Key_1);
    btn_group->addButton(ui->Key_2, Qt::Key_2);
    btn_group->addButton(ui->Key_3, Qt::Key_3);
    btn_group->addButton(ui->Key_4, Qt::Key_4);
    btn_group->addButton(ui->Key_5, Qt::Key_5);
    btn_group->addButton(ui->Key_6, Qt::Key_6);
    btn_group->addButton(ui->Key_7, Qt::Key_7);
    btn_group->addButton(ui->Key_8, Qt::Key_8);
    btn_group->addButton(ui->Key_9, Qt::Key_9);

    btn_group->addButton(ui->Key0, Qt::Key_0);
    btn_group->addButton(ui->Key1, Qt::Key_1);
    btn_group->addButton(ui->Key2, Qt::Key_2);
    btn_group->addButton(ui->Key3, Qt::Key_3);
    btn_group->addButton(ui->Key4, Qt::Key_4);
    btn_group->addButton(ui->Key5, Qt::Key_5);
    btn_group->addButton(ui->Key6, Qt::Key_6);
    btn_group->addButton(ui->Key7, Qt::Key_7);
    btn_group->addButton(ui->Key8, Qt::Key_8);
    btn_group->addButton(ui->Key9, Qt::Key_9);

    btn_group->addButton(ui->Key_A, Qt::Key_A);
    btn_group->addButton(ui->Key_B, Qt::Key_B);
    btn_group->addButton(ui->Key_C, Qt::Key_C);
    btn_group->addButton(ui->Key_D, Qt::Key_D);
    btn_group->addButton(ui->Key_E, Qt::Key_E);
    btn_group->addButton(ui->Key_F, Qt::Key_F);
    btn_group->addButton(ui->Key_G, Qt::Key_G);
    btn_group->addButton(ui->Key_H, Qt::Key_H);
    btn_group->addButton(ui->Key_I, Qt::Key_I);
    btn_group->addButton(ui->Key_J, Qt::Key_J);
    btn_group->addButton(ui->Key_K, Qt::Key_K);
    btn_group->addButton(ui->Key_L, Qt::Key_L);
    btn_group->addButton(ui->Key_M, Qt::Key_M);
    btn_group->addButton(ui->Key_N, Qt::Key_N);
    btn_group->addButton(ui->Key_O, Qt::Key_O);
    btn_group->addButton(ui->Key_P, Qt::Key_P);
    btn_group->addButton(ui->Key_Q, Qt::Key_Q);
    btn_group->addButton(ui->Key_R, Qt::Key_R);
    btn_group->addButton(ui->Key_S, Qt::Key_S);
    btn_group->addButton(ui->Key_T, Qt::Key_T);
    btn_group->addButton(ui->Key_U, Qt::Key_U);
    btn_group->addButton(ui->Key_V, Qt::Key_V);
    btn_group->addButton(ui->Key_W, Qt::Key_W);
    btn_group->addButton(ui->Key_X, Qt::Key_X);
    btn_group->addButton(ui->Key_Y, Qt::Key_Y);
    btn_group->addButton(ui->Key_Z, Qt::Key_Z);

    btn_group->addButton(ui->Key_Space, Qt::Key_Space);
    btn_group->addButton(ui->Key_Enter, Qt::Key_Enter);
    btn_group->addButton(ui->Key_Enter_2, Qt::Key_Enter);
    btn_group->addButton(ui->Key_Period, Qt::Key_Period);
    btn_group->addButton(ui->Key_Period_2, Qt::Key_Period);
    btn_group->addButton(ui->Key_CapsLock, Qt::Key_CapsLock);
    btn_group->addButton(ui->Key_Backspace, Qt::Key_Backspace);
    btn_group->addButton(ui->Key_Backspace_2, Qt::Key_Backspace);
    btn_group->addButton(ui->Key_Minus, Qt::Key_Minus);

    connect(btn_group, SIGNAL(buttonClicked(int)), this, SLOT(ReadButtons(int)));
    connect(ui->Key_Switch, SIGNAL(clicked(bool)), this, SLOT(SwitchInput()));
}

void PageKey::ReadButtons(int id)
{
    switch (id) {
    case Qt::Key_Enter :
        this->setVisible(false);
        QApplication::focusWidget()->clearFocus();
        break;
    case Qt::Key_CapsLock:
        upper = !upper;
        if (upper)
            ui->Key_CapsLock->setText(tr("大写"));
        else
            ui->Key_CapsLock->setText(tr("小写"));
        break;
    default:
#ifdef __arm__
        if (id >= Qt::Key_A && id <= Qt::Key_Z && !upper)
            id += 32;
        QWSServer::sendKeyEvent(id, id, Qt::NoModifier, true, false);
#endif
        break;
    }
}

void PageKey::SwitchInput()
{
    ui->stackedWidget->setCurrentIndex(0);
    this->move(70, this->pos().y());
    this->resize(660, 240);
}

void PageKey::focusChanged(QWidget *,  QWidget *nowWidget)
{
    if (nowWidget != 0 && !this->isAncestorOf(nowWidget)) {
        bool visible = false;
        if (nowWidget->inherits("QLineEdit")) {
            visible = true;
            num = false;
        }
        if (nowWidget->inherits("QSpinBox")
                || nowWidget->inherits("QDoubleSpinBox")
                || nowWidget->inherits("QDateTimeEdit")) {
            visible = true;
            num = true;
#ifdef __arm__
            QWSServer::sendKeyEvent(Qt::Key_A, Qt::Key_A, Qt::ControlModifier, true, false);
#endif
        }
        this->setVisible(visible);
        MoveWindows();
    }
}

void PageKey::MoveWindows()
{
    int mouse_y = QCursor::pos().y(); // 鼠标点击处纵坐标
    int x = 0;
    if (num) {
        x = 240;
        this->resize(320, 240);
        ui->stackedWidget->setCurrentIndex(1);
    } else {
        x = 70;
        this->resize(660, 240);
        ui->stackedWidget->setCurrentIndex(0);
    }
    if (mouse_y > 300) {
        this->move(x, mouse_y-280);
    } else {
        this->move(x, mouse_y+50);
    }
}
/*********************************END OF FILE**********************************/
