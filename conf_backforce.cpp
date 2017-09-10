/*******************************************************************************
 * Copyright [2017]   <青岛艾普智能仪器有限公司>
 * All rights reserved.
 *
 * version:     0.1
 * author:      zhaonanlin
 * brief:       BEMF配置
*******************************************************************************/
#include "conf_backforce.h"

ConfBackEMFTest::ConfBackEMFTest(QWidget *parent) : QWidget(parent)
{
    initUI();
}

ConfBackEMFTest::~ConfBackEMFTest()
{
}

void ConfBackEMFTest::initSettings(QJsonObject obj)
{
    QStringList items = itemNames;
    items << "nonu";
    for (int i=0; i < items.size(); i++) {
        QStringList temp = obj.value(items.at(i)).toString().split(",");
        if (items.at(i) == "nonu") {
            nounSpinBox->setValue(temp.at(0).toInt());
        } else if (items.at(i) == "turn") {
            for (int t=0; t < temp.size(); t++) {
                if (temp.at(t) == "0")
                    mView->item(t, 5)->setText("顺时针");
                if (temp.at(t) == "1")
                    mView->item(t, 5)->setText("逆时针");
            }
        } else {
            for (int t=0; t < temp.size(); t++)
                mView->item(t, i)->setText(temp.at(t));
        }
    }
}

void ConfBackEMFTest::readSettings()
{
    QJsonObject obj;
    QStringList items = itemNames;
    for (int i=0; i < items.size(); i++) {
        QStringList temp;
        if (items.at(i) == "turn") {
            for (int t=0; t < mView->rowCount(); t++) {
                if (mView->item(t, i)->text() == "顺时针")
                    temp.append("0");
                else if (mView->item(t, i)->text() == "逆时针")
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
    QStringList temp;
    temp.clear();
    temp.append(nounSpinBox->text());
    obj.insert("noun", temp.join(","));

    QJsonObject array;
    array.insert("BEMF", obj);
    emit sendAppCmd(array);
}

int ConfBackEMFTest::readSpeed()
{
    return mView->item(0, 4)->text().toInt();
}

void ConfBackEMFTest::initUI()
{
    this->setObjectName("ConfBackForce");
    QStringList headers;
    headers << tr("电压下限") << tr("电压上限") << tr("反电势系数下限")<< tr("反电势系数上限")
            << tr("伺服转速") << tr("伺服转向") << tr("相位差下限") << tr("相位差上限")
            << tr("Vcc") << tr("测试时间");
    itemNames << "volt_min" << "volt_max" << "bemf_min" << "bemf_max"
              << "speed" << "turn" << "skewing_min" << "skewing_max"
              << "volt_vcc" << "time";
    mView = new StandardItem(1, headers.size());
    mView->setHorizontalHeaderLabels(headers);
    for (int i=0; i < 1; i++) {
        for (int j=0; j < headers.size(); j++) {
            mView->setData(mView->index(i, j), "");
        }
    }
    DDoubleSpinBox *voltage = new DDoubleSpinBox;
    voltage->setMininum(1);
    voltage->setMaxinum(500);
    voltage->setDecimals(1);
    DDoubleSpinBox *bemf = new DDoubleSpinBox;
    bemf->setMininum(1);
    bemf->setMaxinum(500);
    SpinBox *speed = new SpinBox;
    speed->setMaxinum(3000);
    QStringList turns;
    turns << "顺时针" << "逆时针";
    ComboBox *turn = new ComboBox;
    turn->setItemNames(turns);
    DDoubleSpinBox *skewing = new DDoubleSpinBox;
    skewing->setMininum(-400);
    skewing->setMaxinum(400);
    DDoubleSpinBox *vcc = new DDoubleSpinBox;
    vcc->setMininum(3);
    vcc->setMaxinum(15.5);
    DDoubleSpinBox *time = new DDoubleSpinBox;
    time->setMininum(1);
    time->setMaxinum(20);
    view = new QTableView(this);
    view->setModel(mView);
    view->setItemDelegateForColumn(0, voltage);
    view->setItemDelegateForColumn(1, voltage);
    view->setItemDelegateForColumn(2, bemf);
    view->setItemDelegateForColumn(3, bemf);
    view->setItemDelegateForColumn(4, speed);
    view->setItemDelegateForColumn(5, turn);
    view->setItemDelegateForColumn(6, skewing);
    view->setItemDelegateForColumn(7, skewing);
    view->setItemDelegateForColumn(8, vcc);
    view->setItemDelegateForColumn(9, time);
    view->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    view->verticalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    nounSpinBox = new QSpinBox(this);
    nounSpinBox->setMinimumSize(97, 35);
    nounSpinBox->setButtonSymbols(QAbstractSpinBox::NoButtons);
    nounSpinBox->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);

    QPushButton *btnSave = new QPushButton(this);
    btnSave->setText(tr("保存"));
    btnSave->setMinimumSize(97, 35);
    connect(btnSave, SIGNAL(clicked(bool)), this, SLOT(readSettings()));

    QPushButton *btnExit = new QPushButton(this);
    btnExit->setText(tr("退出"));
    btnExit->setMinimumSize(97, 35);
    connect(btnExit, SIGNAL(clicked(bool)), this, SLOT(back()));

    QHBoxLayout *btnLayout = new QHBoxLayout;
    btnLayout->addWidget(new QLabel("不平衡度", this));
    btnLayout->addWidget(nounSpinBox);
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
}

void ConfBackEMFTest::back()
{
    emit buttonClicked(NULL);
}
