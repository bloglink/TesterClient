/*******************************************************************************
 * Copyright [2017]   <青岛艾普智能仪器有限公司>
 * All rights reserved.
 *
 * version:     0.1
 * author:      zhaonanlin
 * brief:       交耐配置
*******************************************************************************/
#include "conf_current_ac.h"

ConfCurrent_AC::ConfCurrent_AC(QWidget *parent) : QWidget(parent)
{
    initUI();
}

ConfCurrent_AC::~ConfCurrent_AC()
{
}

void ConfCurrent_AC::initSettings(QJsonObject obj)
{
    QStringList items = itemNames;
    for (int i=0; i < items.size(); i++) {
        QStringList temp = obj.value(items.at(i)).toString().split(",");
        if (items.at(i) == "test") {
            for (int t=0; t < temp.size(); t++) {
                if (temp.at(t) == "0")
                    mView->item(t, 0)->setCheckState(Qt::Unchecked);
                else
                    mView->item(t, 0)->setCheckState(Qt::Checked);
            }
        } else if (items.at(i) == "freq") {
            for (int t=0; t < temp.size(); t++) {
                if (temp.at(t) == "0")
                    mView->item(t, 7)->setText("50");
                else
                    mView->item(t, 7)->setText("60");
            }
        } else {
            for (int t=0; t < temp.size(); t++)
                mView->item(t, i)->setText(temp.at(t));
        }
    }
}


void ConfCurrent_AC::readSettings()
{
    QJsonObject obj;
    QStringList items = itemNames;
    for (int i=0; i < items.size(); i++) {
        QStringList temp;
        if (items.at(i) == "test") {
            for (int t=0; t < mView->rowCount(); t++) {
                if (mView->item(t, 0)->checkState() == Qt::Unchecked)
                    temp.append("0");
                else
                    temp.append("1");
            }
        } else if (items.at(i) == "freq") {
            for (int t=0; t < mView->rowCount(); t++) {
                if (mView->item(t, 7)->text() == "50")
                    temp.append("0");
                else
                    temp.append("1");
            }
            obj.insert(items.at(i), temp.join(","));
        } else {
            for (int t=0; t < mView->rowCount(); t++) {
                double x = mView->item(t, i)->text().toDouble();
                temp.append(QString::number(x));
            }
        }
        obj.insert(items.at(i), temp.join(","));
    }

    QJsonObject array;
    array.insert("ACW", obj);
    emit sendAppCmd(array);
}

void ConfCurrent_AC::initUI()
{
    this->setObjectName("ConfCurrent_AC");
    QStringList headers;
    headers << tr("交耐") << tr("高端") << tr("低端") << tr("电压")
            << tr("电流下限") << tr("电流上限") << tr("时间") << tr("频率")
            << tr("ARC");
    itemNames << "test" << "port1" << "port2" << "volt"
              << "min" << "max" << "time" << "freq" << "arc";
    mView = new StandardItem(5, headers.size());
    mView->setHorizontalHeaderLabels(headers);
    for (int i=0; i < 5; i++) {
        for (int j=0; j < headers.size(); j++) {
            mView->setData(mView->index(i, j), "");
        }
        mView->item(i, 0)->setCheckable(true);
    }
    DSpinBox *voltage = new DSpinBox;
    voltage->setMaxinum(3000);
    voltage->setMininum(500);
    DoubleSpinBox *currentSpinBox = new DoubleSpinBox;
    currentSpinBox->setMaxinum(25);
    DoubleSpinBox *doubleSpinBox = new DoubleSpinBox;
    doubleSpinBox->setMaxinum(999.9);
    QStringList items;
    items << "50" << "60";
    ComboBox *freq = new ComboBox;
    freq->setItemNames(items);
    DSpinBox *arc = new DSpinBox;
    arc->setMaxinum(9);
    view = new QTableView(this);
    view->setModel(mView);
    view->setItemDelegateForColumn(3, voltage);
    view->setItemDelegateForColumn(4, currentSpinBox);
    view->setItemDelegateForColumn(5, currentSpinBox);
    view->setItemDelegateForColumn(6, doubleSpinBox);
    view->setItemDelegateForColumn(7, freq);
    view->setItemDelegateForColumn(8, arc);
    view->verticalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    view->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    QPushButton *btnSave = new QPushButton(this);
    btnSave->setText(tr("保存"));
    btnSave->setMinimumSize(97, 35);
    connect(btnSave, SIGNAL(clicked(bool)), this, SLOT(readSettings()));

    QPushButton *btnExit = new QPushButton(this);
    btnExit->setText(tr("退出"));
    btnExit->setMinimumSize(97, 35);
    connect(btnExit, SIGNAL(clicked(bool)), this, SLOT(back()));

    QHBoxLayout *btnLayout = new QHBoxLayout;
    btnLayout->addStretch();
    btnLayout->addWidget(btnSave);
    btnLayout->addWidget(btnExit);

    QVBoxLayout *layout = new QVBoxLayout;
    layout->addStretch();
    layout->addStretch();
    layout->addWidget(view);
    layout->addLayout(btnLayout);
    layout->addStretch();
    layout->addStretch();
    this->setLayout(layout);

    view->hideColumn(0);
    view->hideColumn(1);
    view->hideColumn(2);
    view->hideRow(0);
    view->hideRow(1);
    view->hideRow(2);
    view->hideRow(3);
}

void ConfCurrent_AC::back()
{
    emit buttonClicked(NULL);
}

