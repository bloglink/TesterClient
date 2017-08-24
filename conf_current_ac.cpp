/*******************************************************************************
 * Copyright [2017]   <青岛艾普智能仪器有限公司>
 * All rights reserved.
 *
 * version:     0.1
 * author:      zhaonanlin
 * brief:       交耐配置
*******************************************************************************/
#include "conf_current_ac.h"

ConfCurrent_AC::ConfCurrent_AC(QWidget *parent) : QWidget(parent)
{
    initUI();
}

ConfCurrent_AC::~ConfCurrent_AC()
{
}

void ConfCurrent_AC::initUI()
{
    this->setObjectName("ConfCurrent_AC");
    QStringList headers;
    headers << tr("交耐") << tr("高端") << tr("低端") << tr("电压")
            << tr("电流下限") << tr("电流上限") << tr("时间") << tr("频率")
            << tr("ARC");
    itemNames << "test" << "port1" << "port2" << "volt"
              << "min" << "max" << "time" << "freq" << "arc";
    model = new StandardItem(5, headers.size());
    model->setHorizontalHeaderLabels(headers);
    for (int i=0; i < 5; i++) {
        for (int j=0; j < headers.size(); j++) {
            model->setData(model->index(i, j), "");
        }
        model->item(i, 0)->setCheckable(true);
    }
    SpinBox *voltage = new SpinBox;
    voltage->setMaxinum(3000);
    DoubleSpinBox *currentSpinBox = new DoubleSpinBox;
    currentSpinBox->setMaxinum(25);
    DoubleSpinBox *doubleSpinBox = new DoubleSpinBox;
    doubleSpinBox->setMaxinum(99.99);
    QStringList items;
    items << "50" << "60";
    ComboBox *freq = new ComboBox;
    freq->setItemNames(items);
    SpinBox *arc = new SpinBox;
    arc->setMaxinum(9);
    view = new QTableView(this);
    view->setModel(model);
    view->setItemDelegateForColumn(3, voltage);
    view->setItemDelegateForColumn(4, currentSpinBox);
    view->setItemDelegateForColumn(5, currentSpinBox);
    view->setItemDelegateForColumn(6, doubleSpinBox);
    view->setItemDelegateForColumn(7, freq);
    view->setItemDelegateForColumn(8, arc);
    view->verticalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    view->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

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
    view->hideRow(0);
    view->hideRow(1);
    view->hideRow(2);
    view->hideRow(3);
}

void ConfCurrent_AC::initData(QString dat)
{
    QDomDocument docs;
    docs.setContent(dat);
    if (docs.elementsByTagName("ACW").isEmpty())
        return;
    QDomNodeList list = docs.elementsByTagName("ACW").at(0).childNodes();
    for (int i=0; i < list.size(); i++) {
        QDomElement dom = list.at(i).toElement();
        QStringList temp = dom.text().split(",");
        int index = itemNames.indexOf(dom.nodeName());
        if (index == -1)
            continue;
        switch (index) {
        case 0:
            for (int t=0; t < temp.size(); t++) {
                if (temp.at(t) == "1")
                    model->item(t, 0)->setCheckState(Qt::Checked);
                else
                    model->item(t, 0)->setCheckState(Qt::Unchecked);
            }
            break;
        case 7:
            for (int t=0; t < temp.size(); t++) {
                if (temp.at(t) == "0")
                    model->item(t, 3)->setText("50");
                else
                    model->item(t, 3)->setText("60");
            }
            break;
        default:
            for (int t=0; t < temp.size(); t++)
                model->item(t, index)->setText(temp.at(t));
            break;
        }
    }
}

void ConfCurrent_AC::saveData()
{
    doc.clear();
    root.clear();
    root = doc.createElement("ACW");
    doc.appendChild(root);

    for (int i=0; i < itemNames.size(); i++)
        appendXmlData(i, itemNames.at(i));

    emit sendNetMsg(doc.toByteArray().insert(0, "6002 "));
    emit buttonClicked(NULL);
}

void ConfCurrent_AC::appendXmlData(int column, QString name)
{
    QStringList temp;
    for (int i=0; i < 5; i++) {
        if (column == 0) {
            if (model->item(i, column)->checkState() == Qt::Unchecked)
                temp.append("0");
            else
                temp.append("1");
        } else if (column == 7) {
            if (model->item(i, 3)->text() == "50")
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

void ConfCurrent_AC::recvAppShow(QString win)
{
    if (win != this->objectName())
        return;
    emit sendNetMsg("6004 ACW");
}
