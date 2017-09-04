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

void ConfResistance::initUI()
{
    this->setObjectName("ConfResistance");
    QStringList headers;
    headers << tr("电阻") << tr("端一") << tr("端二") << tr("线圈材料")
            << tr("电阻单位") << tr("电阻下限") << tr("电阻上限") << tr("标准电阻")
            << tr("线路补偿1") << tr("线路补偿2");
    itemNames << "test" << "port1" << "port2" << "wire" << "unit" << "min" << "max" << "std"
              << "wire_comp1" << "wire_comp2";
    model = new StandardItem(8, headers.size());
    model->setHorizontalHeaderLabels(headers);
    for (int i=0; i < 8; i++) {
        for (int j=0; j < headers.size(); j++) {
            model->setData(model->index(i, j), "");
        }
        model->item(i, 0)->setCheckable(true);
    }
    connect(model, SIGNAL(dataChanged(QModelIndex, QModelIndex, QVector<int>)),
            this, SLOT(autoInput()));
    QStringList names;
    names << "1" << "2" << "3" << "4" << "5" << "6" << "7" << "8";
    ComboBox *comboBox = new ComboBox;
    comboBox->setItemNames(names);
    QStringList wires;
    wires << tr("铜") << tr("铝") << tr("铜铝") << tr("热敏电阻");
    ComboBox *Wire = new ComboBox;
    Wire->setItemNames(wires);
    QStringList units;
    units << tr("mΩ") << tr("Ω") << tr("kΩ");
    ComboBox *Unit = new ComboBox;
    Unit->setItemNames(units);
    DoubleSpinBox *doubleSpinBox = new DoubleSpinBox;
    doubleSpinBox->setMaxinum(9999.99);
    SpinBox *spinBox = new SpinBox;
    spinBox->setMaxinum(20);

    view = new QTableView(this);
    view->setModel(model);
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

    QPushButton *btnExit = new QPushButton(this);
    btnExit->setText(tr("保存退出"));
    btnExit->setMinimumSize(97, 35);
    connect(btnExit, SIGNAL(clicked(bool)), this, SLOT(saveSettings()));

    QHBoxLayout *btnLayout = new QHBoxLayout;
    btnLayout->addWidget(compensation);
    btnLayout->addWidget(new QLabel("标准温度", this));
    btnLayout->addWidget(tempSpinBox);
    btnLayout->addWidget(new QLabel("不平衡度", this));
    btnLayout->addWidget(nounSpinBox);
    btnLayout->addStretch();
    btnLayout->addWidget(btnExit);

    QVBoxLayout *layout = new QVBoxLayout;
    layout->addStretch();
    layout->addWidget(view);
    layout->addLayout(btnLayout);
    layout->addStretch();
    this->setLayout(layout);
}

void ConfResistance::saveSettings()
{
    emit sendNetMsg("6004 DCR");
    //当前使用的测试项目
    QString t = QString("./config/%1.ini").arg(CurrentSettings());
    QSettings *ini = new QSettings(t, QSettings::IniFormat);
    ini->setIniCodec("GB18030");
    ini->beginGroup("SetDcr");

    QDomText text;
    doc.clear();
    root.clear();
    root = doc.createElement("DCR");
    doc.appendChild(root);

    for (int i=0; i < itemNames.size(); i++) {
        ini->setValue(itemNames.at(i), appendXmlData(i, itemNames.at(i)));
    }

    QStringList temp;
    if (compensation->isChecked())
        temp.append("1");
    else
        temp.append("0");
    QDomElement temp_comp = doc.createElement("temp_comp");
    root.appendChild(temp_comp);
    text = doc.createTextNode(temp.join(","));
    temp_comp.appendChild(text);
    ini->setValue("temp_comp", temp.join(","));

    temp.clear();
    temp.append(tempSpinBox->text());
    QDomElement std_temp = doc.createElement("std_temp");
    root.appendChild(std_temp);
    text = doc.createTextNode(temp.join(","));
    std_temp.appendChild(text);
    ini->setValue("std_temp", temp.join(","));

    temp.clear();
    temp.append(nounSpinBox->text());
    QDomElement noun = doc.createElement("noun");
    root.appendChild(noun);
    text = doc.createTextNode(temp.join(","));
    noun.appendChild(text);
    ini->setValue("noun", temp.join(","));

    emit sendNetMsg((doc.toByteArray()).insert(0, "6002 "));
    emit buttonClicked(NULL);
}

void ConfResistance::readSettings()
{
    //当前使用的测试项目
    QString t = QString("./config/%1.ini").arg(CurrentSettings());
    QSettings *ini = new QSettings(t, QSettings::IniFormat);
    ini->setIniCodec("GB18030");
    ini->beginGroup("SetDcr");

    QStringList items = itemNames;
    items << "std_temp" << "temp_comp" << "noun";

    for (int i=0; i < items.size(); i++) {
        QStringList temp = ini->value(items.at(i), "0,0,0,0,0,0,0,0").toString().split(",");
        switch (i) {
        case 0:
            for (int t=0; t < temp.size(); t++) {
                if (temp.at(t) == "0")
                    model->item(t, 0)->setCheckState(Qt::Unchecked);
                else
                    model->item(t, 0)->setCheckState(Qt::Checked);
            }
            break;
        case 3:
            for (int t=0; t < temp.size(); t++) {
                if (temp.at(t) == "0")
                    model->item(t, 3)->setText("铜");
                if (temp.at(t) == "1")
                    model->item(t, 3)->setText("铝");
                if (temp.at(t) == "2")
                    model->item(t, 3)->setText("铜铝");
            }
            break;
        case 4:
            for (int t=0; t < temp.size(); t++) {
                if (temp.at(t) == "0")
                    model->item(t, 4)->setText("mΩ");
                if (temp.at(t) == "1")
                    model->item(t, 4)->setText("Ω");
                if (temp.at(t) == "2")
                    model->item(t, 4)->setText("kΩ");
            }
            break;
        case 10:
            tempSpinBox->setValue(temp.at(0).toInt());
            break;
        case 11:
            if (temp.at(0) == "1")
                compensation->setChecked(true);
            else
                compensation->setChecked(false);
            break;
        case 12:
            nounSpinBox->setValue(temp.at(0).toInt());
            break;
        default:
            for (int t=0; t < temp.size(); t++)
                model->item(t, i)->setText(temp.at(t));
            break;
        }
    }
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
            model->item(i, c)->setText(model->item(0, c)->text());
        break;
    case 0x05:
    case 0x06:
        if (model->item(r, 5)->text().isEmpty() || model->item(r, 6)->text().isEmpty())
            return;
        if (model->item(r, 5)->text().toDouble() > model->item(r, 6)->text().toDouble()) {
            QMessageBox::warning(this, "警告", tr("下限大于上限"), QMessageBox::Ok);
            model->item(r, 5)->setText("0.00");
        }
        break;
    default:
        break;
    }
}

QString ConfResistance::appendXmlData(int column, QString name)
{
    QStringList temp;
    for (int i=0; i < 8; i++) {
        if (column == 0) {
            if (model->item(i, column)->checkState() == Qt::Unchecked)
                temp.append("0");
            else
                temp.append("1");
        } else if (column == 3) {
            if (model->item(i, column)->text() == "铜")
                temp.append("0");
            else if (model->item(i, column)->text() == "铝")
                temp.append("1");
            else if (model->item(i, column)->text() == "铜铝")
                temp.append("2");
        } else if (column == 4) {
            if (model->item(i, column)->text() == "mΩ")
                temp.append("0");
            else if (model->item(i, column)->text() == "Ω")
                temp.append("1");
            else if (model->item(i, column)->text() == "kΩ")
                temp.append("2");
        } else {
            temp.append(model->item(i, column)->text());
        }
    }
    QDomText text = doc.createTextNode(temp.join(","));
    QDomElement xml = doc.createElement(name);
    xml.appendChild(text);
    root.appendChild(xml);
    return temp.join(",");
}

void ConfResistance::recvAppShow(QString win)
{
    if (win != this->objectName())
        return;
    readSettings();
}

QString ConfResistance::CurrentSettings()
{
    QSettings *ini = new QSettings("./nandflash/global.ini", QSettings::IniFormat);
    QString n = ini->value("/GLOBAL/FileInUse", "Base_File").toString();
    return n.remove(".ini");
}
