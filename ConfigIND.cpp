/*******************************************************************************
 * Copyright [2017]   <青岛艾普智能仪器有限公司>
 * All rights reserved.
 *
 * version:     0.0.0.1
 * author:      zhaonanlin
 * brief:       电感配置
*******************************************************************************/
#include "ConfigIND.h"

ConfigIND::ConfigIND(QWidget *parent) : QWidget(parent)
{
    initUI();
}

ConfigIND::~ConfigIND()
{
}

void ConfigIND::initUI()
{
    this->setObjectName("ConfigIND");
    QStringList headers;
    headers << tr("电感") << tr("端一") << tr("端二") << tr("电感单位")
            << tr("电感下限") << tr("电感上限")
            << tr("Q值下限") << tr("Q值上限") << tr("标准电感")
            << tr("标准下限") << tr("标准上限") << tr("线路补偿1") << tr("线路补偿2");
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

    QStringList units;
    units << tr("uH") << tr("mH") << tr("H");
    ComboBox *Unit = new ComboBox;
    Unit->setItemNames(units);
    view->setItemDelegateForColumn(3, Unit);

    SpinBox *spinBox = new SpinBox;
    view->setItemDelegateForColumn(6, spinBox);
    view->setItemDelegateForColumn(7, spinBox);
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
    avrSpinBox = new QSpinBox(this);
    avrSpinBox->setButtonSymbols(QAbstractSpinBox::NoButtons);
    avrSpinBox->setMinimumSize(97, 35);

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

    QPushButton *btnAuto = new QPushButton(this);
    btnAuto->setText(tr("自动计算"));
    btnAuto->setMinimumSize(97, 35);
    connect(btnAuto, SIGNAL(clicked(bool)), this, SLOT(autoCalculate()));
    QPushButton *btnExit = new QPushButton(this);
    btnExit->setText(tr("保存退出"));
    btnExit->setMinimumSize(97, 35);
    connect(btnExit, SIGNAL(clicked(bool)), this, SLOT(saveData()));

    QHBoxLayout *btnLayout = new QHBoxLayout;
    btnLayout->addWidget(nounCheckBox);
    btnLayout->addWidget(nounLineEdit);
    btnLayout->addWidget(nounComboBox);
    btnLayout->addWidget(new QLabel(tr("平均次数")));
    btnLayout->addWidget(avrSpinBox);
    btnLayout->addWidget(new QLabel(tr("频率")));
    btnLayout->addWidget(freqComboBox);
    btnLayout->addWidget(new QLabel(tr("连接方式")));
    btnLayout->addWidget(connComboBox);
    btnLayout->addWidget(modeComboBox);
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

void ConfigIND::initData(QByteArray dat)
{
    QDomDocument doc;
    doc.setContent(dat);
    qDebug() << doc.toByteArray();
}

void ConfigIND::saveData()
{
    QDomText text;
    QDomDocument doc;
    QDomElement root = doc.createElement("IND");
    doc.appendChild(root);

    QStringList temp;
    for (int i=0; i < 8; i++) {
        if (model->item(i, 0)->checkState() == Qt::Unchecked)
            temp.append("0");
        else
            temp.append("1");
    }
    QDomElement test = doc.createElement("test");
    root.appendChild(test);
    text = doc.createTextNode(temp.join(", "));
    test.appendChild(text);

    temp.clear();
    for (int i=0; i < 8; i++)
        temp.append(model->item(i, 1)->text());

    QDomElement port1 = doc.createElement("port1");
    root.appendChild(port1);
    text = doc.createTextNode(temp.join(", "));
    port1.appendChild(text);

    temp.clear();
    for (int i=0; i < 8; i++)
        temp.append(model->item(i, 2)->text());
    QDomElement port2 = doc.createElement("port2");
    root.appendChild(port2);
    text = doc.createTextNode(temp.join(", "));
    port2.appendChild(text);

    temp.clear();
    for (int i=0; i < 8; i++)
        temp.append(model->item(i, 3)->text());
    QDomElement unit = doc.createElement("unit");
    root.appendChild(unit);
    text = doc.createTextNode(temp.join(", "));
    unit.appendChild(text);

    temp.clear();
    for (int i=0; i < 8; i++)
        temp.append(model->item(i, 4)->text());
    QDomElement min = doc.createElement("min");
    root.appendChild(min);
    text = doc.createTextNode(temp.join(", "));
    min.appendChild(text);

    temp.clear();
    for (int i=0; i < 8; i++)
        temp.append(model->item(i, 5)->text());
    QDomElement max = doc.createElement("max");
    root.appendChild(max);
    text = doc.createTextNode(temp.join(", "));
    max.appendChild(text);

    temp.clear();
    for (int i=0; i < 8; i++)
        temp.append(model->item(i, 6)->text());
    QDomElement qmax = doc.createElement("qmax");
    root.appendChild(qmax);
    text = doc.createTextNode(temp.join(", "));
    qmax.appendChild(text);

    temp.clear();
    for (int i=0; i < 8; i++)
        temp.append(model->item(i, 7)->text());
    QDomElement qmin = doc.createElement("qmin");
    root.appendChild(qmin);
    text = doc.createTextNode(temp.join(", "));
    qmin.appendChild(text);

    temp.clear();
    for (int i=0; i < 8; i++)
        temp.append(model->item(i, 8)->text());
    QDomElement std = doc.createElement("std");
    root.appendChild(std);
    text = doc.createTextNode(temp.join(", "));
    std.appendChild(text);

    temp.clear();
    for (int i=0; i < 8; i++)
        temp.append(model->item(i, 9)->text());
    QDomElement std_min = doc.createElement("std_min");
    root.appendChild(std_min);
    text = doc.createTextNode(temp.join(", "));
    std_min.appendChild(text);

    temp.clear();
    for (int i=0; i < 8; i++)
        temp.append(model->item(i, 10)->text());
    QDomElement std_max = doc.createElement("std_max");
    root.appendChild(std_max);
    text = doc.createTextNode(temp.join(", "));
    std_max.appendChild(text);

    temp.clear();
    for (int i=0; i < 8; i++)
        temp.append(model->item(i, 11)->text());
    QDomElement comp1 = doc.createElement("comp1");
    root.appendChild(comp1);
    text = doc.createTextNode(temp.join(", "));
    comp1.appendChild(text);

    temp.clear();
    for (int i=0; i < 8; i++)
        temp.append(model->item(i, 12)->text());
    QDomElement comp2 = doc.createElement("comp2");
    root.appendChild(comp2);
    text = doc.createTextNode(temp.join(", "));
    comp2.appendChild(text);

    temp.clear();
    temp.append(QString::number(avrSpinBox->value()));
    temp.append(QString::number(freqComboBox->currentIndex()));
    temp.append(QString::number(connComboBox->currentIndex()));
    temp.append(QString::number(modeComboBox->currentIndex()));
    QDomElement temp_comp = doc.createElement("mode");
    root.appendChild(temp_comp);
    text = doc.createTextNode(temp.join(", "));
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
    text = doc.createTextNode(temp.join(", "));
    noun.appendChild(text);

    emit saveConfig(doc.toByteArray());
    emit buttonClicked(NULL);
}

void ConfigIND::autoInput()
{
    if (view->currentIndex().row() != 0)
        return;
    int c = view->currentIndex().column();
    switch (c) {
    case 0x03:          // 电感单位
    case 0x06:          // Q值下限
    case 0x07:          // Q值上限
    case 0x09:          // 标准下限
    case 0x0A:          // 标准上限
        for (int i=0; i < 8; i++) {
            model->item(i, c)->setText(model->item(0, c)->text());
        }
        break;
    default:
        break;
    }
}

void ConfigIND::autoCalculate()
{
    for (int i=0; i < 8; i++) {
        double rstd = model->item(i, 8)->text().toDouble();  //标准电阻
        double rmax = model->item(i, 9)->text().toDouble();  //标准上限
        double rmin = model->item(i, 10)->text().toDouble();  //标准下限
        model->item(i, 4)->setText(QString("%1").arg(rstd*(100-rmin)/100, 0, 'g', 3));
        model->item(i, 5)->setText(QString("%1").arg(rstd*(100+rmax)/100, 0, 'g', 3));
    }
}

