/*******************************************************************************
 * Copyright [2017]   <青岛艾普智能仪器有限公司>
 * All rights reserved.
 *
 * version:     0.0.0.1
 * author:      zhaonanlin
 * brief:       测试界面
*******************************************************************************/
#include "TestPage.h"

TestPage::TestPage(QWidget *parent) : QWidget(parent)
{
    initUI();
}

TestPage::~TestPage()
{
}

void TestPage::initItems(quint16 s)
{
    for (int i=0; i < mView->rowCount(); i++) {
        mView->item(i, 2)->setText("");
        mView->item(i, 3)->setText("");
    }
    if (s == 0x13)
        station->setText("左");
    else
        station->setText("右");
    judge->setText("--");
}

void TestPage::updateItems(QString dat)
{
    QDomDocument docs;
    docs.setContent(dat);
    if (!docs.elementsByTagName("Test_Data_Param").isEmpty()) {
        mView->setRowCount(0);
        QDomNodeList list = docs.elementsByTagName("Test_Data_Param").at(0).childNodes();
        for (int i=0; i < list.size(); i++) {
            QDomElement dom = list.at(i).toElement();
            QStringList temp = dom.text().split(",");
            if (dom.nodeName() == "Test_1") {
                for (int i=0; i < temp.size(); i++) {
                    mView->appendRow(new QStandardItem(temp.at(i)));
                    mView->setItem(i, 1, new QStandardItem);
                    mView->setItem(i, 2, new QStandardItem);
                    mView->setItem(i, 3, new QStandardItem);
                }
            }
            if (dom.nodeName() == "Test_2") {
                for (int i=0; i < qMin(temp.size(), mView->rowCount()); i++)
                    mView->setItem(i, 1, new QStandardItem(temp.at(i)));
            }
        }
    }
    if (!docs.elementsByTagName("Test_Data_Result").isEmpty()) {
        QDomNodeList list = docs.elementsByTagName("Test_Data_Result").at(0).childNodes();
        for (int i=0; i < list.size(); i++) {
            QDomElement dom = list.at(i).toElement();
            QString temp = dom.text();
            if (dom.nodeName() == "Test_3") {
                for (int i=0; i < mView->rowCount(); i++) {
                    if (mView->item(i, 3)->text().isEmpty()) {
                        mView->item(i, 2)->setText(temp);
                        break;
                    }
                }
            }
        }
    }
    if (!docs.elementsByTagName("Test_Data_Judge").isEmpty()) {
        QDomNodeList list = docs.elementsByTagName("Test_Data_Judge").at(0).childNodes();
        for (int i=0; i < list.size(); i++) {
            QDomElement dom = list.at(i).toElement();
            QString temp = dom.text();
            if (dom.nodeName() == "Test_3") {
                for (int i=0; i < mView->rowCount(); i++) {
                    if (mView->item(i, 3)->text().isEmpty()) {
                        mView->item(i, 3)->setText(temp);
                        if (temp == "OK")
                            mView->item(i, 3)->setForeground(QBrush(QColor(Qt::green)));
                        else
                            mView->item(i, 3)->setForeground(QBrush(QColor(Qt::red)));
                        break;
                    }
                }
            }
        }
    }
}

bool TestPage::updateResult()
{
    for (int i=0; i < mView->rowCount(); i++) {
        if (mView->item(i, 3)->text() != "OK") {
            judge->setStyleSheet("font:55pt;color:red");
            judge->setText("NG");
            return false;
        }
    }
    judge->setStyleSheet("font:55pt;color:green");
    judge->setText("OK");
    return true;
}

void TestPage::initUI()
{
    this->setObjectName("TestPage");
    QStringList headers;
    headers << tr("测试项目") << tr("测试参数") << tr("测试结果") << tr("测试判定");
    mView = new StandardItem(0, headers.size());
    mView->setHorizontalHeaderLabels(headers);

    view = new QTableView(this);
    view->setModel(mView);
    view->horizontalHeader()->setSectionResizeMode(2, QHeaderView::Stretch);
    view->setColumnWidth(0, 150);
    view->setColumnWidth(1, 450);
    view->setColumnWidth(3, 150);

    wave = new QCustomPlot(this);
    wave->setBackground(QBrush(QColor(25, 25, 25))); //设置背景色
    wave->xAxis->grid()->setPen(QPen(Qt::darkGreen, 1, Qt::DotLine));
    wave->yAxis->grid()->setPen(QPen(Qt::darkGreen, 1, Qt::DotLine));
    wave->xAxis->setTicks(false);
    wave->yAxis->setTicks(false);
    wave->xAxis->setTickLabels(false);
    wave->yAxis->setTickLabels(false);
    wave->axisRect()->setMinimumMargins(QMargins(0, 0, 0, 0));
    wave->axisRect()->setupFullAxesBox();
    wave->rescaleAxes();
    wave->addGraph();
    wave->xAxis->setRange(0, 400);
    wave->yAxis->setRange(0, 250);
    wave->graph(0)->setPen(QPen(Qt::green));
    DrawWave();

    QHBoxLayout *waveLayout = new QHBoxLayout;
    waveLayout->addWidget(wave);
    waveLayout->setMargin(0);

    QGroupBox *waveBox = new QGroupBox;
    waveBox->setLayout(waveLayout);

    QHBoxLayout *textLayout = new QHBoxLayout;
    textLayout->addWidget(new QLabel("型号:__", this));
    textLayout->addWidget(new QLabel("编号:__", this));
    textLayout->addWidget(new QLabel("操作员:__", this));
    textLayout->addWidget(new QLabel("温度:__", this));

    QVBoxLayout *mLayout = new QVBoxLayout;
    mLayout->addWidget(view);
    mLayout->addWidget(waveBox);
    mLayout->addLayout(textLayout);
    mLayout->setStretch(0, 5);
    mLayout->setStretch(1, 3);

    QGroupBox *mGroup = new QGroupBox(this);
    mGroup->setLayout(mLayout);

    QPushButton *btnHome = new QPushButton("开机主页", this);
    btnHome->setObjectName("HomePage");
    btnHome->setMinimumSize(97, 44);
    connect(btnHome, SIGNAL(clicked(bool)), this, SLOT(windowChange()));

    QPushButton *btnConf = new QPushButton("型号管理", this);
    btnConf->setObjectName("ConfPage");
    btnConf->setMinimumSize(97, 44);
    connect(btnConf, SIGNAL(clicked(bool)), this, SLOT(windowChange()));

    QPushButton *btnTest = new QPushButton("开始测试", this);
    btnTest->setMinimumSize(97, 44);
    connect(btnTest, SIGNAL(clicked(bool)), this, SIGNAL(buttonTest()));

//    QPushButton *btnDCR = new QPushButton("测试电阻", this);
//    btnDCR->setMinimumSize(97, 44);
//    connect(btnDCR, SIGNAL(clicked(bool)), this, SIGNAL(buttonTest1()));

//    QPushButton *btnINR = new QPushButton("测试绝缘", this);
//    btnINR->setMinimumSize(97, 44);
//    connect(btnINR, SIGNAL(clicked(bool)), this, SIGNAL(buttonTest2()));

//    QPushButton *btnACW = new QPushButton("测试交耐", this);
//    btnACW->setMinimumSize(97, 44);
//    connect(btnACW, SIGNAL(clicked(bool)), this, SIGNAL(buttonTest3()));

//    QPushButton *btnIND = new QPushButton("测试电感", this);
//    btnIND->setMinimumSize(97, 44);
//    connect(btnIND, SIGNAL(clicked(bool)), this, SIGNAL(buttonTest4()));

//    QPushButton *btnPWR = new QPushButton("测试空载", this);
//    btnPWR->setMinimumSize(97, 44);
//    connect(btnPWR, SIGNAL(clicked(bool)), this, SIGNAL(buttonTest5()));

//    QPushButton *btnLOD = new QPushButton("测试负载", this);
//    btnLOD->setMinimumSize(97, 44);
//    connect(btnLOD, SIGNAL(clicked(bool)), this, SIGNAL(buttonTest6()));

//    QPushButton *btnPrt = new QPushButton("测试打印", this);
//    btnPrt->setMinimumSize(97, 44);
//    connect(btnPrt, SIGNAL(clicked(bool)), this, SLOT(Printer()));

        QLabel *btnLogo = new QLabel(this);
        btnLogo->setPixmap(QPixmap(":/source/logo.png"));
        btnLogo->setScaledContents(true);
        btnLogo->setMinimumHeight(btnLogo->width());

//    qrencode = new ConfQrenCode(this);
//    qrencode->setMinimumSize(90,90);
//    qrencode->setMaximumSize(90,90);

    DrawHistogram();

    QGridLayout *cLayout = new QGridLayout;
    for (int i=0; i < 8; i++) {
        QLabel *color = new QLabel(QString::number(i+1));
        colors.append(color);
        cLayout->addWidget(color, i/2, i%2);
        color->setAlignment(Qt::AlignCenter);
        color->setStyleSheet("background-color:#191919");
    }

    station = new QLabel(this);
    station->setStyleSheet("font:50pt;color:yellow");
    station->setText("左");
    station->setAlignment(Qt::AlignCenter);

    judge = new QLabel(this);
    judge->setStyleSheet("font:55pt;color:green");
    judge->setText("OK");
    judge->setAlignment(Qt::AlignCenter);

    QGridLayout *tLayout = new QGridLayout;
    tLayout->addWidget(btnHome, 0, 0, 1, 2);
    tLayout->addWidget(btnConf, 1, 0, 1, 2);
    tLayout->addWidget(btnTest, 2, 0, 1, 2);
    tLayout->addWidget(btnLogo, 3, 0, 1, 2);
    tLayout->addWidget(histogram, 4, 0, 1, 2);
    tLayout->setRowStretch(4, 2);
//    tLayout->addWidget(btnDCR, 5, 0, 1, 2);
//    tLayout->addWidget(btnINR, 6, 0, 1, 2);
//    tLayout->addWidget(btnACW, 7, 0, 1, 2);
//    tLayout->addWidget(btnIND, 8, 0, 1, 2);
//    tLayout->addWidget(btnPWR, 9, 0, 1, 2);
//    tLayout->addWidget(btnLOD, 10, 0, 1, 2);
//    tLayout->addWidget(btnPrt, 11, 0, 1, 2);
    tLayout->addLayout(cLayout, 12, 0, 1, 2);
    tLayout->setRowStretch(12, 1);

    tLayout->addWidget(station, 21, 0, 1, 2);
    tLayout->addWidget(judge, 22, 0, 1, 2);

    QGroupBox *tGroup = new QGroupBox(this);
    tGroup->setLayout(tLayout);

    QHBoxLayout *layout = new QHBoxLayout;
    layout->addWidget(mGroup);
    layout->addWidget(tGroup);
    layout->setStretch(0, 7);
    layout->setStretch(1, 1);
    this->setLayout(layout);
}

void TestPage::saveData()
{
    QDomText text;
    QDomDocument doc;
    QDomElement root = doc.createElement("Conf");
    doc.appendChild(root);

    QStringList temp;
    for (int i=0; i < colors.size(); i++) {
        QPalette palette = colors.at(i)->palette();
        temp.append(palette.color(QPalette::Background).name());
    }
    QDomElement color = doc.createElement("color");
    root.appendChild(color);
    text = doc.createTextNode(temp.join(","));
    color.appendChild(text);

    temp.clear();
    temp.append(typeComboBox->currentText());
    QDomElement type = doc.createElement("type");
    root.appendChild(type);
    text = doc.createTextNode(temp.join(","));
    type.appendChild(text);

    emit sendNetMsg(doc.toByteArray());
    emit buttonClicked(NULL);
}

void TestPage::clickButton()
{
    QPushButton *btn = qobject_cast<QPushButton *>(sender());
    for (int i=0; i < buttons.size(); i++) {
        buttons.at(i)->setChecked(false);
    }
    btn->setChecked(true);
    pModel->appendRow(new QStandardItem(btn->text()));
}

void TestPage::showButtons()
{
    if (btnFrame->isHidden()) {
        btnFrame->show();
        btnHide->setIcon(QIcon(":/icons/right.png"));
    } else {
        btnFrame->hide();
        btnHide->setIcon(QIcon(":/icons/left.png"));
    }
}

void TestPage::selectColor()
{
    QPushButton *btn = qobject_cast<QPushButton *>(sender());
    QColor color = QColorDialog::getColor(Qt::white, this);
    if (color.isValid()) {
        btn->setStyleSheet(QString("background-color:%1").arg(color.name()));
    }
}

void TestPage::deleteItem()
{
    int row = pView->currentIndex().row();
    if (row < 0)
        return;
    pModel->removeRow(row);
}

void TestPage::DrawHistogram()
{
    histogram = new QCustomPlot(this);
    histogram->setBackground(QBrush(QColor(25, 25, 25))); //设置背景色
    histogram->yAxis->setTicks(false);
    histogram->yAxis2->setTicks(false);
    histogram->xAxis2->setTicks(false);
    histogram->axisRect()->setMinimumMargins(QMargins(0, 0, 0, 0));
    histogram->axisRect()->setupFullAxesBox();

    // prepare data:
    QVector<double> x1(1), y1(1);
    QVector<double> x2(1), y2(1);
    QVector<double> x3(1), y3(1);

    x1[0] = 1;
    y1[0] = 100;
    x2[0] = 2;
    y2[0] = 100;
    x3[0] = 3;
    y3[0] = 1;

    QCPBars *bars1 = new QCPBars(histogram->xAxis, histogram->yAxis);
    bars1->setWidth(0.9);
    bars1->setData(x1, y1);
    bars1->setPen(Qt::NoPen);
    bars1->setBrush(QColor("blue"));

    QCPBars *bars2 = new QCPBars(histogram->xAxis, histogram->yAxis);
    bars2->setWidth(0.9);
    bars2->setData(x2, y2);
    bars2->setPen(Qt::NoPen);
    bars2->setBrush(QColor("green"));

    QCPBars *bars3 = new QCPBars(histogram->xAxis, histogram->yAxis);
    bars3->setWidth(0.9);
    bars3->setData(x3, y3);
    bars3->setPen(Qt::NoPen);
    bars3->setBrush(QColor("red"));

    QLinearGradient plotGradient;
    plotGradient.setStart(0, 0);
    plotGradient.setFinalStop(0, 350);
    plotGradient.setColorAt(0, QColor(80, 80, 80));
    plotGradient.setColorAt(1, QColor(50, 50, 50));
    histogram->setBackground(plotGradient);

    histogram->rescaleAxes();
    histogram->xAxis->setRange(0, 100);
    histogram->yAxis->setRange(0, 100);

    QVector<double> ticks;
    QVector<QString> labels;
    ticks << 1 << 2 << 3;
    labels << "ALL" << "OK" << "NG";
    QSharedPointer<QCPAxisTickerText> textTicker(new QCPAxisTickerText);
    textTicker->addTicks(ticks, labels);
    histogram->xAxis->setTicker(textTicker);
    histogram->xAxis->setTickLabelRotation(0);
    histogram->xAxis->setTicks(false);  // 下标尺
    histogram->xAxis2->setTicks(false);  // 上标尺
    histogram->xAxis->setSubTicks(false);  // 下细标尺
    histogram->xAxis2->setSubTicks(false);  // 上细标尺
    histogram->xAxis->setTickLength(0, 1);  // x轴标尺步长
    histogram->xAxis->setRange(0.5, 3.5);  // x轴范围
    histogram->xAxis->setBasePen(QPen(Qt::black));
    histogram->xAxis->setTickPen(QPen(Qt::black));
    histogram->xAxis->grid()->setVisible(false);
    histogram->yAxis->grid()->setVisible(false);
    histogram->xAxis->setTickLabelColor(Qt::white);
    histogram->xAxis->setLabelColor(Qt::white);
}
void TestPage::DrawWave()
{
    QLinearGradient gradient(0, 0, 0, 400);
    gradient.setColorAt(0, QColor(18, 25, 34));
    gradient.setColorAt(0.5, QColor(105, 105, 105));
    gradient.setColorAt(1, QColor(18, 25, 34));
    wave->setBackground(QBrush(gradient)); //设置背景色

    QVector<double> x(1080);
    QVector<double> hu(1080), hv(1080), hw(1080);
    QVector<double> uy(1080), vy(1080), wy(1080);
    QVector<double> ay(1080), by(1080);

    for (int i=0; i < 1080; i++) { //添加数据
        x[i] = i;
        hu[i] = qSin((i+30)/PI/18)*20+80;
        hv[i] = qSin((i+270)/PI/18)*20+80;
        hw[i] = qSin((i+150)/PI/18)*20+80;
        uy[i] = ((((i+360+180)/180)%2) == 0) ? 41 : 50;
        vy[i] = ((((i+360+60)/180)%2)  == 0) ? 31 : 40;
        wy[i] = ((((i+360-60)/180)%2)  == 0) ? 21 : 30;
//        ay[i] = ((((i+17)/15)%2) == 0) ? 11 : 20;
//        by[i] = ((((i+9)/15)%2)  == 0) ? 1 : 10;
    }

//    QCPGraph *graph1 = wave->addGraph();
//    graph1->setPen(QPen(Qt::yellow));
//    graph1->setData(x, hu);
//    QCPGraph *graph2 = wave->addGraph();
//    graph2->setPen(QPen(Qt::green));
//    graph2->setData(x, hv);
//    QCPGraph *graph3 = wave->addGraph();
//    graph3->setPen(QPen(Qt::red));
//    graph3->setData(x, hw);
    QCPGraph *graph4 = wave->addGraph();
    graph4->setPen(QPen(Qt::yellow));
    graph4->setData(x, uy);
    QCPGraph *graph5 = wave->addGraph();
    graph5->setPen(QPen(Qt::green));
    graph5->setData(x, vy);
    QCPGraph *graph6 = wave->addGraph();
    graph6->setPen(QPen(Qt::red));
    graph6->setData(x, wy);
    QCPGraph *graph7 = wave->addGraph();
    graph7->setPen(QPen(Qt::yellow));
    graph7->setData(x, ay);
    QCPGraph *graph8 = wave->addGraph();
    graph8->setPen(QPen(Qt::yellow));
    graph8->setData(x, by);

    // set some pens, brushes and backgrounds:
    wave->xAxis->setBasePen(QPen(Qt::white, 1));
    wave->yAxis->setBasePen(QPen(Qt::white, 1));
    wave->xAxis->setTickPen(QPen(Qt::white, 1));
    wave->yAxis->setTickPen(QPen(Qt::white, 1));
    wave->xAxis->setSubTickPen(QPen(Qt::white, 1));
    wave->yAxis->setSubTickPen(QPen(Qt::white, 1));
    wave->xAxis->setTickLabelColor(Qt::white);
    wave->yAxis->setTickLabelColor(Qt::white);
    wave->xAxis->grid()->setPen(QPen(QColor(140, 140, 140), 1, Qt::DotLine));
    wave->yAxis->grid()->setPen(QPen(QColor(140, 140, 140), 1, Qt::DotLine));
    wave->xAxis->grid()->setSubGridPen(QPen(QColor(80, 80, 80), 1, Qt::DotLine));
    wave->yAxis->grid()->setSubGridPen(QPen(QColor(80, 80, 80), 1, Qt::DotLine));
    wave->xAxis->grid()->setSubGridVisible(true);
    wave->yAxis->grid()->setSubGridVisible(true);
    wave->xAxis->grid()->setZeroLinePen(Qt::NoPen);
    wave->yAxis->grid()->setZeroLinePen(Qt::NoPen);
    wave->xAxis->setUpperEnding(QCPLineEnding::esSpikeArrow);
    wave->yAxis->setUpperEnding(QCPLineEnding::esSpikeArrow);

    //设置坐标轴名称
    //    wave->xAxis->setLabel("x");
    //    wave->xAxis->setLabelColor(Qt::white);
    //        wave->yAxis->setLabel("y");

    //设置坐标轴显示范围，否则只能看到默认范围
    wave->xAxis->setRange(0, 360);
    wave->yAxis->setRange(20, 51);
}

void TestPage::Printer()
{
    qrencode->generateString("xxx");
    QPrinter printer;
    // 创建打印预览对话框
    QPrintPreviewDialog preview(&printer, this);
    // 当要生成预览页面时，发射paintRequested()信号
    connect(&preview, SIGNAL(paintRequested(QPrinter*)),
            this,SLOT(printPreview(QPrinter*)));
    preview.exec();
}

void TestPage::printPreview(QPrinter *printer)
{
    QPainter painter(printer);
    QPixmap image;
    image=image.grabWidget(qrencode,0,0,qrencode->width(),qrencode->height());
    painter.drawPixmap(0,0,image);
    //    textEdit->print(printer);
}

void TestPage::windowChange()
{
    emit buttonClicked(QObject::sender()->objectName().toUtf8());
}

void TestPage::recvAppShow(QString win)
{
    if (win != this->objectName())
        return;
    emit sendNetMsg("6008");
    emit sendNetMsg("6004 Sys");
}
