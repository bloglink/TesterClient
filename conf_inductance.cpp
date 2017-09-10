/*******************************************************************************
 * Copyright [2017]   <青岛艾普智能仪器有限公司>
 * All rights reserved.
 *
 * version:     0.1
 * author:      zhaonanlin
 * brief:       电感配置
*******************************************************************************/
#include "conf_inductance.h"

ConfInductance::ConfInductance(QWidget *parent) : QWidget(parent)
{
    initUI();
}

ConfInductance::~ConfInductance()
{
}

void ConfInductance::initSettings(QJsonObject obj)
{
    QStringList items = itemNames;
    items << "mode" << "noun";
    for (int i=0; i < items.size(); i++) {
        QStringList temp = obj.value(items.at(i)).toString().split(",");
        if (items.at(i) == "test") {
            for (int t=0; t < temp.size(); t++) {
                if (temp.at(t) == "0")
                    mView->item(t, 0)->setCheckState(Qt::Unchecked);
                else
                    mView->item(t, 0)->setCheckState(Qt::Checked);
            }
        } else if (items.at(i) == "unit") {
            for (int t=0; t < temp.size(); t++) {
                if (temp.at(t) == "0")
                    mView->item(t, 3)->setText("uH");
                if (temp.at(t) == "1")
                    mView->item(t, 3)->setText("mH");
            }
        } else if (items.at(i) == "mode") {
            avrgSpinBox->setValue((temp.at(0)).toInt());
            freqComboBox->setCurrentIndex((temp.at(1)).toInt());
            connComboBox->setCurrentIndex((temp.at(2)).toInt());
            modeComboBox->setCurrentIndex((temp.at(3)).toInt());
        } else if (items.at(i) == "noun") {
            nounSpinBox->setValue(temp.at(0).toInt());
        } else {
            for (int t=0; t < temp.size(); t++)
                mView->item(t, i)->setText(temp.at(t));
        }
    }
}


void ConfInductance::readSettings()
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
        } else if (items.at(i) == "unit") {
            for (int t=0; t < mView->rowCount(); t++) {
                if (mView->item(t, i)->text() == "uH")
                    temp.append("0");
                else if (mView->item(t, i)->text() == "mH")
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
    temp.append(QString::number(avrgSpinBox->value()));
    temp.append(QString::number(freqComboBox->currentIndex()));
    temp.append(QString::number(connComboBox->currentIndex()));
    temp.append(QString::number(modeComboBox->currentIndex()));
    obj.insert("mode", temp.join(","));

    temp.clear();
    temp.append(nounSpinBox->text());
    obj.insert("noun", temp.join(","));

    QJsonObject array;
    array.insert("IND", obj);
    emit sendAppCmd(array);
}

void ConfInductance::initUI()
{
    this->setObjectName("ConfInductance");
    QStringList headers;
    headers << tr("电感") << tr("端一") << tr("端二") << tr("电感单位")
            << tr("电感下限") << tr("电感上限")
            << tr("Q值下限") << tr("Q值上限") << tr("标准电感")
            << tr("线路补偿1") << tr("线路补偿2");
    itemNames << "test" << "port1" << "port2" << "unit"
              << "min" << "max" << "qmin" << "qmax" << "std"
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
    ComboBox *comboBox = new ComboBox;
    comboBox->setItemNames(names);
    QStringList units;
    units << tr("uH") << tr("mH");
    ComboBox *Unit = new ComboBox;
    Unit->setItemNames(units);
    DoubleSpinBox *doubleSpinBox = new DoubleSpinBox;
    doubleSpinBox->setMaxinum(9999.99);
    SpinBox *rateBox = new SpinBox;
    rateBox->setMaxinum(100);
    SpinBox *spinBox = new SpinBox;
    spinBox->setMaxinum(20);
    view = new QTableView(this);
    view->setModel(mView);
    view->setItemDelegateForColumn(1, comboBox);
    view->setItemDelegateForColumn(2, comboBox);
    view->setItemDelegateForColumn(3, Unit);
    view->setItemDelegateForColumn(4, doubleSpinBox);
    view->setItemDelegateForColumn(5, doubleSpinBox);
    view->setItemDelegateForColumn(6, rateBox);
    view->setItemDelegateForColumn(7, rateBox);
    view->setItemDelegateForColumn(8, doubleSpinBox);
    view->setItemDelegateForColumn(9, spinBox);
    view->setItemDelegateForColumn(10, spinBox);
    view->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    view->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Fixed);
    view->setColumnWidth(0, 50);
    view->verticalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    nounSpinBox = new QSpinBox(this);
    nounSpinBox->setMinimumSize(97, 35);
    nounSpinBox->setButtonSymbols(QAbstractSpinBox::NoButtons);
    nounSpinBox->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    avrgSpinBox = new QSpinBox(this);
    avrgSpinBox->setButtonSymbols(QAbstractSpinBox::NoButtons);
    avrgSpinBox->setMinimumSize(97, 35);

    freqComboBox = new QComboBox(this);
    freqComboBox->addItem("100");
    freqComboBox->addItem("120");
    freqComboBox->addItem("1k");
    freqComboBox->addItem("10k");
    freqComboBox->setView(new QListView);
    freqComboBox->setMinimumSize(97, 35);

    connComboBox = new QComboBox(this);
    connComboBox->addItem("串联");
    connComboBox->addItem("并联");
    connComboBox->setView(new QListView);
    connComboBox->setMinimumSize(97, 35);

    modeComboBox = new QComboBox(this);
    modeComboBox->addItem("快测");
    modeComboBox->addItem("慢测");
    modeComboBox->setView(new QListView);
    modeComboBox->setMinimumSize(97, 35);

    QPushButton *btnSave = new QPushButton(this);
    btnSave->setText(tr("保存"));
    btnSave->setMinimumSize(97, 35);
    connect(btnSave, SIGNAL(clicked(bool)), this, SLOT(readSettings()));

    QPushButton *btnExit = new QPushButton(this);
    btnExit->setText(tr("退出"));
    btnExit->setMinimumSize(97, 35);
    connect(btnExit, SIGNAL(clicked(bool)), this, SLOT(back()));

    QHBoxLayout *btnLayout = new QHBoxLayout;
    btnLayout->addWidget(new QLabel(tr("不平衡度")));
    btnLayout->addWidget(nounSpinBox);
    btnLayout->addWidget(new QLabel(tr("平均次数")));
    btnLayout->addWidget(avrgSpinBox);
    btnLayout->addWidget(new QLabel(tr("频率")));
    btnLayout->addWidget(freqComboBox);
    btnLayout->addWidget(new QLabel(tr("连接方式")));
    btnLayout->addWidget(connComboBox);
    btnLayout->addWidget(modeComboBox);
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

void ConfInductance::autoInput()
{
    int r = view->currentIndex().row();
    int c = view->currentIndex().column();
    switch (c) {
    case 0x03:          // 电感单位
        if (view->currentIndex().row() != 0)
            return;
        for (int i=0; i < 8; i++)
            mView->item(i, c)->setText(mView->item(0, c)->text());
        break;
    case 0x04:
    case 0x05:
        if (mView->item(r, 4)->text().isEmpty() || mView->item(r, 5)->text().isEmpty())
            return;
        if (mView->item(r, 4)->text().toDouble() > mView->item(r, 5)->text().toDouble()) {
            QMessageBox::warning(this, "警告", tr("下限大于上限"), QMessageBox::Ok);
            mView->item(r, 4)->setText("0.00");
        }
        break;
    case 0x06:          // Q值下限
    case 0x07:          // Q值上限
        if (mView->item(r, 6)->text().isEmpty() || mView->item(r, 7)->text().isEmpty())
            return;
        if (mView->item(r, 6)->text().toDouble() > mView->item(r, 7)->text().toDouble()) {
            QMessageBox::warning(this, "警告", tr("下限大于上限"), QMessageBox::Ok);
            mView->item(r, 6)->setText("0");
        }
        break;
    default:
        break;
    }
}

void ConfInductance::back()
{
    emit buttonClicked(NULL);
}

