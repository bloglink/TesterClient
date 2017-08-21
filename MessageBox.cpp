/*******************************************************************************
 * Copyright [2016]   <  青岛艾普智能仪器有限公司>
 * All rights reserved.
 *
 * version:     2.1.0.170427
 * author:      zhaonanlin
 * brief:       暂停询问模块
*******************************************************************************/
#include <QLabel>
#include <QPushButton>
#include <QMessageBox>
#include <QCheckBox>
#include <QHBoxLayout>
#include <QEvent>
#include <QApplication>
#include <QDebug>
#include "MessageBox.h"

PopupBox::PopupBox(QWidget  *parent, const QString &title, const QString &text,
                               QMessageBox::StandardButtons buttons,
                               QMessageBox::StandardButton defaultButton)
    : QDialog(parent, Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint)
{
    setWindowFlags(Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint);
    this->setStyleSheet("QDialog{border:2px solid red;}");

    setWindowIcon(QIcon(":/source/link.png"));
    setWindowTitle(title);
    setMinimumSize(300, 130);

    ButtonBox = new QDialogButtonBox(this);
    ButtonBox->setStandardButtons(QDialogButtonBox::StandardButtons(int(buttons)) |
                                  QDialogButtonBox::StandardButtons(int(defaultButton)));

    QLabel *IconLabel = new QLabel(this);
    QLabel *TextLabel = new QLabel(this);

    QPixmap pixmap(":/source/logo.png");
    IconLabel->setPixmap(pixmap);
    IconLabel->setFixedSize(113, 57);
    IconLabel->setScaledContents(true);

    TextLabel->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
    TextLabel->setStyleSheet("color:yellow;font:15pt");
    TextLabel->setWordWrap(true);
    TextLabel->setText(text);

    QGridLayout *GridLayout = new QGridLayout(this);
    GridLayout->addWidget(IconLabel, 0, 0, 2, 1, Qt::AlignTop);
    GridLayout->addWidget(TextLabel, 0, 1, 2, 1);
    GridLayout->addWidget(ButtonBox, GridLayout->rowCount(), 0, 1, GridLayout->columnCount());

    GridLayout->setSizeConstraint(QLayout::SetNoConstraint);
    GridLayout->setHorizontalSpacing(10);
    GridLayout->setVerticalSpacing(10);
    GridLayout->setContentsMargins(10, 10, 10, 10);
    this->setLayout(GridLayout);

    TranslateButtonText();

    connect(ButtonBox, SIGNAL(clicked(QAbstractButton*)), this,
            SLOT(onButtonClicked(QAbstractButton*)));
}

PopupBox::~PopupBox()
{
}

void PopupBox::changeEvent(QEvent *event)
{
    switch (event->type()) {
    case QEvent::LanguageChange:
        TranslateButtonText();
        break;
    default:
        QDialog::changeEvent(event);
        break;
    }
}

void PopupBox::TranslateButtonText()
{
    QPushButton *RetryButton = ButtonBox->button(QDialogButtonBox::Retry);
    if (RetryButton != NULL)
        RetryButton->setText(tr("重测"));

    QPushButton *CancelButton = ButtonBox->button(QDialogButtonBox::Yes);
    if (CancelButton != NULL)
        CancelButton->setText(tr("确定"));
    QPushButton *OkButton = ButtonBox->button(QDialogButtonBox::Ok);
    if (OkButton != NULL)
        OkButton->setText(tr("继续"));
}

void PopupBox::ReadVariant(QVariant s)
{
    QVariantHash hash = s.toHash();
    if (hash.value("TxAddress") != "WinHome")
        return;
    if (hash.value("TxCommand") != "BoxButton")
        return;
    if (hash.value("TxMessage") == "Retry")
        onButtonClicked(ButtonBox->button(QDialogButtonBox::Retry));
    else
        onButtonClicked(ButtonBox->button(QDialogButtonBox::Ok));
}

void PopupBox::onButtonClicked(QAbstractButton *button)
{
    if (!ButtonBox->buttons().contains(button))
        return;
    done(ButtonBox->standardButton(button));
}
