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

    nounLineEdit = new QLineEdit(this);
    nounLineEdit->setMinimumSize(97, 35);
    nounLineEdit->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    tempLineEdit = new QLineEdit(this);
    tempLineEdit->setMinimumSize(97, 35);
    tempLineEdit->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    compensation = new QCheckBox(this);
    compensation->setText(tr("温度补偿"));

    QPushButton *btnExit = new QPushButton(this);
    btnExit->setText(tr("保存退出"));
    btnExit->setMinimumSize(97, 35);
    connect(btnExit, SIGNAL(clicked(bool)), this, SLOT(saveData()));

    QHBoxLayout *btnLayout = new QHBoxLayout;
    btnLayout->addWidget(compensation);
    btnLayout->addWidget(new QLabel("标准温度", this));
    btnLayout->addWidget(tempLineEdit);
    btnLayout->addWidget(new QLabel("不平衡度", this));
    btnLayout->addWidget(nounLineEdit);
    btnLayout->addStretch();
    btnLayout->addWidget(btnExit);

    QVBoxLayout *layout = new QVBoxLayout;
    layout->addStretch();
    layout->addWidget(view);
    layout->addLayout(btnLayout);
    layout->addStretch();
    this->setLayout(layout);
}

void ConfResistance::initData(QString dat)
{
    QDomDocument docs;
    docs.setContent(dat);
    if (docs.elementsByTagName("DCR").isEmpty())
        return;
    QStringList items = itemNames;
    items << "std_temp" << "temp_comp" << "noun";
    QDomNodeList list = docs.elementsByTagName("DCR").at(0).childNodes();
    for (int i=0; i < list.size(); i++) {
        QDomElement dom = list.at(i).toElement();
        QStringList temp = dom.text().split(",");
        int index = items.indexOf(dom.nodeName());
        if (index == -1)
            continue;
        switch (index) {
        case 0:
            for (int t=0; t < temp.size(); t++) {
                if (temp.at(t) == "0")
                    model->item(t, 0)->setCheckState(Qt::Unchecked);
                else
                    model->item(t, 0)->setCheckState(Qt::Checked);
            }
            break;
        case 3:
            for (int i=0; i < temp.size(); i++) {
                if (temp.at(i) == "0")
                    model->item(i, 3)->setText("铜");
                if (temp.at(i) == "1")
                    model->item(i, 3)->setText("铝");
                if (temp.at(i) == "2")
                    model->item(i, 3)->setText("铜铝");
            }
            break;
        case 4:
            for (int i=0; i < temp.size(); i++) {
                if (temp.at(i) == "0")
                    model->item(i, 4)->setText("mΩ");
                if (temp.at(i) == "1")
                    model->item(i, 4)->setText("Ω");
                if (temp.at(i) == "2")
                    model->item(i, 4)->setText("kΩ");
            }
            break;
        case 10:
            tempLineEdit->setText(temp.at(0));
            break;
        case 11:
            if (temp.at(0) == "1")
                compensation->setChecked(true);
            else
                compensation->setChecked(false);
            break;
        case 12:
            nounLineEdit->setText(temp.at(0));
            break;
        default:
            for (int t=0; t < temp.size(); t++)
                model->item(t, index)->setText(temp.at(t));
            break;
        }
    }
}

void ConfResistance::saveData()
{
    QDomText text;
    doc.clear();
    root.clear();
    root = doc.createElement("DCR");
    doc.appendChild(root);

    for (int i=0; i < itemNames.size(); i++)
        appendXmlData(i, itemNames.at(i));

    QStringList temp;
    if (compensation->isChecked())
        temp.append("1");
    else
        temp.append("0");
    QDomElement temp_comp = doc.createElement("temp_comp");
    root.appendChild(temp_comp);
    text = doc.createTextNode(temp.join(","));
    temp_comp.appendChild(text);

    temp.clear();
    temp.append(tempLineEdit->text());
    QDomElement std_temp = doc.createElement("std_temp");
    root.appendChild(std_temp);
    text = doc.createTextNode(temp.join(","));
    std_temp.appendChild(text);

    temp.clear();
    temp.append(nounLineEdit->text());
    QDomElement noun = doc.createElement("noun");
    root.appendChild(noun);
    text = doc.createTextNode(temp.join(","));
    noun.appendChild(text);

    emit sendNetMsg((doc.toByteArray()).insert(0, "6002 "));
    emit buttonClicked(NULL);
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

void ConfResistance::appendXmlData(int column, QString name)
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
}

void ConfResistance::recvAppShow(QString win)
{
    if (win != this->objectName())
        return;
    emit sendNetMsg("6004 DCR");
}
