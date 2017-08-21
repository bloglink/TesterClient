/*******************************************************************************
 * Copyright [2017]   <青岛艾普智能仪器有限公司>
 * All rights reserved.
 *
 * version:     0.0.0.1
 * author:      zhaonanlin
 * brief:       功率配置
*******************************************************************************/
#include "ConfigPWR.h"

ConfigPWR::ConfigPWR(QWidget *parent) : QWidget(parent)
{
    initUI();
}

ConfigPWR::~ConfigPWR()
{
}

void ConfigPWR::initUI()
{
    this->setObjectName("ConfigPWR");
    QStringList headers;
    headers << tr("电压") << tr("电流下限") << tr("电流上限")
            << tr("功率下限") << tr("功率上限")
            << tr("转速下限") << tr("转速上限")
            << tr("Vcc电压") << tr("Vcc时间") << tr("Vcc缓升")
            << tr("Vm 电压") << tr("Vm 时间") << tr("Vm 缓升")
            << tr("Vsp电压") << tr("Vsp时间") << tr("Vsp缓升") << tr("测试时间");
    model = new ItemModel(1, headers.size());
    model->setHorizontalHeaderLabels(headers);
    for (int i=0; i < 1; i++) {
        for (int j=0; j < headers.size(); j++) {
            model->setData(model->index(i, j), "");
        }
    }

    view = new QTableView(this);
    view->setModel(model);

    view->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    view->verticalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    SpinBox *spinBox = new SpinBox;
    view->setItemDelegateForColumn(6, spinBox);
    view->setItemDelegateForColumn(7, spinBox);
    view->setItemDelegateForColumn(9, spinBox);
    view->setItemDelegateForColumn(10, spinBox);
    view->setItemDelegateForColumn(11, spinBox);
    view->setItemDelegateForColumn(12, spinBox);

    turnCheckBox = new QCheckBox(this);
    turnCheckBox->setText(tr("空载转向"));
    turnComboBox = new QComboBox(this);
    turnComboBox->addItem(tr("逆时针"));
    turnComboBox->addItem(tr("顺时针"));
    turnComboBox->setMinimumSize(97, 35);
    turnComboBox->setView(new QListView);

    QPushButton *btnExit = new QPushButton(this);
    btnExit->setText(tr("保存退出"));
    btnExit->setMinimumSize(97, 35);
    connect(btnExit, SIGNAL(clicked(bool)), this, SLOT(saveData()));

    QHBoxLayout *btnLayout = new QHBoxLayout;
    btnLayout->addWidget(turnCheckBox);
    btnLayout->addWidget(turnComboBox);
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
}

void ConfigPWR::initData(QByteArray dat)
{
    QDomDocument doc;
    doc.setContent(dat);
    qDebug() << doc.toByteArray();
}

void ConfigPWR::saveData()
{
    doc.clear();
    root.clear();
    root = doc.createElement("PWR");
    doc.appendChild(root);

    QStringList temp;
    temp << "volt" << "curr_min" << "curr_max"
         << "pwr_min" << "pwr_max"
         << "speed_min" << "speed_max"
         << "vcc_volt" << "vcc_time" << "vcc_up"
         << "vdc_volt" << "vdc_time" << "vdc_up"
         << "vsp_volt" << "vsp_time" << "vsp_up"
         << "time";
    for (int i=0; i < temp.size(); i++)
        appendXmlData(i, temp.at(i));
    emit saveConfig(doc.toByteArray());
    emit buttonClicked(NULL);
}

void ConfigPWR::appendXmlData(int column, QString name)
{
    QDomText text = doc.createTextNode(model->item(0, column)->text());
    QDomElement xml = doc.createElement(name);
    xml.appendChild(text);
    root.appendChild(xml);
}


