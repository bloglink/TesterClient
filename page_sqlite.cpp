/*******************************************************************************
 * Copyright [2017]   <青岛艾普智能仪器有限公司>
 * All rights reserved.
 *
 * version:     0.1
 * author:      zhaonanlin
 * brief:       数据管理
*******************************************************************************/
#include "page_sqlite.h"

PageSqlite::PageSqlite(QWidget *parent) : QWidget(parent)
{
    initUI();
}

PageSqlite::~PageSqlite()
{
}

void PageSqlite::initUI()
{
    this->setObjectName("DataPage");
    QStringList headers;
    headers << tr("项目") << tr("测试总数") << tr("合格数量") << tr("不合格数量") << tr("合格率");
    QStringList items;
    items << tr("总数") << tr("电阻") << tr("反嵌") << tr("绝缘") << tr("交耐") << tr("匝间")
          << tr("电感") << tr("空载") << tr("负载") << tr("霍尔");
    model = new StandardItem(items.size(), headers.size());
    model->setHorizontalHeaderLabels(headers);
    for (int i=0; i < items.size(); i++) {
        for (int j=0; j < headers.size(); j++) {
            model->setData(model->index(i, j), "");
        }
        model->item(i, 0)->setText(items.at(i));
    }
    view = new QTableView(this);
    view->setModel(model);
    view->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    view->verticalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    drawHistogram(items);

    QPushButton *btnExit = new QPushButton(this);
    btnExit->setText(tr("保存退出"));
    btnExit->setMinimumSize(97, 35);
    connect(btnExit, SIGNAL(clicked(bool)), this, SLOT(saveData()));

    QHBoxLayout *btnLayout = new QHBoxLayout;
    btnLayout->addStretch();
    btnLayout->addWidget(btnExit);

    QVBoxLayout *plot = new QVBoxLayout;

    plot->addWidget(view);
    plot->addWidget(customplot);
    plot->addLayout(btnLayout);
    plot->setStretch(0, 1);
    plot->setStretch(1, 1);

    this->setLayout(plot);
}

void PageSqlite::drawHistogram(QStringList names)
{
    customplot = new QCustomPlot(this);
    customplot->setBackground(QBrush(QColor(25, 25, 25))); //设置背景色
    customplot->axisRect()->setMinimumMargins(QMargins(0, 0, 0, 0));
    customplot->axisRect()->setupFullAxesBox();
    QStringList colors;
    colors << "red" << "green" << "blue" << "cyan" << "magenta"
           << "yellow" << "darkRed" << "darkGreen" << "darkBlue"
           << "darkCyan" << "darkMagenta" << "darkYellow";
    for (int i=0; i < names.size(); i++) {
        QVector<double> x1(1), y1(1);
        x1[0] = i+1;
        y1[0] = (i+1)*8;

        QCPBars *bars1 = new QCPBars(customplot->xAxis, customplot->yAxis);
        bars1->setWidth(0.9);
        bars1->setData(x1, y1);
        bars1->setPen(Qt::NoPen);
        bars1->setBrush(QColor(colors.at(i)));
    }
    QLinearGradient plotGradient;
    plotGradient.setStart(0, 0);
    plotGradient.setFinalStop(0, 350);
    plotGradient.setColorAt(0, QColor(80, 80, 80));
    plotGradient.setColorAt(1, QColor(50, 50, 50));
    customplot->setBackground(plotGradient);

    QVector<double> ticks(names.size());
    QVector<QString> labels(names.size());
    for (int i=0; i < names.size(); i++) {
        ticks[i] = i+1;
        labels[i] = names.at(i);
    }
    QSharedPointer<QCPAxisTickerText> textTicker(new QCPAxisTickerText);
    textTicker->addTicks(ticks, labels);
    customplot->xAxis->setTicker(textTicker);
    customplot->xAxis->setTickLabelRotation(0);
    customplot->xAxis->setSubTicks(false);
    customplot->xAxis->setTickLength(0, 1);
    customplot->xAxis->setRange(0.5, names.size()+0.5);
    customplot->xAxis->setBasePen(QPen(Qt::black));
    customplot->xAxis->setTickPen(QPen(Qt::black));
    customplot->xAxis->grid()->setVisible(false);
    customplot->yAxis->grid()->setVisible(false);
    customplot->xAxis->setTickLabelColor(Qt::white);
    customplot->xAxis->setLabelColor(Qt::white);
    customplot->yAxis->setTicks(false);
    customplot->yAxis2->setTicks(false);
    customplot->xAxis2->setTicks(false);
    customplot->yAxis->setRange(0, 100);
}

void PageSqlite::saveData()
{
    emit buttonClicked(NULL);
}

