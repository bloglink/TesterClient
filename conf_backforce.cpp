/*******************************************************************************
 * Copyright [2017]   <青岛艾普智能仪器有限公司>
 * All rights reserved.
 *
 * version:     0.1
 * author:      zhaonanlin
 * brief:       BEMF配置
*******************************************************************************/
#include "conf_backforce.h"

ConfBackForce::ConfBackForce(QWidget *parent) : QWidget(parent)
{
    initUI();
}

ConfBackForce::~ConfBackForce()
{
}

void ConfBackForce::initUI()
{
    this->setObjectName("ConfBackForce");
    QStringList headers;
    headers << tr("Hu电压下限") << tr("Hu电压上限") << tr("Hv电压下限")
            << tr("Hv电压上限") << tr("Hw电压下限") << tr("Hw电压上限")
            << tr("伺服转速") << tr("伺服转向") << tr("与霍尔相位差");
    itemNames << "hu_volt_min" << "hu_volt_max" << "hv_volt_min"
              << "hv_volt_max" << "hw_volt_min"
              << "hw_volt_max" << "speed" << "turn" << "skewing";
    model = new StandardItem(1, headers.size());
    model->setHorizontalHeaderLabels(headers);
    for (int i=0; i < 1; i++) {
        for (int j=0; j < headers.size(); j++) {
            model->setData(model->index(i, j), "");
        }
    }
    SpinBox *voltage = new SpinBox;
    voltage->setMaxinum(500);
    SpinBox *speed = new SpinBox;
    speed->setMaxinum(3000);
    QStringList turns;
    turns << "顺时针" << "逆时针";
    ComboBox *turn = new ComboBox;
    turn->setItemNames(turns);
    SpinBox *skewing = new SpinBox;
    skewing->setMaxinum(360);
    view = new QTableView(this);
    view->setModel(model);
    view->setItemDelegateForColumn(0, voltage);
    view->setItemDelegateForColumn(1, voltage);
    view->setItemDelegateForColumn(2, voltage);
    view->setItemDelegateForColumn(3, voltage);
    view->setItemDelegateForColumn(4, voltage);
    view->setItemDelegateForColumn(5, voltage);
    view->setItemDelegateForColumn(6, speed);
    view->setItemDelegateForColumn(7, turn);
    view->setItemDelegateForColumn(7, skewing);
    view->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    view->verticalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    nounSpinBox = new QSpinBox(this);
    nounSpinBox->setMinimumSize(97, 35);
    nounSpinBox->setButtonSymbols(QAbstractSpinBox::NoButtons);
    nounSpinBox->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);

    QPushButton *btnExit = new QPushButton(this);
    btnExit->setText(tr("保存退出"));
    btnExit->setMinimumSize(97, 35);
    connect(btnExit, SIGNAL(clicked(bool)), this, SLOT(saveData()));

    QHBoxLayout *btnLayout = new QHBoxLayout;
    btnLayout->addWidget(new QLabel("不平衡度", this));
    btnLayout->addWidget(nounSpinBox);
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

void ConfBackForce::initData(QString dat)
{
    QDomDocument docs;
    docs.setContent(dat);
    if (docs.elementsByTagName("PWR").isEmpty())
        return;
    QStringList items = itemNames;
    items << "sequence" << "turn";
    QDomNodeList list = docs.elementsByTagName("PWR").at(0).childNodes();
    for (int i=0; i < list.size(); i++) {
        QDomElement dom = list.at(i).toElement();
        QStringList temp = dom.text().split(",");
        int index = items.indexOf(dom.nodeName());
        if (index == -1)
            continue;
        switch (index) {
        case 9:
            nounSpinBox->setValue(temp.at(0).toInt());
            break;
        default:
            for (int t=0; t < temp.size(); t++)
                model->item(t, index)->setText(temp.at(t));
            break;
        }
    }
}

void ConfBackForce::saveData()
{
    doc.clear();
    root.clear();
    root = doc.createElement("BEMF");
    doc.appendChild(root);

    for (int i=0; i < itemNames.size(); i++)
        appendXmlData(i, itemNames.at(i));

    QStringList temp;
    temp.append(nounSpinBox->text());
    QDomElement noun = doc.createElement("noun");
    root.appendChild(noun);
    QDomText text = doc.createTextNode(temp.join(","));
    noun.appendChild(text);

    emit sendNetMsg(doc.toByteArray().insert(0, "6002 "));
    emit buttonClicked(NULL);
    initData(doc.toByteArray());
}

void ConfBackForce::appendXmlData(int column, QString name)
{
    QDomText text = doc.createTextNode(model->item(0, column)->text());
    QDomElement xml = doc.createElement(name);
    xml.appendChild(text);
    root.appendChild(xml);
}

void ConfBackForce::recvAppShow(QString win)
{
    if (win != this->objectName())
        return;
    emit sendNetMsg("6004 BEMF");
}
