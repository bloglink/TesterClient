/*******************************************************************************
 * Copyright [2017]   <青岛艾普智能仪器有限公司>
 * All rights reserved.
 *
 * version:     0.0.0.1
 * author:      zhaonanlin
 * brief:       电阻配置
*******************************************************************************/
#include "ConfigDCR.h"

ConfigDCR::ConfigDCR(QWidget *parent) : QWidget(parent)
{
    initUI();
}

ConfigDCR::~ConfigDCR()
{
}

void ConfigDCR::initUI()
{
    this->setObjectName("ConfigDCR");
    QStringList headers;
    headers << tr("电阻") << tr("端一") << tr("端二") << tr("线圈材料")
            << tr("电阻单位") << tr("电阻下限") << tr("电阻上限") << tr("标准电阻")
            << tr("标准上限") << tr("标准下限") << tr("标准温度")
            << tr("线路补偿1") << tr("线路补偿2");
    model = new ItemModel(8, headers.size());
    model->setHorizontalHeaderLabels(headers);
    for (int i=0; i < 8; i++) {
        for (int j=0; j < headers.size(); j++) {
            model->setData(model->index(i, j), "");
        }
        model->item(i, 0)->setCheckable(true);
    }
    connect(model, SIGNAL(dataChanged(QModelIndex, QModelIndex, QVector<int>)),
            this, SLOT(autoInput()));

    view = new QTableView(this);
    view->setModel(model);

    view->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    view->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Fixed);
    view->setColumnWidth(0, 50);
    view->verticalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    QStringList names;
    names << "1" << "2" << "3" << "4" << "5" << "6" << "7" << "8";

    ComboBox *T1 = new ComboBox;
    T1->setItemNames(names);
    ComboBox *T2 = new ComboBox;
    T2->setItemNames(names);

    view->setItemDelegateForColumn(1, T1);
    view->setItemDelegateForColumn(2, T2);

    QStringList wires;
    wires << tr("铜") << tr("铝") << tr("铜铝") << tr("热敏电阻");
    ComboBox *Wire = new ComboBox;
    Wire->setItemNames(wires);
    view->setItemDelegateForColumn(3, Wire);

    QStringList units;
    units << tr("mΩ") << tr("Ω") << tr("kΩ");
    ComboBox *Unit = new ComboBox;
    Unit->setItemNames(units);
    view->setItemDelegateForColumn(4, Unit);

    SpinBox *spinBox = new SpinBox;
    view->setItemDelegateForColumn(8, spinBox);
    view->setItemDelegateForColumn(9, spinBox);
    view->setItemDelegateForColumn(10, spinBox);
    view->setItemDelegateForColumn(11, spinBox);
    view->setItemDelegateForColumn(12, spinBox);

    nounCheckBox = new QCheckBox(this);
    nounCheckBox->setText(tr("不平衡度"));
    nounComboBox = new QComboBox(this);
    nounComboBox->addItem(tr("三相模式"));
    nounComboBox->addItem(tr("相间模式"));
    nounComboBox->setMinimumSize(97, 35);
    nounComboBox->setView(new QListView);
    nounLineEdit = new QLineEdit(this);
    nounLineEdit->setMinimumSize(97, 35);
    nounLineEdit->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    compensation = new QCheckBox(this);
    compensation->setText(tr("温度补偿"));

    QPushButton *btnAuto = new QPushButton(this);
    btnAuto->setText(tr("自动计算"));
    btnAuto->setMinimumSize(97, 35);
    connect(btnAuto, SIGNAL(clicked(bool)), this, SLOT(autoCalculate()));
    QPushButton *btnExit = new QPushButton(this);
    btnExit->setText(tr("保存退出"));
    btnExit->setMinimumSize(97, 35);
    connect(btnExit, SIGNAL(clicked(bool)), this, SLOT(saveData()));

    QHBoxLayout *btnLayout = new QHBoxLayout;
    btnLayout->addWidget(compensation);
    btnLayout->addWidget(nounCheckBox);
    btnLayout->addWidget(nounLineEdit);
    btnLayout->addWidget(nounComboBox);
    btnLayout->addStretch();
    btnLayout->addWidget(btnAuto);
    btnLayout->addWidget(btnExit);

    QVBoxLayout *layout = new QVBoxLayout;
    layout->addStretch();
    layout->addWidget(view);
    layout->addLayout(btnLayout);
    layout->addStretch();
    this->setLayout(layout);
}

void ConfigDCR::initData(QByteArray dat)
{
    QDomDocument docs;
    docs.setContent(dat);
    if (docs.elementsByTagName("DCR").isEmpty())
        return;
    QDomNodeList list = docs.elementsByTagName("DCR").at(0).childNodes();
    for (int i=0; i < list.size(); i++) {
        QDomElement dom = list.at(i).toElement();
        QStringList temp = dom.text().split(",");
        if (dom.nodeName() == "test") {
            for (int i=0; i < temp.size(); i++) {
                if (temp.at(i) == "1")
                    model->item(i, 0)->setCheckState(Qt::Checked);
            }
        }
        if (dom.nodeName() == "port1")
            for (int i=0; i < temp.size(); i++)
                model->item(i, 1)->setText(temp.at(i));
        if (dom.nodeName() == "port2")
            for (int i=0; i < temp.size(); i++)
                model->item(i, 2)->setText(temp.at(i));
        if (dom.nodeName() == "unit")
            for (int i=0; i < temp.size(); i++)
                model->item(i, 3)->setText(temp.at(i));
        if (dom.nodeName() == "min")
            for (int i=0; i < temp.size(); i++)
                model->item(i, 4)->setText(temp.at(i));
        if (dom.nodeName() == "max")
            for (int i=0; i < temp.size(); i++)
                model->item(i, 5)->setText(temp.at(i));
        if (dom.nodeName() == "std")
            for (int i=0; i < temp.size(); i++)
                model->item(i, 6)->setText(temp.at(i));
        if (dom.nodeName() == "std_min")
            for (int i=0; i < temp.size(); i++)
                model->item(i, 7)->setText(temp.at(i));
        if (dom.nodeName() == "std_max")
            for (int i=0; i < temp.size(); i++)
                model->item(i, 8)->setText(temp.at(i));
        if (dom.nodeName() == "std_temp")
            for (int i=0; i < temp.size(); i++)
                model->item(i, 9)->setText(temp.at(i));
        if (dom.nodeName() == "wire_comp1")
            for (int i=0; i < temp.size(); i++)
                model->item(i, 10)->setText(temp.at(i));
        if (dom.nodeName() == "wire_comp2")
            for (int i=0; i < temp.size(); i++)
                model->item(i, 11)->setText(temp.at(i));
        if (dom.nodeName() == "temp_comp") {
            if (temp.at(0) == "1")
                nounCheckBox->setChecked(true);
            else
                nounCheckBox->setChecked(false);
        }
    }
}

void ConfigDCR::saveData()
{
    QDomText text;
    doc.clear();
    root.clear();
    root = doc.createElement("DCR");
    doc.appendChild(root);

    QStringList names;
    names << "test" << "port1" << "port2" << "unit" << "min" << "max" << "std"
          << "std_min" << "std_max" << "std_temp" << "wire_comp1" << "wire_comp2";

    for (int i=0; i < names.size(); i++)
        appendXmlData(i, names.at(i));

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
    if (nounCheckBox->isChecked())
        temp.append("1");
    else
        temp.append("0");
    temp.append(nounLineEdit->text());
    temp.append(QString::number(nounComboBox->currentIndex()));
    QDomElement noun = doc.createElement("noun");
    root.appendChild(noun);
    text = doc.createTextNode(temp.join(","));
    noun.appendChild(text);

    emit sendNetMsg((doc.toByteArray()).insert(0, "6002 "));
    emit buttonClicked(NULL);
    initData(doc.toByteArray());
}

void ConfigDCR::autoInput()
{
    if (view->currentIndex().row() != 0)
        return;
    int c = view->currentIndex().column();
    switch (c) {
    case 0x03:  //线圈材料
    case 0x04:  //电阻单位
    case 0x08:  //标准上限
    case 0x09:  //标准下限
    case 0x0A:  //标准温度
        for (int i=0; i < 8; i++) {
            model->item(i, c)->setText(model->item(0, c)->text());
        }
        break;
    default:
        break;
    }
}

void ConfigDCR::autoCalculate()
{
    for (int i=0; i < 8; i++) {
        double rstd = model->item(i, 7)->text().toDouble();  //标准电阻
        double rmax = model->item(i, 8)->text().toDouble();  //标准上限
        double rmin = model->item(i, 9)->text().toDouble();  //标准下限
        model->item(i, 5)->setText(QString("%1").arg(rstd*(100-rmin)/100,  0,  'g',  3));
        model->item(i, 6)->setText(QString("%1").arg(rstd*(100+rmax)/100,  0,  'g',  3));
    }
}

void ConfigDCR::appendXmlData(int column, QString name)
{
    QStringList temp;
    for (int i=0; i < 8; i++) {
        if (column == 0) {
            if (model->item(i, column)->checkState() == Qt::Unchecked)
                temp.append("0");
            else
                temp.append("1");
        } else {
            temp.append(model->item(i, column)->text());
        }
    }
    QDomText text = doc.createTextNode(temp.join(","));
    QDomElement xml = doc.createElement(name);
    xml.appendChild(text);
    root.appendChild(xml);
}

void ConfigDCR::recvAppShow(QString win)
{
    if (win != this->objectName())
        return;
    emit sendNetMsg("6004 DCR");
}
