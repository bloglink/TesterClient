#include "ConfigFG.h"

ConfigFG::ConfigFG(QWidget *parent) : QWidget(parent)
{
    initUI();
}

ConfigFG::~ConfigFG()
{
}

void ConfigFG::initUI()
{
    this->setObjectName("ConfigFG");
    QStringList headers;
    headers << tr("低电平下限") << tr("低电平上限")
            << tr("高电平下限") << tr("高电平上限")
            << tr("频率下限") << tr("频率上限")
            << tr("占空比下限") << tr("占空比上限")
            << tr("相位差下限") << tr("相位差上限")
            << tr("磁极数") << tr("Vcc电压") << tr("测试时间");
    itemNames << "volt_low_min" << "volt_low_max" << "volt_up_min" << "volt_up_max"
              << "freq_min" << "freq_max" << "duty_min" << "duty_max"
              << "skewing_min" << "skewing_max"
              << "count" << "vcc_volt" << "time";
    model = new StandardItem(1, headers.size());
    model->setHorizontalHeaderLabels(headers);
    for (int i=0; i < 1; i++) {
        for (int j=0; j < headers.size(); j++) {
            model->setData(model->index(i, j), "");
        }
    }
    SpinBox *voltage = new SpinBox;
    voltage->setMaxinum(15);
    SpinBox *freq = new SpinBox;
    freq->setMaxinum(5000);
    SpinBox *speed = new SpinBox;
    speed->setMaxinum(3000);
    DoubleSpinBox *vcc = new DoubleSpinBox;
    vcc->setMaxinum(15);
    DoubleSpinBox *vsp = new DoubleSpinBox;
    vsp->setMaxinum(15);
    DoubleSpinBox *time = new DoubleSpinBox;
    time->setMaxinum(99);

    view = new QTableView(this);
    view->setModel(model);
    view->setItemDelegateForColumn(0, voltage);
    view->setItemDelegateForColumn(1, voltage);
    view->setItemDelegateForColumn(2, voltage);
    view->setItemDelegateForColumn(3, voltage);
    view->setItemDelegateForColumn(4, freq);
    view->setItemDelegateForColumn(5, freq);
    view->setItemDelegateForColumn(6, speed);
    view->setItemDelegateForColumn(7, vcc);
    view->setItemDelegateForColumn(8, vsp);
    view->setItemDelegateForColumn(9, time);
    view->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
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
}

void ConfigFG::initData(QString dat)
{
    QDomDocument docs;
    docs.setContent(dat);
    if (docs.elementsByTagName("FG").isEmpty())
        return;
    QStringList items = itemNames;;
    QDomNodeList list = docs.elementsByTagName("FG").at(0).childNodes();
    for (int i=0; i < list.size(); i++) {
        QDomElement dom = list.at(i).toElement();
        QStringList temp = dom.text().split(",");
        int index = items.indexOf(dom.nodeName());
        if (index == -1)
            continue;
        for (int t=0; t < temp.size(); t++)
            model->item(t, index)->setText(temp.at(t));
    }
}

void ConfigFG::saveData()
{
    doc.clear();
    root.clear();
    root = doc.createElement("FG");
    doc.appendChild(root);

    for (int i=0; i < itemNames.size(); i++)
        appendXmlData(i, itemNames.at(i));
    initData(doc.toByteArray());
    emit sendNetMsg(doc.toByteArray().insert(0, "6002 "));
    emit buttonClicked(NULL);
}

void ConfigFG::appendXmlData(int column, QString name)
{
    QDomText text = doc.createTextNode(model->item(0, column)->text());
    QDomElement xml = doc.createElement(name);
    xml.appendChild(text);
    root.appendChild(xml);
}

void ConfigFG::recvAppShow(QString win)
{
    if (win != this->objectName())
        return;
    emit sendNetMsg("6004 FG");
}

