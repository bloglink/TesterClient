/*******************************************************************************
 * Copyright [2017]   <青岛艾普智能仪器有限公司>
 * All rights reserved.
 *
 * version:     0.0.0.1
 * author:      zhaonanlin
 * brief:       空载配置
*******************************************************************************/
#include "conf_noloadtest.h"

ConfNoLoadTest::ConfNoLoadTest(QWidget *parent) : QWidget(parent)
{
    initUI();
}

ConfNoLoadTest::~ConfNoLoadTest()
{
}

void ConfNoLoadTest::initSettings(QJsonObject obj)
{
    QStringList items = itemNames;
    items << "sequence" << "turn";
    for (int i=0; i < items.size(); i++) {
        QStringList temp = obj.value(items.at(i)).toString().split(",");
        if (items.at(i) == "sequence") {
            for (int t=0; t < temp.size(); t++)
                tModel->item(0, t)->setText(temp.at(t));
        } else if (items.at(i) == "turn") {
            if (temp.size() < 2)
                continue;
            if (temp.at(0) == "0")
                turnCheckBox->setChecked(false);
            else
                turnCheckBox->setChecked(true);
            turnComboBox->setCurrentIndex(temp.at(1).toInt());
        } else {
            for (int t=0; t < temp.size(); t++)
                mView->item(t, i)->setText(temp.at(t));
        }
    }
}

void ConfNoLoadTest::readSettings()
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
    QStringList temp;
    for (int i=0; i < tModel->columnCount(); i++) {
        double x = tModel->item(0, i)->text().toDouble();
        temp.append(QString::number(x));
    }
    obj.insert("sequence", temp.join(","));

    temp.clear();
    if (turnCheckBox->isChecked())
        temp.append("1");
    else
        temp.append("0");
    temp.append(QString::number(turnComboBox->currentIndex()));
    obj.insert("turn", temp.join(","));

    QJsonObject array;
    array.insert("NOLOAD", obj);
    emit sendAppCmd(array);
}

QStringList ConfNoLoadTest::readLimit()
{
    QStringList items = itemNames;
    QStringList temp;
    for (int i=0; i < items.size(); i++) {
        for (int t=0; t < mView->rowCount(); t++) {
            double x = mView->item(t, i)->text().toDouble();
            temp.append(QString::number(x));
        }
    }
    if (turnCheckBox->isChecked())
        temp.append("1");
    else
        temp.append("0");
    temp.append(QString::number(turnComboBox->currentIndex()));
    return temp;
}

void ConfNoLoadTest::initUI()
{
    this->setObjectName("ConfNoLoadTest");
    QStringList headers;
    headers << tr("电压") << tr("电流下限") << tr("电流上限")
            << tr("功率下限") << tr("功率上限")
            << tr("转速下限") << tr("转速上限") << tr("Vcc电压")
            << tr("Vsp电压") << tr("测试时间") << tr("驱动器") << tr("电源");
    itemNames << "volt" << "curr_min" << "curr_max"
              << "pwr_min" << "pwr_max"
              << "speed_min" << "speed_max"
              << "vcc_volt" << "vsp_volt" << "time" << "driver" << "power";
    mView = new StandardItem(1, headers.size());
    mView->setHorizontalHeaderLabels(headers);
    for (int i=0; i < 1; i++) {
        for (int j=0; j < headers.size(); j++) {
            mView->setData(mView->index(i, j), "");
        }
    }
    DDoubleSpinBox *voltage = new DDoubleSpinBox;
    voltage->setMaxinum(500);
    voltage->setDecimals(1);
    DDoubleSpinBox *current = new DDoubleSpinBox;
    current->setMaxinum(5);
    current->setDecimals(3);
    DDoubleSpinBox *power = new DDoubleSpinBox;
    power->setMaxinum(5000);
    power->setDecimals(1);
    SpinBox *speed = new SpinBox;
    speed->setMaxinum(5000);
    DoubleSpinBox *vcc = new DoubleSpinBox;
    vcc->setMaxinum(15);
    DoubleSpinBox *vsp = new DoubleSpinBox;
    vsp->setMaxinum(15);
    DoubleSpinBox *time = new DoubleSpinBox;
    time->setMaxinum(99);
    ComboBox *dirver = new ComboBox;
    QStringList mode;
    mode << "0"<< "1" << "2";
    dirver->setItemNames(mode);
    view = new QTableView(this);
    view->setModel(mView);
    view->setItemDelegateForColumn(0, voltage);
    view->setItemDelegateForColumn(1, current);
    view->setItemDelegateForColumn(2, current);
    view->setItemDelegateForColumn(3, power);
    view->setItemDelegateForColumn(4, power);
    view->setItemDelegateForColumn(5, speed);
    view->setItemDelegateForColumn(6, speed);
    view->setItemDelegateForColumn(7, vcc);
    view->setItemDelegateForColumn(8, vsp);
    view->setItemDelegateForColumn(9, time);
    view->setItemDelegateForColumn(10, dirver);
    view->setItemDelegateForColumn(11, dirver);
    view->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    view->verticalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    view->hideColumn(10);
    view->hideColumn(11);

    turnCheckBox = new QCheckBox(this);
    turnCheckBox->setText(tr("空载转向"));
    turnComboBox = new QComboBox(this);
    turnComboBox->addItem(tr("CCW"));
    turnComboBox->addItem(tr("CW"));
    turnComboBox->setMinimumSize(97, 35);
    turnComboBox->setView(new QListView);

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
    btnLayout->addWidget(btnSave);
    btnLayout->addWidget(btnExit);

    QStringList header;
    header << tr("T0") << tr("T1") << tr("T2") << tr("T3") << tr("T4")
           << tr("T5") << tr("T6") << tr("T7") << tr("T8") << tr("T9");
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
    plot->setTitle(tr("空载上电时序"));

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
    layout->setStretch(1, 1);
    layout->setStretch(2, 3);
    this->setLayout(layout);
}

void ConfNoLoadTest::sequence()
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

    double T5 = 100;
    double T6 = 50;
    double T7 = 100;
    double T8 = 50;
    double T9 = 100;

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

    double t = (T0+T1+T2+T3+T4+T6+T7+T8+T9)*2;
    double Ts = t/20;
    double Te = t/15;

    double t0 = Ts;
    double t1 = Ts+T0;
    double t2 = Ts+T0+T1;
    double t3 = Ts+T0+T1+T2;
    double t4 = Ts+T0+T1+T2+T3;
    double t5 = Ts+T0+T1+T2+T3+T4;

    double t6 = t-Te-T9-T8-T7-T6-T5;
    double t7 = t-Te-T9-T8-T7-T6;
    double t8 = t-Te-T9-T8-T7;
    double t9 = t-Te-T9-T8;
    double t10 = t-Te-T9;
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

    wavePacket(t5, t6, "测试时间");

    ruler(t6);
    wavePacket(t6, t7, "T5");
    ruler(t7);
    wavePacket(t7, t8, "T6");
    ruler(t8);
    wavePacket(t8, t9, "T7");
    ruler(t9);
    wavePacket(t9, t10, "T8");
    ruler(t10);
    wavePacket(t10, t11, "T9");
    ruler(t11);

    customplot->legend->clearItems();

    QVector<double> x(t);
    QVector<double> hu(t), hv(t), hw(t);

    for (int i=0; i < t; i++) { //添加数据
        x[i] = i;
        if (i < t0)
            hu[i] = 0;
        else if (i < t1)
            hu[i] = (i-t0)*(35/T0);
        else if (i < t10)
            hu[i] = 35;
        else if (i < t11)
            hu[i] = (t11-i)*(35/T9);
        else
            hu[i] = 0;

        if (i < t2)
            hv[i] = 0;
        else if (i < t3)
            hv[i] = (i-t2)*(40/T2);
        else if (i < t8)
            hv[i] = 40;
        else if (i < t9)
            hv[i] = (t9-i)*(40/T7);
        else
            hv[i] = 0;

        if (i < t4)
            hw[i] = 0;
        else if (i < t5)
            hw[i] = (i-t4)*(30/T4);
        else if (i < t6)
            hw[i] = 30;
        else if (i < t7)
            hw[i] = (t7-i)*(30/T5);
        else
            hw[i] = 0;
    }

    QCPGraph *graph1 = customplot->addGraph();
    graph1->setPen(QPen(Qt::yellow, 2, Qt::SolidLine));
    graph1->setData(x, hu);
    graph1->setName("Vcc");
    QCPGraph *graph2 = customplot->addGraph();
    graph2->setPen(QPen(Qt::green, 2, Qt::SolidLine));
    graph2->setData(x, hv);
    graph2->setName("Vm");
    QCPGraph *graph3 = customplot->addGraph();
    graph3->setPen(QPen(Qt::red, 2, Qt::SolidLine));
    graph3->setData(x, hw);
    graph3->setName("Vsp");

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

void ConfNoLoadTest::ruler(double x)
{
    QVector<double> xx(100), yy(100);
    for (int i=0; i < 100; i++) {
        xx[i] = x;
        yy[i] = i%40;
    }
    QCPGraph *rulers = customplot->addGraph();
    rulers->setPen(QPen(Qt::white, 1, Qt::DashDotDotLine));
    rulers->setData(xx, yy);
}

void ConfNoLoadTest::wavePacket(double x1, double x2, QString name)
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

void ConfNoLoadTest::back()
{
    emit buttonClicked(NULL);
}
