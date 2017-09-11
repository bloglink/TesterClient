/*******************************************************************************
 * Copyright [2017]   <青岛艾普智能仪器有限公司>
 * All rights reserved.
 *
 * version:     0.1
 * author:      zhaonanlin
 * brief:       电阻配置
*******************************************************************************/
#include "conf_resistance.h"

ConfResistance::ConfResistance(QWidget *parent) : QWidget(parent)
{
    initUI();
}

ConfResistance::~ConfResistance()
{
}

void ConfResistance::initSettings(QJsonObject obj)
{
    QStringList items = itemNames;
    items << "std_temp" << "temp_comp" << "noun" << "time";
    for (int i=0; i < items.size(); i++) {
        QStringList temp = obj.value(items.at(i)).toString().split(",");
        if (items.at(i) == "test") {
            for (int t=0; t < temp.size(); t++) {
                if (temp.at(t) == "0")
                    mView->item(t, 0)->setCheckState(Qt::Unchecked);
                else
                    mView->item(t, 0)->setCheckState(Qt::Checked);
            }
        } else if (items.at(i) == "wire") {
            for (int t=0; t < temp.size(); t++) {
                if (temp.at(t) == "0")
                    mView->item(t, 3)->setText("铜");
                if (temp.at(t) == "1")
                    mView->item(t, 3)->setText("铝");
                if (temp.at(t) == "2")
                    mView->item(t, 3)->setText("铜铝");
            }
        } else if (items.at(i) == "unit") {
            for (int t=0; t < temp.size(); t++) {
                if (temp.at(t) == "0")
                    mView->item(t, 4)->setText("mΩ");
                if (temp.at(t) == "1")
                    mView->item(t, 4)->setText("Ω");
                if (temp.at(t) == "2")
                    mView->item(t, 4)->setText("kΩ");
            }
        } else if (items.at(i) == "std_temp") {
            tempSpinBox->setValue(temp.at(0).toInt());
        } else if (items.at(i) == "temp_comp") {
            if (temp.at(0) == "0")
                compensation->setChecked(true);
            else
                compensation->setChecked(false);
        } else if (items.at(i) == "noun") {
            nounSpinBox->setValue(temp.at(0).toInt());
        } else if (items.at(i) == "time") {
            timeBox->setValue(temp.at(0).toDouble());
        } else {
            for (int t=0; t < temp.size(); t++)
                mView->item(t, i)->setText(temp.at(t));
        }
    }
}

void ConfResistance::readSettings()
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
        } else if (items.at(i) == "wire") {
            for (int t=0; t < mView->rowCount(); t++) {
                if (mView->item(t, i)->text() == "铜")
                    temp.append("0");
                else if (mView->item(t, i)->text() == "铝")
                    temp.append("1");
                else if (mView->item(t, i)->text() == "铜铝")
                    temp.append("2");
            }
            obj.insert(items.at(i), temp.join(","));
        } else if (items.at(i) == "unit") {
            for (int t=0; t < mView->rowCount(); t++) {
                if (mView->item(t, i)->text() == "mΩ")
                    temp.append("0");
                else if (mView->item(t, i)->text() == "Ω")
                    temp.append("1");
                else if (mView->item(t, i)->text() == "kΩ")
                    temp.append("2");
            }
        } else {
            for (int t=0; t < mView->rowCount(); t++) {
                double x = mView->item(t, i)->text().toDouble();
                temp.append(QString::number(x));
            }
        }
        obj.insert(items.at(i), temp.join(","));
    }
    QStringList temp;
    if (compensation->isChecked())
        temp.append("0");
    else
        temp.append("1");
    obj.insert("temp_comp", temp.join(","));

    temp.clear();
    temp.append(tempSpinBox->text());
    obj.insert("std_temp", temp.join(","));

    temp.clear();
    temp.append(nounSpinBox->text());
    obj.insert("noun", temp.join(","));

    temp.clear();
    temp.append(QString::number(timeBox->value()));
    obj.insert("time", temp.join(","));

    QJsonObject array;
    array.insert("DCR", obj);
    emit sendAppCmd(array);
}

void ConfResistance::initUI()
{
    this->setObjectName("ConfResistance");
    QStringList headers;
    headers << tr("电阻") << tr("端一") << tr("端二") << tr("线圈材料")
            << tr("电阻单位") << tr("电阻下限") << tr("电阻上限") << tr("标准电阻")
            << tr("线路补偿1") << tr("线路补偿2");
    itemNames << "test" << "port1" << "port2" << "wire" << "unit" << "min" << "max" << "std"
              << "wire_comp1" << "wire_comp2";
    mView = new StandardItem(8, headers.size());
    mView->setHorizontalHeaderLabels(headers);
    for (int i=0; i < 8; i++) {
        for (int j=0; j < headers.size(); j++) {
            mView->setData(mView->index(i, j), "");
        }
        mView->item(i, 0)->setCheckable(true);
    }
    connect(mView, SIGNAL(dataChanged(QModelIndex, QModelIndex, QVector<int>)),
            this, SLOT(autoInput()));
    QStringList names;
    names << "1" << "2" << "3" << "4" << "5" << "6" << "7" << "8";
    DComboBox *comboBox = new DComboBox;
    comboBox->setItemNames(names);
    QStringList wires;
    wires << tr("铜") << tr("铝") << tr("铜铝") << tr("热敏电阻");
    DComboBox *Wire = new DComboBox;
    Wire->setItemNames(wires);
    QStringList units;
    units << tr("mΩ") << tr("Ω") << tr("kΩ");
    DComboBox *Unit = new DComboBox;
    Unit->setItemNames(units);
    DDoubleSpinBox *doubleSpinBox = new DDoubleSpinBox;
    doubleSpinBox->setMaxinum(9999.99);
    doubleSpinBox->setDecimals(1);
    DSpinBox *spinBox = new DSpinBox;
    spinBox->setMaxinum(20);

    view = new QTableView(this);
    view->setModel(mView);
    view->setItemDelegateForColumn(1, comboBox);
    view->setItemDelegateForColumn(2, comboBox);
    view->setItemDelegateForColumn(3, Wire);
    view->setItemDelegateForColumn(4, Unit);
    view->setItemDelegateForColumn(5, doubleSpinBox);
    view->setItemDelegateForColumn(6, doubleSpinBox);
    view->setItemDelegateForColumn(7, doubleSpinBox);
    view->setItemDelegateForColumn(8, spinBox);
    view->setItemDelegateForColumn(9, spinBox);
    view->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    view->verticalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    nounSpinBox = new QSpinBox(this);
    nounSpinBox->setMinimumSize(97, 35);
    nounSpinBox->setButtonSymbols(QAbstractSpinBox::NoButtons);
    nounSpinBox->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    tempSpinBox = new QSpinBox(this);
    tempSpinBox->setMinimumSize(97, 35);
    tempSpinBox->setButtonSymbols(QAbstractSpinBox::NoButtons);
    tempSpinBox->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    compensation = new QCheckBox(this);
    compensation->setText(tr("温度补偿"));
    timeBox = new QDoubleSpinBox(this);
    timeBox->setMinimumSize(97, 35);
    timeBox->setButtonSymbols(QAbstractSpinBox::NoButtons);
    timeBox->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);

    QPushButton *btnSave = new QPushButton(this);
    btnSave->setText(tr("保存"));
    btnSave->setMinimumSize(97, 35);
    connect(btnSave, SIGNAL(clicked(bool)), this, SLOT(readSettings()));

    QPushButton *btnExit = new QPushButton(this);
    btnExit->setText(tr("退出"));
    btnExit->setMinimumSize(97, 35);
    connect(btnExit, SIGNAL(clicked(bool)), this, SLOT(back()));

    QHBoxLayout *btnLayout = new QHBoxLayout;
    btnLayout->addWidget(compensation);
    btnLayout->addWidget(new QLabel("标准温度", this));
    btnLayout->addWidget(tempSpinBox);
    btnLayout->addWidget(new QLabel("不平衡度", this));
    btnLayout->addWidget(nounSpinBox);
    btnLayout->addWidget(new QLabel("测试时间", this));
    btnLayout->addWidget(timeBox);
    btnLayout->addStretch();
    btnLayout->addWidget(btnSave);
    btnLayout->addWidget(btnExit);

    QVBoxLayout *layout = new QVBoxLayout;
    layout->addStretch();
    layout->addWidget(view);
    layout->addLayout(btnLayout);
    layout->addStretch();
    this->setLayout(layout);
}

void ConfResistance::autoInput()
{
    int r = view->currentIndex().row();
    int c = view->currentIndex().column();
    switch (c) {
    case 0x03:  //线圈材料
    case 0x04:  //电阻单位
        if (r != 0)
            return;
        for (int i=0; i < 8; i++)
            mView->item(i, c)->setText(mView->item(0, c)->text());
        break;
    case 0x05:
    case 0x06:
        if (mView->item(r, 5)->text().isEmpty() || mView->item(r, 6)->text().isEmpty())
            return;
        if (mView->item(r, 5)->text().toDouble() > mView->item(r, 6)->text().toDouble()) {
            QMessageBox::warning(this, "警告", tr("下限大于上限"), QMessageBox::Ok);
            mView->item(r, 5)->setText("0.00");
        }
        break;
    default:
        break;
    }
}

void ConfResistance::back()
{
    emit buttonClicked(NULL);
}

