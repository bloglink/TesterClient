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
    QStringList units;
    units << tr("uH") << tr("mH") << tr("H");
    ComboBox *Unit = new ComboBox;
    Unit->setItemNames(units);
    DoubleSpinBox *doubleSpinBox = new DoubleSpinBox;
    doubleSpinBox->setMaxinum(9999.99);
    SpinBox *rateBox = new SpinBox;
    rateBox->setMaxinum(100);
    SpinBox *spinBox = new SpinBox;
    spinBox->setMaxinum(20);
    view = new QTableView(this);
    view->setModel(model);
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

    QPushButton *btnExit = new QPushButton(this);
    btnExit->setText(tr("保存退出"));
    btnExit->setMinimumSize(97, 35);
    connect(btnExit, SIGNAL(clicked(bool)), this, SLOT(saveData()));

    QHBoxLayout *btnLayout = new QHBoxLayout;
    btnLayout->addWidget(new QLabel(tr("不平衡度")));
    btnLayout->addWidget(nounLineEdit);
    btnLayout->addWidget(new QLabel(tr("平均次数")));
    btnLayout->addWidget(avrSpinBox);
    btnLayout->addWidget(new QLabel(tr("频率")));
    btnLayout->addWidget(freqComboBox);
    btnLayout->addWidget(new QLabel(tr("连接方式")));
    btnLayout->addWidget(connComboBox);
    btnLayout->addWidget(modeComboBox);
    btnLayout->addStretch();
    btnLayout->addWidget(btnExit);

    QVBoxLayout *layout = new QVBoxLayout;
    layout->addStretch();
    layout->addWidget(view);
    layout->addLayout(btnLayout);
    layout->addStretch();
    this->setLayout(layout);
}

void ConfInductance::initData(QString dat)
{
    QDomDocument docs;
    docs.setContent(dat);
    if (docs.elementsByTagName("IND").isEmpty())
        return;
    QStringList items = itemNames;
    items << "mode" << "noun";
    QDomNodeList list = docs.elementsByTagName("IND").at(0).childNodes();
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
                    model->item(i, 3)->setText("uH");
                if (temp.at(i) == "1")
                    model->item(i, 3)->setText("mH");
                if (temp.at(i) == "2")
                    model->item(i, 3)->setText("H");
            }
            break;
        case 11:
            avrSpinBox->setValue((temp.at(0)).toInt());
            freqComboBox->setCurrentIndex((temp.at(1)).toInt());
            connComboBox->setCurrentIndex((temp.at(2)).toInt());
            modeComboBox->setCurrentIndex((temp.at(3)).toInt());
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

void ConfInductance::saveData()
{
    QDomText text;
    doc.clear();
    root.clear();
    root = doc.createElement("IND");
    doc.appendChild(root);

    for (int i=0; i < itemNames.size(); i++)
        appendXmlData(i, itemNames.at(i));

    QStringList temp;
    temp.append(QString::number(avrSpinBox->value()));
    temp.append(QString::number(freqComboBox->currentIndex()));
    temp.append(QString::number(connComboBox->currentIndex()));
    temp.append(QString::number(modeComboBox->currentIndex()));
    QDomElement temp_comp = doc.createElement("mode");
    root.appendChild(temp_comp);
    text = doc.createTextNode(temp.join(","));
    temp_comp.appendChild(text);

    temp.clear();
    temp.append(nounLineEdit->text());
    QDomElement noun = doc.createElement("noun");
    root.appendChild(noun);
    text = doc.createTextNode(temp.join(","));
    noun.appendChild(text);

    emit sendNetMsg(doc.toByteArray().insert(0, "6002 "));
    emit buttonClicked(NULL);
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
            model->item(i, c)->setText(model->item(0, c)->text());
        break;
    case 0x04:
    case 0x05:
        if (model->item(r, 4)->text().isEmpty() || model->item(r, 5)->text().isEmpty())
            return;
        if (model->item(r, 4)->text().toDouble() > model->item(r, 5)->text().toDouble()) {
            QMessageBox::warning(this, "警告", tr("下限大于上限"), QMessageBox::Ok);
            model->item(r, 4)->setText("0.00");
        }
        break;
    case 0x06:          // Q值下限
    case 0x07:          // Q值上限
        if (model->item(r, 6)->text().isEmpty() || model->item(r, 7)->text().isEmpty())
            return;
        if (model->item(r, 6)->text().toDouble() > model->item(r, 7)->text().toDouble()) {
            QMessageBox::warning(this, "警告", tr("下限大于上限"), QMessageBox::Ok);
            model->item(r, 6)->setText("0");
        }
        break;
    default:
        break;
    }
}

void ConfInductance::appendXmlData(int column, QString name)
{
    QStringList temp;
    for (int i=0; i < 8; i++) {
        if (column == 0) {
            if (model->item(i, column)->checkState() == Qt::Unchecked)
                temp.append("0");
            else
                temp.append("1");
        } else if (column == 3) {
            if (model->item(i, column)->text() == "uH")
                temp.append("0");
            else if (model->item(i, column)->text() == "mH")
                temp.append("1");
            else if (model->item(i, column)->text() == "H")
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

void ConfInductance::recvAppShow(QString win)
{
    if (win != this->objectName())
        return;
    emit sendNetMsg("6004 IND");
}
