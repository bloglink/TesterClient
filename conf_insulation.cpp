/*******************************************************************************
 * Copyright [2017]   <青岛艾普智能仪器有限公司>
 * All rights reserved.
 *
 * version:     0.1
 * author:      zhaonanlin
 * brief:       绝缘配置
*******************************************************************************/
#include "conf_insulation.h"

ConfInsulation::ConfInsulation(QWidget *parent) : QWidget(parent)
{
    initUI();
}

ConfInsulation::~ConfInsulation()
{
}

void ConfInsulation::initUI()
{
    this->setObjectName("ConfInsulation");
    QStringList headers;
    headers << tr("交耐") << tr("高端") << tr("低端") << tr("电压")
            << tr("电阻下限") << tr("电阻上限") << tr("时间");
    itemNames << "test" << "port1" << "port2" << "volt"
              << "min" << "max" << "time";
    model = new StandardItem(5, headers.size());
    model->setHorizontalHeaderLabels(headers);
    for (int i=0; i < 5; i++) {
        for (int j=0; j < headers.size(); j++) {
            model->setData(model->index(i, j), "");
        }
        model->item(i, 0)->setCheckable(true);
    }
    QStringList items;
    items << "500" << "1000";
    ComboBox *voltage = new ComboBox;
    voltage->setItemNames(items);
    SpinBox *spinBox = new SpinBox;
    spinBox->setMaxinum(500);
    DoubleSpinBox *doubleSpinBox = new DoubleSpinBox;
    doubleSpinBox->setMaxinum(99.99);
    view = new QTableView(this);
    view->setModel(model);
    view->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    view->verticalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    view->setItemDelegateForColumn(3, voltage);
    view->setItemDelegateForColumn(4, spinBox);
    view->setItemDelegateForColumn(5, spinBox);
    view->setItemDelegateForColumn(6, doubleSpinBox);

    QPushButton *btnExit = new QPushButton(this);
    btnExit->setText(tr("保存退出"));
    btnExit->setMinimumSize(97, 35);
    connect(btnExit, SIGNAL(clicked(bool)), this, SLOT(saveSettings()));

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
    view->hideRow(0);
    view->hideRow(1);
    view->hideRow(2);
    view->hideRow(3);
}

void ConfInsulation::readSettings()
{
    //当前使用的测试项目
    QString t = QString("./config/%1.ini").arg(CurrentSettings());
    QSettings *ini = new QSettings(t, QSettings::IniFormat);
    ini->setIniCodec("GB18030");
    ini->beginGroup("SetInr");

    QStringList items = itemNames;

    for (int i=0; i < items.size(); i++) {
        QStringList temp = ini->value(items.at(i), "0,0,0,0,0").toString().split(",");
        switch (i) {
        case 0:
            for (int t=0; t < temp.size(); t++) {
                if (temp.at(t) == "1")
                    model->item(t, 0)->setCheckState(Qt::Checked);
                else
                    model->item(t, 0)->setCheckState(Qt::Unchecked);
            }
            break;
        case 3:
            for (int t=0; t < temp.size(); t++) {
                if (temp.at(t) == "0")
                    model->item(t, 3)->setText("500");
                else
                    model->item(t, 3)->setText("1000");
            }
            break;
        default:
            for (int t=0; t < temp.size(); t++)
                model->item(t, i)->setText(temp.at(t));
            break;
        }
    }
}

void ConfInsulation::saveSettings()
{
    emit sendNetMsg("6004 IR");
    //当前使用的测试项目
    QString t = QString("./config/%1.ini").arg(CurrentSettings());
    QSettings *ini = new QSettings(t, QSettings::IniFormat);
    ini->setIniCodec("GB18030");
    ini->beginGroup("SetInr");

    doc.clear();
    root.clear();
    root = doc.createElement("IR");
    doc.appendChild(root);

    for (int i=0; i < itemNames.size(); i++)
        ini->setValue(itemNames.at(i), appendXmlData(i, itemNames.at(i)));

    emit sendNetMsg(doc.toByteArray().insert(0, "6002 "));
    emit buttonClicked(NULL);
}

QString ConfInsulation::appendXmlData(int column, QString name)
{
    QStringList temp;
    for (int i=0; i < 5; i++) {
        if (column == 0) {
            if (model->item(i, column)->checkState() == Qt::Unchecked)
                temp.append("0");
            else
                temp.append("1");
        } else if (column == 3) {
            if (model->item(i, 3)->text() == "500")
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
    return temp.join(",");
}

void ConfInsulation::recvAppShow(QString win)
{
    if (win != this->objectName())
        return;
    readSettings();
}

QString ConfInsulation::CurrentSettings()
{
    QSettings *ini = new QSettings("./nandflash/global.ini", QSettings::IniFormat);
    QString n = ini->value("/GLOBAL/FileInUse", "Base_File").toString();
    return n.remove(".ini");
}
