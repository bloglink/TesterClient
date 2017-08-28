/*******************************************************************************
 * Copyright [2017]   <青岛艾普智能仪器有限公司>
 * All rights reserved.
 *
 * version:     0.1
 * author:      zhaonanlin
 * brief:       负载配置
*******************************************************************************/
#include "conf_loadtesting.h"

ConfLoadTesting::ConfLoadTesting(QWidget *parent) : QWidget(parent)
{
    initUI();
}

ConfLoadTesting::~ConfLoadTesting()
{
}

void ConfLoadTesting::initUI()
{
    this->setObjectName("ConfLoadTesting");
    QStringList headers;
    headers << tr("电压") << tr("电流下限") << tr("电流上限")
            << tr("功率下限") << tr("功率上限")
            << tr("转速下限") << tr("转速上限") << tr("扭矩") << tr("Vcc电压")
            << tr("Vsp电压") << tr("测试时间");
    itemNames << "volt" << "curr_min" << "curr_max"
              << "pwr_min" << "pwr_max"
              << "speed_min" << "speed_max" << "torque"
              << "vcc_volt" << "vsp_volt" << "time";
    model = new StandardItem(1, headers.size());
    model->setHorizontalHeaderLabels(headers);
    for (int i=0; i < 1; i++) {
        for (int j=0; j < headers.size(); j++) {
            model->setData(model->index(i, j), "");
        }
    }
    SpinBox *voltage = new SpinBox;
    voltage->setMaxinum(500);
    DoubleSpinBox *current = new DoubleSpinBox;
    current->setMaxinum(5);
    SpinBox *power = new SpinBox;
    power->setMaxinum(5000);
    SpinBox *speed = new SpinBox;
    speed->setMaxinum(3000);
    DoubleSpinBox *torque = new DoubleSpinBox;
    torque->setMaxinum(20);
    DoubleSpinBox *vcc = new DoubleSpinBox;
    vcc->setMaxinum(15);
    DoubleSpinBox *vsp = new DoubleSpinBox;
    vsp->setMaxinum(15);
    DoubleSpinBox *time = new DoubleSpinBox;
    time->setMaxinum(99);
    view = new QTableView(this);
    view->setModel(model);
    view->setItemDelegateForColumn(0, voltage);
    view->setItemDelegateForColumn(1, current);
    view->setItemDelegateForColumn(2, current);
    view->setItemDelegateForColumn(3, power);
    view->setItemDelegateForColumn(4, power);
    view->setItemDelegateForColumn(5, speed);
    view->setItemDelegateForColumn(6, speed);
    view->setItemDelegateForColumn(7, torque);
    view->setItemDelegateForColumn(8, vcc);
    view->setItemDelegateForColumn(9, vsp);
    view->setItemDelegateForColumn(10, time);
    view->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    view->verticalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    QPushButton *btnExit = new QPushButton(this);
    btnExit->setText(tr("保存退出"));
    btnExit->setMinimumSize(97, 35);
    connect(btnExit, SIGNAL(clicked(bool)), this, SLOT(saveData()));

    QHBoxLayout *btnLayout = new QHBoxLayout;
    btnLayout->addStretch();
    btnLayout->addWidget(btnExit);

    QStringList header;
    header << tr("T0") << tr("T1") << tr("T2") << tr("T3") << tr("T4")
           << tr("T5") << tr("T6") << tr("T7") << tr("T8") << tr("T9")
           << tr("T10") << tr("T11") << tr("T12") << tr("T13");
    tModel = new StandardItem(1, header.size());
    tModel->setHorizontalHeaderLabels(header);
    for (int i=0; i < 1; i++) {
        for (int j=0; j < header.size(); j++) {
            tModel->setData(tModel->index(i, j), "");
        }
    }
    connect(tModel, SIGNAL(itemChanged(QStandardItem*)), this, SLOT(sequence()));
    tView = new QTableView(this);
    tView->setModel(tModel);
    tView->setItemDelegateForRow(0, time);
    tView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    tView->verticalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    QGroupBox *plot = new QGroupBox(this);
    plot->setTitle(tr("负载上电时序"));

    customplot = new QCustomPlot(this);
    sequence();

    QVBoxLayout *plotLayout = new QVBoxLayout;
    plotLayout->addWidget(tView);
    plotLayout->addWidget(customplot);
    plotLayout->setStretch(1, 1);
    plotLayout->setMargin(15);
    plot->setLayout(plotLayout);

    QVBoxLayout *layout = new QVBoxLayout;
    layout->addStretch();
    layout->addWidget(view);
    layout->addWidget(plot);
    layout->addLayout(btnLayout);
    layout->addStretch();
    layout->setStretch(0, 1);
    layout->setStretch(1, 1);
    layout->setStretch(2, 3);
    layout->setStretch(4, 1);
    this->setLayout(layout);
}

void ConfLoadTesting::initData(QString dat)
{
    QDomDocument docs;
    docs.setContent(dat);
    if (docs.elementsByTagName("LOAD").isEmpty())
        return;
    QStringList items = itemNames;
    items << "sequence";
    QDomNodeList list = docs.elementsByTagName("LOAD").at(0).childNodes();
    for (int i=0; i < list.size(); i++) {
        QDomElement dom = list.at(i).toElement();
        QStringList temp = dom.text().split(",");
        int index = items.indexOf(dom.nodeName());
        if (index == -1)
            continue;
        switch (index) {
        case 11:
            for (int t=0; t < temp.size(); t++)
                tModel->item(0, t)->setText(temp.at(t));
            break;
        default:
            for (int t=0; t < temp.size(); t++)
                model->item(t, index)->setText(temp.at(t));
            break;
        }
    }
}

void ConfLoadTesting::saveData()
{
    doc.clear();
    root.clear();
    root = doc.createElement("LOAD");
    doc.appendChild(root);

    for (int i=0; i < itemNames.size(); i++)
        appendXmlData(i, itemNames.at(i));
    QStringList temp;
    for (int i=0; i < tModel->columnCount(); i++) {
        temp.append(tModel->item(0, i)->text());
    }
    QDomText text = doc.createTextNode(temp.join(","));
    QDomElement xml = doc.createElement("sequence");
    xml.appendChild(text);
    root.appendChild(xml);
    emit sendNetMsg(doc.toByteArray().insert(0, "6002 "));
    emit buttonClicked(NULL);
}

void ConfLoadTesting::appendXmlData(int column, QString name)
{
    QDomText text = doc.createTextNode(model->item(0, column)->text());
    QDomElement xml = doc.createElement(name);
    xml.appendChild(text);
    root.appendChild(xml);
}

void ConfLoadTesting::sequence()
{
    customplot->clearGraphs();
    customplot->clearItems();
    customplot->clearMask();
    customplot->clearPlottables();
    customplot->legend->setVisible(true);

    double T0 = 100;
    double T1 = 50;
    double T2 = 100;
    double T3 = 50;
    double T4 = 100;

    double T5 = 50;
    double T6 = 50;
    double T7 = 50;
    double T8 = 50;

    double T9 = 100;
    double T10 = 50;
    double T11 = 100;
    double T12 = 50;
    double T13 = 100;

    if (!tModel->item(0, 0)->text().isEmpty())
        T0 = tModel->item(0, 0)->text().toDouble()*100;
    if (!tModel->item(0, 1)->text().isEmpty())
        T1 = tModel->item(0, 1)->text().toDouble()*100;
    if (!tModel->item(0, 2)->text().isEmpty())
        T2 = tModel->item(0, 2)->text().toDouble()*100;
    if (!tModel->item(0, 3)->text().isEmpty())
        T3 = tModel->item(0, 3)->text().toDouble()*100;
    if (!tModel->item(0, 4)->text().isEmpty())
        T4 = tModel->item(0, 4)->text().toDouble()*100;
    if (!tModel->item(0, 5)->text().isEmpty())
        T5 = tModel->item(0, 5)->text().toDouble()*100;
    if (!tModel->item(0, 6)->text().isEmpty())
        T6 = tModel->item(0, 6)->text().toDouble()*100;
    if (!tModel->item(0, 7)->text().isEmpty())
        T7 = tModel->item(0, 7)->text().toDouble()*100;
    if (!tModel->item(0, 8)->text().isEmpty())
        T8 = tModel->item(0, 8)->text().toDouble()*100;
    if (!tModel->item(0, 9)->text().isEmpty())
        T9 = tModel->item(0, 9)->text().toDouble()*100;
    if (!tModel->item(0, 10)->text().isEmpty())
        T10 = tModel->item(0, 10)->text().toDouble()*100;
    if (!tModel->item(0, 11)->text().isEmpty())
        T11 = tModel->item(0, 11)->text().toDouble()*100;
    if (!tModel->item(0, 12)->text().isEmpty())
        T12 = tModel->item(0, 12)->text().toDouble()*100;
    if (!tModel->item(0, 13)->text().isEmpty())
        T13 = tModel->item(0, 13)->text().toDouble()*100;

    double t = (T0+T1+T2+T3+T4+T10+T11+T12+T13+T5+T6+T7+T8)*2;
    double Ts = t/20;
    double Te = t/15;

    double t0 = Ts;
    double t1 = Ts+T0;
    double t2 = Ts+T0+T1;
    double t3 = Ts+T0+T1+T2;
    double t4 = Ts+T0+T1+T2+T3;
    double t5 = Ts+T0+T1+T2+T3+T4;
    double tn0 = Ts+T0+T1+T2+T3+T4+T5;
    double tn1 = Ts+T0+T1+T2+T3+T4+T5+T6;

    double tn2 = t-Te-T13-T12-T11-T10-T9-T8-T7;
    double tn3 = t-Te-T13-T12-T11-T10-T9-T8;
    double t6 = t-Te-T13-T12-T11-T10-T9;
    double t7 = t-Te-T13-T12-T11-T10;
    double t8 = t-Te-T13-T12-T11;
    double t9 = t-Te-T13-T12;
    double t10 = t-Te-T13;
    double t11 = t-Te;

    ruler(t0);
    ruler(t1);
    wavePacket(t0, t1, "T0");
    ruler(t2);
    wavePacket(t1, t2, "T1");
    ruler(t3);
    wavePacket(t2, t3, "T2");
    ruler(t4);
    wavePacket(t3, t4, "T3");
    ruler(t5);
    wavePacket(t4, t5, "T4");
    ruler(tn0);
    wavePacket(t5, tn0, "T5");
    ruler(tn1);
    wavePacket(tn0, tn1, "T6");

    wavePacket(tn1, tn2, "测试时间");
    ruler(tn2);
    wavePacket(tn2, tn3, "T7");
    ruler(tn3);
    wavePacket(tn3, t6, "T8");
    ruler(t6);
    wavePacket(t6, t7, "T9");
    ruler(t7);
    wavePacket(t7, t8, "T10");
    ruler(t8);
    wavePacket(t8, t9, "T11");
    ruler(t9);
    wavePacket(t9, t10, "T12");
    ruler(t10);
    wavePacket(t10, t11, "T13");
    ruler(t11);

    customplot->legend->clearItems();

    QVector<double> x(t); //可变数组存放绘图的坐标的数据，分别存放x和y坐标的数据
    QVector<double> hu(t), hv(t), hw(t), hn(t);

    for (int i = 0; i<t; i++) { //添加数据
        x[i] = i;
        if (i < t0)
            hu[i] = 0;
        else if (i < t1)
            hu[i] = (i-t0)*(35/T0);
        else if (i < t10)
            hu[i] = 35;
        else if (i < t11)
            hu[i] = (t11-i)*(35/T13);
        else
            hu[i] = 0;

        if (i < t2)
            hv[i] = 0;
        else if (i < t3)
            hv[i] = (i-t2)*(40/T2);
        else if (i < t8)
            hv[i] = 40;
        else if (i < t9)
            hv[i] = (t9-i)*(40/T11);
        else
            hv[i] = 0;

        if (i < t4)
            hw[i] = 0;
        else if (i < t5)
            hw[i] = (i-t4)*(30/T4);
        else if (i < t6)
            hw[i] = 30;
        else if (i < t7)
            hw[i] = (t7-i)*(30/T9);
        else
            hw[i] = 0;

        if (i < tn0)
            hn[i] = 0;
        else if (i < tn1)
            hn[i] = (i-tn0)*(25/T6);
        else if (i < tn2)
            hn[i] = 25;
        else if (i < tn3)
            hn[i] = (tn3-i)*(25/T7);
        else
            hn[i] = 0;

    }

    QCPGraph *graph1 = customplot->addGraph();  //向绘图区域QCustomPlot添加一条曲线
    graph1->setPen(QPen(Qt::yellow, 2, Qt::SolidLine));
    graph1->setData(x, hu);
    graph1->setName("Vcc");
    QCPGraph *graph2 = customplot->addGraph();  //向绘图区域QCustomPlot添加一条曲线
    graph2->setPen(QPen(Qt::green, 2, Qt::SolidLine));
    graph2->setData(x, hv);
    graph2->setName("Vm");
    QCPGraph *graph3 = customplot->addGraph();  //向绘图区域QCustomPlot添加一条曲线
    graph3->setPen(QPen(Qt::red, 2, Qt::SolidLine));
    graph3->setData(x, hw);
    graph3->setName("Vsp");
    QCPGraph *graph4 = customplot->addGraph();  //向绘图区域QCustomPlot添加一条曲线
    graph4->setPen(QPen(Qt::blue, 2, Qt::SolidLine));
    graph4->setData(x, hn);
    graph4->setName("Load");

    QLinearGradient gradient(0, 0, 0, 400);
    gradient.setColorAt(0, QColor(90, 90, 90));
    gradient.setColorAt(0.38, QColor(105, 105, 105));
    gradient.setColorAt(1, QColor(70, 70, 70));
    customplot->setBackground(QBrush(gradient));

    //设置坐标轴显示范围，否则只能看到默认范围
    customplot->xAxis->setRange(0, t);
    customplot->yAxis->setRange(0, 51);
    customplot->xAxis->grid()->setVisible(false);
    customplot->yAxis->grid()->setVisible(false);
    customplot->xAxis->setTicks(false);
    customplot->yAxis->setTicks(false);
    customplot->axisRect()->setMinimumMargins(QMargins(0, 0, 0, 0));
    customplot->axisRect()->setupFullAxesBox();

    customplot->replot();
}


void ConfLoadTesting::ruler(double x)
{
    QVector<double> xx(100), yy(100);
    for (int i=0; i < 100; i++) {
        xx[i] = x;
        yy[i] = i%40;
    }
    QCPGraph *rulers = customplot->addGraph();  //向绘图区域QCustomPlot添加一条曲线
    rulers->setPen(QPen(Qt::white, 1, Qt::DashDotDotLine));
    rulers->setData(xx, yy);
}

void ConfLoadTesting::wavePacket(double x1, double x2, QString name)
{
    QCPItemBracket *bracket = new QCPItemBracket(customplot);
    bracket->left->setCoords(x1, 40);
    bracket->right->setCoords(x2, 40);
    bracket->setLength(13);
    bracket->setPen(QPen(Qt::white));

    QCPItemText *wavePacketText = new QCPItemText(customplot);
    wavePacketText->position->setParentAnchor(bracket->center);
    wavePacketText->position->setCoords(0, -10);
    wavePacketText->setPositionAlignment(Qt::AlignBottom|Qt::AlignHCenter);
    wavePacketText->setText(name);
    wavePacketText->setFont(QFont(font().family(), 10));
    wavePacketText->setColor(QColor(Qt::white));
}

void ConfLoadTesting::recvAppShow(QString win)
{
    if (win != this->objectName())
        return;
    emit sendNetMsg("6004 LOAD");
}
