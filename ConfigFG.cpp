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
    model = new StandardItem(1, headers.size());
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

void ConfigFG::initData(QByteArray dat)
{
    QDomDocument doc;
    doc.setContent(dat);
    qDebug() << doc.toByteArray();
}

void ConfigFG::saveData()
{
    doc.clear();
    root.clear();
    root = doc.createElement("FG");
    doc.appendChild(root);

    QStringList temp;
    temp << "volt_low_min" << "volt_low_max" << "volt_up_min" << "volt_up_max"
         << "freq_min" << "freq_max" << "duty_min" << "duty_max"
         << "skewing_min" << "skewing_max"
         << "count" << "vcc_volt" << "time";
    for (int i=0; i < temp.size(); i++)
        appendXmlData(i, temp.at(i));
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

