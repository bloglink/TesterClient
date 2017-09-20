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
        } else if (items.at(i) == "volt") {
            for (int t=0; t < mView->rowCount(); t++) {
                if (mView->item(t, 3)->text() == "500")
                    temp.append("0");
                else
                    temp.append("1");
            }
            obj.insert(items.at(i), temp.join(","));
        } else {
            for (int t=0; t < mView->rowCount(); t++) {
                double x = mView->item(t, i)->text().toDouble();
                temp.append(QString::number(x));
            }
        }
        obj.insert(items.at(i), temp.join(","));
    }

    QJsonObject array;
    array.insert("IR", obj);
    emit sendAppCmd(array);
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
    connect(btnSave, SIGNAL(clicked(bool)), this, SLOT(readSettings()));

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

