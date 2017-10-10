#include "backpage.h"

BackPage::BackPage(QWidget *parent) : QWidget(parent)
{
    initUI();
}

void BackPage::initSettings(QJsonObject obj)
{
    QStringList items = itemNames;
    for (int i=0; i < items.size(); i++) {
        QStringList temp = obj.value(items.at(i)).toString().split(",");
        if (items.at(i) == "test") {
            for (int t=0; t < temp.size(); t++) {
                if (temp.at(t) == "0")
                    mView->item(t, 0)->setCheckState(Qt::Unchecked);
                else
                    mView->item(t, 0)->setCheckState(Qt::Checked);
            }
        } else if (items.at(i) == "volt") {
            for (int t=0; t < temp.size(); t++) {
                if (temp.at(t) == "0")
                    mView->item(t, 3)->setText("500");
                else
                    mView->item(t, 3)->setText("1000");
            }
        } else {
            for (int t=0; t < temp.size(); t++)
                mView->item(t, i)->setText(temp.at(t));
        }
    }
}


void BackPage::readSettings()
{
    QSettings *g_ini = new QSettings("./nandflash/global.ini", QSettings::IniFormat);
    g_ini->setIniCodec("GB18030");
    g_ini->beginGroup("GLOBAL");

    QStringList k_loads = g_ini->value("k_loads","10000,10000,10000,10000,10000").toString().split(",");
    QStringList b_loads = g_ini->value("b_loads","5000,5000,5000,5000,5000").toString().split(",");
    for (int i=0; i < qMin(k_loads.size(), b_loads.size()); i++) {
        mView->item(i, 0)->setText(k_loads.at(i));
        mView->item(i, 1)->setText(b_loads.at(i));
    }
}

void BackPage::saveSettings()
{
    QSettings *g_ini = new QSettings("./nandflash/global.ini", QSettings::IniFormat);
    g_ini->setIniCodec("GB18030");
    g_ini->beginGroup("GLOBAL");
    QStringList k_loads;
    QStringList b_loads;
    for (int i=0; i < mView->rowCount(); i++) {
        k_loads.append(mView->item(i, 0)->text());
        b_loads.append(mView->item(i, 1)->text());
    }
    g_ini->setValue("k_loads", k_loads.join(","));
    g_ini->setValue("b_loads", b_loads.join(","));
}

void BackPage::initUI()
{
    this->setObjectName("BackPage");
    QStringList headers;
    headers << tr("K值") << tr("b值");
    itemNames << "K" << "b";
    mView = new StandardItem(5, headers.size());
    mView->setHorizontalHeaderLabels(headers);
    for (int i=0; i < 5; i++) {
        for (int j=0; j < headers.size(); j++) {
            mView->setData(mView->index(i, j), "");
        }
    }
    view = new QTableView(this);
    view->setModel(mView);
    view->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    view->verticalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    QPushButton *btnSave = new QPushButton(this);
    btnSave->setText(tr("保存"));
    btnSave->setMinimumSize(97, 35);
    connect(btnSave, SIGNAL(clicked(bool)), this, SLOT(saveSettings()));

    QPushButton *btnExit = new QPushButton(this);
    btnExit->setText(tr("退出"));
    btnExit->setMinimumSize(97, 35);
    connect(btnExit, SIGNAL(clicked(bool)), this, SLOT(back()));

    QHBoxLayout *btnLayout = new QHBoxLayout;
    btnLayout->addStretch();
    btnLayout->addWidget(btnSave);
    btnLayout->addWidget(btnExit);;

    QVBoxLayout *layout = new QVBoxLayout;
    layout->addStretch();
    layout->addStretch();
    layout->addWidget(view);
    layout->addLayout(btnLayout);
    layout->addStretch();
    layout->addStretch();
    this->setLayout(layout);
}

void BackPage::back()
{
    emit buttonClicked(NULL);
}

void BackPage::recvAppShow(QString win)
{
    if (win != this->objectName())
        return;
    readSettings();
}
