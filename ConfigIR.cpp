/*******************************************************************************
 * Copyright [2017]   <青岛艾普智能仪器有限公司>
 * All rights reserved.
 *
 * version:     0.0.0.1
 * author:      zhaonanlin
 * brief:       绝缘配置
*******************************************************************************/
#include "ConfigIR.h"

ConfigIR::ConfigIR(QWidget *parent) : QWidget(parent)
{
    initUI();
}

ConfigIR::~ConfigIR()
{
}

void ConfigIR::initUI()
{
    this->setObjectName("ConfigIR");
    QStringList headers;
    headers << tr("交耐") << tr("高端") << tr("低端") << tr("电压")
            << tr("电阻下限") << tr("电阻上限") << tr("时间") << tr("补偿1") << tr("补偿2");
    model = new ItemModel(4, headers.size());
    model->setHorizontalHeaderLabels(headers);
    for (int i=0; i < 4; i++) {
        for (int j=0; j < headers.size(); j++) {
            model->setData(model->index(i, j), "");
        }
        model->item(i, 0)->setCheckable(true);
    }
    view = new QTableView(this);
    view->setModel(model);

    view->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    view->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Fixed);
    view->setColumnWidth(0, 50);
    view->verticalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    QPushButton *btnExit = new QPushButton(this);
    btnExit->setText(tr("保存退出"));
    btnExit->setMinimumSize(97, 35);
    connect(btnExit, SIGNAL(clicked(bool)), this, SLOT(saveData()));

    QHBoxLayout *btnLayout = new QHBoxLayout;
    btnLayout->addStretch();
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
    view->hideRow(1);
    view->hideRow(2);
    view->hideRow(3);
}

void ConfigIR::initData(QByteArray dat)
{
    QDomDocument doc;
    doc.setContent(dat);
    qDebug() << doc.toByteArray();
}

void ConfigIR::saveData()
{
    QDomText text;
    QDomDocument doc;
    QDomElement root = doc.createElement("IR");
    doc.appendChild(root);

    QStringList temp;
    for (int i=0; i < 4; i++) {
        if (model->item(i, 0)->checkState() == Qt::Unchecked)
            temp.append("0");
        else
            temp.append("1");
    }
    QDomElement test = doc.createElement("test");
    root.appendChild(test);
    text = doc.createTextNode(temp.join(","));
    test.appendChild(text);

    temp.clear();
    for (int i=0; i < 4; i++)
        temp.append(model->item(i, 1)->text());
    QDomElement port1 = doc.createElement("port1");
    root.appendChild(port1);
    text = doc.createTextNode(temp.join(","));
    port1.appendChild(text);

    temp.clear();
    for (int i=0; i < 4; i++)
        temp.append(model->item(i, 2)->text());
    QDomElement port2 = doc.createElement("port2");
    root.appendChild(port2);
    text = doc.createTextNode(temp.join(","));
    port2.appendChild(text);

    temp.clear();
    for (int i=0; i < 4; i++)
        temp.append(model->item(i, 3)->text());
    QDomElement volt = doc.createElement("volt");
    root.appendChild(volt);
    text = doc.createTextNode(temp.join(","));
    volt.appendChild(text);

    temp.clear();
    for (int i=0; i < 4; i++)
        temp.append(model->item(i, 4)->text());
    QDomElement min = doc.createElement("min");
    root.appendChild(min);
    text = doc.createTextNode(temp.join(","));
    min.appendChild(text);

    temp.clear();
    for (int i=0; i < 4; i++)
        temp.append(model->item(i, 5)->text());
    QDomElement max = doc.createElement("max");
    root.appendChild(max);
    text = doc.createTextNode(temp.join(","));
    max.appendChild(text);

    temp.clear();
    for (int i=0; i < 4; i++)
        temp.append(model->item(i, 6)->text());
    QDomElement time = doc.createElement("time");
    root.appendChild(time);
    text = doc.createTextNode(temp.join(","));
    time.appendChild(text);

    temp.clear();
    for (int i=0; i < 4; i++)
        temp.append(model->item(i, 7)->text());
    QDomElement comp1 = doc.createElement("comp1");
    root.appendChild(comp1);
    text = doc.createTextNode(temp.join(","));
    comp1.appendChild(text);

    temp.clear();
    for (int i=0; i < 4; i++)
        temp.append(model->item(i, 8)->text());
    QDomElement comp2 = doc.createElement("comp2");
    root.appendChild(comp2);
    text = doc.createTextNode(temp.join(","));
    comp2.appendChild(text);

    emit saveConfig(doc.toByteArray());
    emit buttonClicked(NULL);
}

