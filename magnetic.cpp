#include "magnetic.h"

Magnetic::Magnetic(QWidget *parent) : QWidget(parent)
{
    initUI();
}


void Magnetic::initSettings(QJsonObject obj)
{
    QStringList items = itemNames;
    items << "dir";
    for (int i=0; i < items.size(); i++) {
        QStringList temp = obj.value(items.at(i)).toString().split(",");
        if (items.at(i) == "test") {
            for (int t=0; t < temp.size(); t++) {
                if (temp.at(t) == "0")
                    mView->item(t, 0)->setCheckState(Qt::Unchecked);
                else
                    mView->item(t, 0)->setCheckState(Qt::Checked);
            }
        } else if (items.at(i) == "wave") {
        } else if (items.at(i) == "dir") {
        } else {
            for (int t=0; t < temp.size(); t++)
                mView->item(t, i)->setText(temp.at(t));
        }
    }
}

void Magnetic::readSettings()
{
    QJsonObject obj;
    QStringList items = itemNames;
    for (int i=0; i < items.size(); i++) {
        QStringList temp;
        if (items.at(i) == "test") {
            for (int t=0; t < mView->rowCount(); t++) {
                if (mView->item(t, 0)->checkState() == Qt::Unchecked)
                    temp.append("0");
                else
                    temp.append("1");
            }
        } else {
            for (int t=0; t < mView->rowCount(); t++) {
                double x = mView->item(t, i)->text().toDouble();
                temp.append(QString::number(x));
            }
        }
        obj.insert(items.at(i), temp.join(","));
    }
    QStringList temp;

    QJsonObject array;
    array.insert("MAG", obj);
    emit sendAppCmd(array);
}

void Magnetic::initUI()
{
    this->setObjectName("Magnetic");
    QStringList headers;
    headers << tr("反嵌") << tr("端一") << tr("端二") << tr("上限") << tr("波形");
    itemNames << "test" << "port1" << "port2" << "max" << "wave";
    mView = new StandardItem(8, headers.size());
    mView->setHorizontalHeaderLabels(headers);
    for (int i=0; i < 8; i++) {
        for (int j=0; j < headers.size(); j++) {
            mView->setData(mView->index(i, j), "");
        }
        mView->item(i, 0)->setCheckable(true);
        waves.append(new QCustomPlot(this));
        waves.at(i)->setBackground(QBrush(QColor(25, 25, 25))); //设置背景色
        waves.at(i)->xAxis->grid()->setPen(QPen(Qt::darkGreen, 1, Qt::DotLine));
        waves.at(i)->yAxis->grid()->setPen(QPen(Qt::darkGreen, 1, Qt::DotLine));
        waves.at(i)->xAxis->setTicks(false);
        waves.at(i)->yAxis->setTicks(false);
        waves.at(i)->xAxis->setTickLabels(false);
        waves.at(i)->yAxis->setTickLabels(false);
        waves.at(i)->axisRect()->setMinimumMargins(QMargins(1, 5, 5, 5));
        waves.at(i)->xAxis->setBasePen(Qt::NoPen);
        waves.at(i)->yAxis->setBasePen(Qt::NoPen);
        waves.at(i)->xAxis2->setBasePen(Qt::NoPen);
        waves.at(i)->yAxis2->setBasePen(Qt::NoPen);
        waves.at(i)->xAxis->setRange(0, 400);
        waves.at(i)->yAxis->setRange(0, 101);
    }

    QStringList names;
    names << "1" << "2" << "3" << "4" << "5" << "6" << "7" << "8";
    DComboBox *comboBox = new DComboBox;
    comboBox->setItemNames(names);
    DSpinBox *spinBox = new DSpinBox;

    view = new QTableView(this);
    view->setModel(mView);
    view->setItemDelegateForColumn(1, comboBox);
    view->setItemDelegateForColumn(2, comboBox);
    view->setItemDelegateForColumn(3, spinBox);
    view->horizontalHeader()->setSectionResizeMode(4, QHeaderView::Stretch);
    view->verticalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    view->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Fixed);
    view->setColumnWidth(0, 50);

    for (int i=0; i < 8; i++) {
        view->setIndexWidget(mView->index(i, 4), waves.at(i));
    }

    turnCheckBox = new QCheckBox(this);
    turnCheckBox->setText(tr("旋向"));
    turnComboBox = new QComboBox(this);
    turnComboBox->addItem(tr("CCW"));
    turnComboBox->addItem(tr("CW"));
    turnComboBox->setMinimumSize(97, 35);
    turnComboBox->setView(new QListView);

    QPushButton *btnWaveL = new QPushButton(this);
    btnWaveL->setText(tr("采集左"));
    btnWaveL->setMinimumSize(97, 35);
    connect(btnWaveL, SIGNAL(clicked(bool)), this, SLOT(waveL()));

    QPushButton *btnWaveR = new QPushButton(this);
    btnWaveR->setText(tr("采集右"));
    btnWaveR->setMinimumSize(97, 35);
    connect(btnWaveR, SIGNAL(clicked(bool)), this, SLOT(waveR()));

    QPushButton *btnSave = new QPushButton(this);
    btnSave->setText(tr("保存"));
    btnSave->setMinimumSize(97, 35);
    connect(btnSave, SIGNAL(clicked(bool)), this, SLOT(readSettings()));

    QPushButton *btnExit = new QPushButton(this);
    btnExit->setText(tr("退出"));
    btnExit->setMinimumSize(97, 35);
    connect(btnExit, SIGNAL(clicked(bool)), this, SLOT(back()));

    QHBoxLayout *btnLayout = new QHBoxLayout;
    btnLayout->addWidget(turnCheckBox);
    btnLayout->addWidget(turnComboBox);
    btnLayout->addStretch();
    btnLayout->addWidget(btnWaveL);
    btnLayout->addWidget(btnWaveR);
    btnLayout->addWidget(btnSave);
    btnLayout->addWidget(btnExit);

    QVBoxLayout *layout = new QVBoxLayout;
    layout->addWidget(view);
    layout->addLayout(btnLayout);
    layout->setStretch(0, 8);
    layout->setStretch(1, 1);
    this->setLayout(layout);
}

void Magnetic::waveL()
{
    emit sendNetMsg(QString("6038 %1").arg(0x13).toUtf8());
}

void Magnetic::waveR()
{
    emit sendNetMsg(QString("6038 %1").arg(0x14).toUtf8());
}

void Magnetic::back()
{
    emit buttonClicked(NULL);
}

void Magnetic::recvAppShow(QString win)
{
    if (win != this->objectName())
        return;
    emit sendNetMsg("6004 MAG");
}
