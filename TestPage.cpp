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
    countOk = 0;
    countAll = 0;
    codeTimer = new QTimer(this);
    connect(codeTimer, SIGNAL(timeout()), this, SLOT(showCode()));
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
    judge->setText("ON");
    judge->setStyleSheet("font:55pt;color:yellow");
    QVector<double> x(100), y(100);
    for (int i=0; i < 100; i++) {
        x[i] = -1;
        y[i] = -1;
    }
    graph1->setData(x, y);
    graph2->setData(x, y);
    graph3->setData(x, y);
    graph4->setData(x, y);
    graph5->setData(x, y);
    graph6->setData(x, y);
    wave->legend->setVisible(false);
    waveU.clear();
    waveV.clear();
    waveW.clear();
    waveHu.clear();
    waveHv.clear();
    waveHw.clear();
    wave->replot();
    textBemf.clear();
    textLoad.clear();
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

void TestPage::updateItem(QString item)
{
    int t = 0;
    for (int i=0; i < mView->rowCount(); i++) {
        if (!mView->item(i, 2)->text().isEmpty()) {
            t = i;
        }
    }
    mView->item(t, 2)->setText(item);
}

bool TestPage::updateJudge(QString item)
{
    int t = 0;
    for (int i=0; i < mView->rowCount(); i++) {
        if (!mView->item(i, 3)->text().isEmpty()) {
            t = i;
        }
    }
    mView->item(t, 3)->setText(item);
    if (item == "OK")
        mView->item(t, 3)->setForeground(QBrush(QColor(Qt::green)));
    else
        mView->item(t, 3)->setForeground(QBrush(QColor(Qt::red)));
    return true;
}

bool TestPage::updateResult(quint16 s)
{
    countAll++;
    if (s != 0) {
        judge->setStyleSheet("font:55pt;color:red");
        judge->setText("NG");
        return false;
    }
    for (int i=0; i < mView->rowCount(); i++) {
        if (mView->item(i, 3)->text() != "OK") {
            judge->setStyleSheet("font:55pt;color:red");
            judge->setText("NG");
            qDebug() << i;
            return false;
        }
    }
    countOk++;
    judge->setStyleSheet("font:55pt;color:green");
    judge->setText("OK");
    qDebug() << countAll << countOk;
    DrawHistogram();
    return true;
}

void TestPage::updateTemp(QString s)
{
    textTemp->setText(s);
}

QString TestPage::readResult()
{
    QString temp;
    for (int i=0; i < mView->rowCount(); i++) {
        temp.append(mView->item(i, 0)->text());
        temp.append("@");
        temp.append(mView->item(i, 1)->text());
        temp.append("@");
        temp.append(mView->item(i, 2)->text());
        temp.append("@");
        temp.append(mView->item(i, 3)->text());
        temp.append("\n");
    }
    textNumb->setText(QString("编码:__"));
    return temp;
}

void TestPage::updateWave(QString w)
{
    if (w.startsWith("0 ")) {
        waveU = w.split(" ");
        waveU.removeFirst();
        exportToCsv("U", waveU);
    } else if (w.startsWith("1 ")) {
        waveV = w.split(" ");
        waveV.removeFirst();
        exportToCsv("V", waveV);
    } else if (w.startsWith("2 ")) {
        waveW = w.split(" ");
        waveW.removeFirst();
        exportToCsv("W", waveW);
    } else if (w.startsWith("3 ")) {
        waveHu = w.split(" ");
        waveHu.removeFirst();
        exportToCsv("Hu", waveHu);
    } else if (w.startsWith("4 ")) {
        waveHv = w.split(" ");
        waveHv.removeFirst();
        exportToCsv("Hv", waveHv);
    } else if (w.startsWith("5 ")) {
        waveHw = w.split(" ");
        waveHw.removeFirst();
        exportToCsv("Hw", waveHw);
    }
    DrawWave();
}

void TestPage::setTextBemf(QString s)
{
    textBemf = s;
}

void TestPage::setTextLoad(QString s)
{
    textLoad = s;
}

void TestPage::setTextHall(QString s)
{
    textHall = s;
}

QString TestPage::readNumb()
{
    QString s = textNumb->text().remove("编码:");
    return s.remove("@");
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
    view->setEditTriggers(QAbstractItemView::NoEditTriggers);
    view->horizontalHeader()->setSectionResizeMode(2, QHeaderView::Stretch);
    view->setColumnWidth(0, 120);
    view->setColumnWidth(1, 400);
    view->setColumnWidth(3, 120);
    view->setFocusPolicy(Qt::NoFocus);
    connect(view, SIGNAL(clicked(QModelIndex)), this, SLOT(clickView()));

    wave = new QCustomPlot(this);
    wave->setBackground(QBrush(QColor(25, 25, 25))); //设置背景色
    wave->xAxis->grid()->setPen(QPen(Qt::darkGreen, 1, Qt::DotLine));
    wave->yAxis->grid()->setPen(QPen(Qt::darkGreen, 1, Qt::DotLine));
    wave->xAxis->setTicks(false);
    wave->yAxis->setTicks(false);
    wave->xAxis->setTickLabels(false);
    wave->yAxis->setTickLabels(false);
    wave->axisRect()->setMinimumMargins(QMargins(0, 0, 0, 0));
    wave->xAxis->setBasePen(Qt::NoPen);
    wave->yAxis->setBasePen(Qt::NoPen);
    wave->xAxis2->setBasePen(Qt::NoPen);
    wave->yAxis2->setBasePen(Qt::NoPen);
    wave->xAxis->setRange(0, 400);
    wave->yAxis->setRange(0, 101);
    graph1 = wave->addGraph();
    graph1->setName("U");
    graph1->setPen(QPen(Qt::cyan, 2));

    graph2 = wave->addGraph();
    graph2->setName("V");
    graph2->setPen(QPen(Qt::yellow, 2));

    graph3 = wave->addGraph();
    graph3->setName("W");
    graph3->setPen(QPen(Qt::green, 2));

    graph4 = wave->addGraph();
    graph4->setName("HA");
    graph4->setPen(QPen(Qt::cyan, 2));

    graph5 = wave->addGraph();
    graph5->setName("HB");
    graph5->setPen(QPen(Qt::yellow, 2));

    graph6 = wave->addGraph();
    graph6->setName("HC");
    graph6->setPen(QPen(Qt::green, 2));

    QHBoxLayout *waveLayout = new QHBoxLayout;
    waveLayout->addWidget(wave);
    waveLayout->setMargin(0);

    QGroupBox *waveBox = new QGroupBox;
    waveBox->setLayout(waveLayout);

    QHBoxLayout *textLayout = new QHBoxLayout;
    textType = new QLabel("型号:__", this);
    textNumb = new QLabel("编码:__", this);
    textUser = new QLabel("操作员:__", this);
    textTemp = new QLabel("温度:__", this);
    textLayout->addWidget(textType);
    textLayout->addWidget(textNumb);
    textLayout->addWidget(textUser);
    textLayout->addWidget(textTemp);

    QVBoxLayout *mLayout = new QVBoxLayout;
    mLayout->addWidget(view);
    mLayout->addWidget(waveBox);
    mLayout->addLayout(textLayout);
    mLayout->setStretch(0, 6);
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

    QPushButton *btnStop = new QPushButton("停止测试", this);
    btnStop->setMinimumSize(97, 44);
    connect(btnStop, SIGNAL(clicked(bool)), this, SIGNAL(buttonStop()));

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
    histogram = new QCustomPlot(this);
    bars1 = new QCPBars(histogram->xAxis, histogram->yAxis);
    bars2 = new QCPBars(histogram->xAxis, histogram->yAxis);
    bars3 = new QCPBars(histogram->xAxis, histogram->yAxis);
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
    station->hide();

    judge = new QLabel(this);
    judge->setStyleSheet("font:55pt;color:green");
    judge->setText("OK");
    judge->setAlignment(Qt::AlignCenter);

    QGridLayout *tLayout = new QGridLayout;
    tLayout->addWidget(btnHome, 0, 0, 1, 2);
    tLayout->addWidget(btnConf, 1, 0, 1, 2);
    tLayout->addWidget(btnTest, 2, 0, 1, 2);
    tLayout->addWidget(btnStop, 3, 0, 1, 2);
    tLayout->addWidget(btnLogo, 4, 0, 1, 2);
    tLayout->addWidget(histogram, 5, 0, 1, 2);
    tLayout->setRowStretch(5, 2);
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

    box = new PopupBox(this, "", "测试", QMessageBox::Ok);
    box->setWindowFlags(Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint | Qt::Popup);
    box->setStyleSheet("QDialog{border:2px solid cyan;}");
    box->resize(QSize(1024, 768));
    box->hide();
}

void TestPage::initSettings()
{
    //当前使用的测试项目
    QString n = QString("./config/%1.ini").arg(CurrentSettings());
    QSettings *ini = new QSettings(n, QSettings::IniFormat);
    ini->setIniCodec("GB18030");

    QStringList WireColor = ini->value("/Conf/color", "").toString().split(",");
    for (int i=0; i < qMin(colors.size(),WireColor.size()); i++) {
        colors.at(i)->setStyleSheet(QString("background-color:%1").arg(WireColor.at(i)));
    }
    textType->setText(QString("型号:%1").arg(CurrentSettings()));
    textUser->setText(QString("操作员:%1").arg(currentUser()));
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

void TestPage::clickView()
{
    int row = view->currentIndex().row();
    if (row < 0)
        return;
    if (mView->item(row, 0)->text().contains(tr("空载"))) {
        box->setText(textLoad);
        box->show();
    }
    if (mView->item(row, 0)->text().contains(tr("霍尔"))) {
        box->setText(textHall);
        box->show();
    }
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
    //    histogram->clearGraphs();
    //    histogram->clearFocus();
    //    histogram->clearItems();
    //    histogram->clearMask();
    //    histogram->clearPlottables();
    // prepare data:
    QVector<double> x1(1), y1(1);
    QVector<double> x2(1), y2(1);
    QVector<double> x3(1), y3(1);

    x1[0] = 1;
    x2[0] = 2;
    x3[0] = 3;
    if (countAll == 0) {
        y1[0] = 100;
        y2[0] = 100;
        y3[0] = 1;
        histogram->yAxis->setRange(0, 101);
    } else {
        y1[0] = countAll;
        y2[0] = countOk;
        y3[0] = countAll - countOk;
        histogram->yAxis->setRange(0, countAll+1);
    }

    bars1->setWidth(0.9);
    bars1->setData(x1, y1);
    bars1->setPen(Qt::NoPen);
    bars1->setBrush(QColor("blue"));

    bars2->setWidth(0.9);
    bars2->setData(x2, y2);
    bars2->setPen(Qt::NoPen);
    bars2->setBrush(QColor("green"));

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

    QVector<double> ticks;
    QVector<QString> labels;
    ticks << 1 << 2 << 3;
    labels << "ALL" << "OK" << "NG";
    QSharedPointer<QCPAxisTickerText> textTicker(new QCPAxisTickerText);
    textTicker->addTicks(ticks, labels);
    histogram->xAxis->setTicker(textTicker);
    histogram->xAxis->setTickLength(0, 1);  // x轴标尺步长

    histogram->xAxis->setBasePen(Qt::NoPen);
    histogram->yAxis->setBasePen(Qt::NoPen);
    histogram->xAxis2->setBasePen(Qt::NoPen);
    histogram->yAxis2->setBasePen(Qt::NoPen);
    histogram->xAxis->grid()->setVisible(false);
    histogram->yAxis->grid()->setVisible(false);
    histogram->xAxis->setTickLabelColor(Qt::white);
    histogram->yAxis->setTicks(false);
    histogram->xAxis->setLabelColor(Qt::white);
    histogram->setBackground(QBrush(QColor(25, 25, 25))); //设置背景色
    histogram->axisRect()->setMinimumMargins(QMargins(0, 0, 0, 0));
    histogram->rescaleAxes();
    histogram->xAxis->setRange(0.5, 3.5);  // x轴范围

    histogram->replot();
}
void TestPage::DrawWave()
{
    int len = 100;
    len = qMax(len, waveU.size());
    len = qMax(len, waveV.size());
    len = qMax(len, waveW.size());
    len = qMax(len, waveHu.size());
    len = qMax(len, waveHv.size());
    len = qMax(len, waveHw.size());
    int t = readScale().toInt();
    QVector<double> ux(waveU.size()),uy(waveU.size());
    for (int i=0; i < waveU.size(); i++) {
        ux[i] = i;
        uy[i] = (QString(waveU.at(i)).toDouble()-128)*t/100+800;
    }
    graph1->setData(ux, uy);

    QVector<double> vx(waveV.size()),vy(waveV.size());
    for (int i=0; i < waveV.size(); i++) {
        vx[i] = i;
        vy[i] = (QString(waveV.at(i)).toDouble()-128)*t/100+800;
    }
    graph2->setData(vx, vy);

    QVector<double> wx(waveW.size()),wy(waveW.size());
    for (int i=0; i < waveW.size(); i++) {
        wx[i] = i;
        wy[i] = (QString(waveW.at(i)).toDouble()-128)*t/100+800;
    }
    graph3->setData(wx, wy);

    QVector<double> hux(waveHu.size()),huy(waveHu.size());
    for (int i=0; i < waveHu.size(); i++) {
        hux[i] = i;
        huy[i] = QString(waveHu.at(i)).toDouble()*180/256+400;
    }
    graph4->setData(hux, huy);

    QVector<double> hvx(waveHv.size()),hvy(waveHv.size());
    for (int i=0; i < waveHv.size(); i++) {
        hvx[i] = i;
        hvy[i] = QString(waveHv.at(i)).toDouble()*180/256+200;
    }
    graph5->setData(hvx, hvy);

    QVector<double> hwx(waveHw.size()),hwy(waveHw.size());
    for (int i=0; i < waveHw.size(); i++) {
        hwx[i] = i;
        hwy[i] = QString(waveHw.at(i)).toDouble()*180/256+0;
    }
    graph6->setData(hwx, hwy);

    wave->xAxis->setBasePen(Qt::NoPen);
    wave->yAxis->setBasePen(Qt::NoPen);
    wave->xAxis2->setBasePen(Qt::NoPen);
    wave->yAxis2->setBasePen(Qt::NoPen);
    wave->xAxis->setRange(0, len);
    wave->yAxis->setRange(0, 1000);
    wave->setBackground(QBrush(QColor(25, 25, 25)));
    wave->legend->setVisible(true);
    wave->replot();
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

void TestPage::exportToCsv(QString title, QStringList wave)
{
    QFile file(QString("wave-%1.csv").arg(QDate::currentDate().toString("yy-MM-dd")));
    if (!file.open(QFile::WriteOnly | QIODevice::Append)) {
        QMessageBox::warning(this,  "",   "创建失败",  QMessageBox::Ok);
        return;
    }
    file.write(QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss").toUtf8());
    file.write(",");
    file.write(title.toUtf8());
    file.write(",");
    file.write(wave.join(" ").toUtf8());
    file.write("\n");
    file.close();
}

QString TestPage::CurrentSettings()
{
    QSettings *ini = new QSettings("./nandflash/global.ini", QSettings::IniFormat);
    QString n = ini->value("/GLOBAL/FileInUse", "Base_File").toString();
    return n.remove(".ini");
}

QString TestPage::currentUser()
{
    QSettings *ini = new QSettings("./nandflash/global.ini", QSettings::IniFormat);
    QString temp = ini->value("/GLOBAL/User", "0").toString();;
    if (temp == "0")
        return "guest";
    else
        return "admin";
}

QString TestPage::readScale()
{
    QSettings *ini = new QSettings("./nandflash/global.ini", QSettings::IniFormat);
    QString temp = ini->value("/SCALE/bemf", "100").toString();
    return temp;
}

void TestPage::showCode()
{
    codeTimer->stop();
    //    if (code.size() < 2)
    //        return;
    textNumb->setText(QString("编码:%1").arg(code));
    code.clear();
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
    initSettings();
}
void TestPage::keyPressEvent(QKeyEvent *e)
{
    this->setFocus();
    codeTimer->stop();
    code.append(e->text());
    e->accept();
}

void TestPage::keyReleaseEvent(QKeyEvent *e)
{
    codeTimer->start(10);
    e->accept();
}
