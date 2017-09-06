#include "conf_hall.h"

ConfHall::ConfHall(QWidget *parent) : QWidget(parent)
{
    initUI();
}


ConfHall::~ConfHall()
{
}

void ConfHall::initSettings(QJsonObject obj)
{
    QStringList items = itemNames;
    for (int i=0; i < items.size(); i++) {
        QStringList temp = obj.value(items.at(i)).toString().split(",");
        for (int t=0; t < temp.size(); t++)
            mView->item(t, i)->setText(temp.at(t));
    }
}

void ConfHall::readSettings()
{
    QJsonObject obj;
    QStringList items = itemNames;
    for (int i=0; i < items.size(); i++) {
        QStringList temp;
        for (int t=0; t < mView->rowCount(); t++) {
            double x = mView->item(t, i)->text().toDouble();
            temp.append(QString::number(x));
        }
        obj.insert(items.at(i), temp.join(","));
    }

    QJsonObject array;
    array.insert("HALL", obj);
    emit sendAppCmd(array);
    emit buttonClicked(NULL);
}

void ConfHall::initUI()
{
    this->setObjectName("ConfHall");
    QStringList headers;
    headers << tr("低电平下限") << tr("低电平上限")
            << tr("高电平下限") << tr("高电平上限")
            << tr("频率下限") << tr("频率上限")
            << tr("占空比下限") << tr("占空比上限")
            << tr("相位差下限") << tr("相位差上限")
            << tr("磁极数") << tr("Vcc电压") << tr("测试时间") << "测试方式";
    itemNames << "volt_low_min" << "volt_low_max" << "volt_up_min" << "volt_up_max"
              << "freq_min" << "freq_max" << "duty_min" << "duty_max"
              << "skewing_min" << "skewing_max" << "count" << "vcc_volt" << "time" << "mode";
    mView = new StandardItem(1, headers.size());
    mView->setHorizontalHeaderLabels(headers);
    for (int i=0; i < 1; i++) {
        for (int j=0; j < headers.size(); j++) {
            mView->setData(mView->index(i, j), "");
        }
    }
    DoubleSpinBox *voltage = new DoubleSpinBox;
    voltage->setMaxinum(15);
    SpinBox *freq = new SpinBox;
    freq->setMaxinum(25000);
    DoubleSpinBox *skewing = new DoubleSpinBox;
    skewing->setMaxinum(360);
    DoubleSpinBox *duty = new DoubleSpinBox;
    duty->setMaxinum(100);
    SpinBox *count = new SpinBox;
    count->setMaxinum(10);
    DoubleSpinBox *time = new DoubleSpinBox;
    time->setMaxinum(99);
    DoubleSpinBox *vcc = new DoubleSpinBox;
    vcc->setMaxinum(15);

    view = new QTableView(this);
    view->setModel(mView);
    view->setItemDelegateForColumn(0, voltage);
    view->setItemDelegateForColumn(1, voltage);
    view->setItemDelegateForColumn(2, voltage);
    view->setItemDelegateForColumn(3, voltage);
    view->setItemDelegateForColumn(4, freq);
    view->setItemDelegateForColumn(5, freq);
    view->setItemDelegateForColumn(6, duty);
    view->setItemDelegateForColumn(7, duty);
    view->setItemDelegateForColumn(8, skewing);
    view->setItemDelegateForColumn(9, skewing);
    view->setItemDelegateForColumn(10, count);
    view->setItemDelegateForColumn(11, vcc);
    view->setItemDelegateForColumn(12, time);
    view->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    view->verticalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    QPushButton *btnExit = new QPushButton(this);
    btnExit->setText(tr("保存退出"));
    btnExit->setMinimumSize(97, 35);
    connect(btnExit, SIGNAL(clicked(bool)), this, SLOT(readSettings()));

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

