/*******************************************************************************
 * Copyright [2017]   <青岛艾普智能仪器有限公司>
 * All rights reserved.
 *
 * version:     0.0.0.1
 * author:      zhaonanlin
 * brief:       主界面
*******************************************************************************/
#include "MainPage.h"

MainPage::MainPage(QWidget *parent) : QWidget(parent)
{
    initUI();
}

MainPage::~MainPage()
{
}

bool MainPage::login()
{
    LoginPage login;
    connect(&login, SIGNAL(sendJson(QJsonObject)), this, SLOT(initUdp(QJsonObject)));
    connect(this, SIGNAL(sendNetMsg(QString)), &login, SLOT(recvNetMsg(QString)));
    return login.exec();
}

void MainPage::initUI()
{
    this->setWindowTitle("电机综合测试仪V-0.1");
    //设置界面风格
    QFile file;
    QString qss;
    file.setFileName(":/skins/bu_bk.css");
    file.open(QFile::ReadOnly);
    qss = QLatin1String(file.readAll());
    qApp->setStyleSheet(qss);

    HomePage *home = new HomePage(this);
    connect(home, SIGNAL(buttonClicked(QByteArray)), this, SLOT(readButtons(QByteArray)));

    WinSyst *syst = new WinSyst(this);
    connect(syst, SIGNAL(buttonClicked(QByteArray)), this, SLOT(readButtons(QByteArray)));

    WinData *wdat = new WinData(this);
    connect(wdat, SIGNAL(buttonClicked(QByteArray)), this, SLOT(readButtons(QByteArray)));

    conf = new ConfPage(this);
    connect(conf, SIGNAL(buttonClicked(QByteArray)), this, SLOT(readButtons(QByteArray)));
    connect(conf, SIGNAL(sendNetMsg(QByteArray)), &udp, SLOT(recvAppMsg(QByteArray)));
    connect(this, SIGNAL(transmitShow(QString)), conf, SLOT(recvAppShow(QString)));

    test = new TestPage(this);
    connect(test, SIGNAL(buttonClicked(QByteArray)), this, SLOT(readButtons(QByteArray)));
    connect(test, SIGNAL(sendNetMsg(QByteArray)), &udp, SLOT(recvAppMsg(QByteArray)));
    connect(this, SIGNAL(transmitShow(QString)), test, SLOT(recvAppShow(QString)));
    connect(test, SIGNAL(buttonTest()), this, SLOT(testThread()));
    connect(test, SIGNAL(buttonTest1()), this, SLOT(testDCR()));
    connect(test, SIGNAL(buttonTest2()), this, SLOT(testINR()));

    dcr = new ConfigDCR(this);
    connect(dcr, SIGNAL(buttonClicked(QByteArray)), this, SLOT(readButtons(QByteArray)));
    connect(dcr, SIGNAL(sendNetMsg(QByteArray)), &udp, SLOT(recvAppMsg(QByteArray)));
    connect(this, SIGNAL(transmitShow(QString)), dcr, SLOT(recvAppShow(QString)));

    acw = new ConfigACW(this);
    connect(acw, SIGNAL(buttonClicked(QByteArray)), this, SLOT(readButtons(QByteArray)));
    connect(acw, SIGNAL(sendNetMsg(QByteArray)), &udp, SLOT(recvAppMsg(QByteArray)));
    connect(this, SIGNAL(transmitShow(QString)), acw, SLOT(recvAppShow(QString)));

    inr = new ConfigIR(this);
    connect(inr, SIGNAL(buttonClicked(QByteArray)), this, SLOT(readButtons(QByteArray)));
    connect(inr, SIGNAL(sendNetMsg(QByteArray)), &udp, SLOT(recvAppMsg(QByteArray)));
    connect(this, SIGNAL(transmitShow(QString)), inr, SLOT(recvAppShow(QString)));

    ind = new ConfigIND(this);
    connect(ind, SIGNAL(buttonClicked(QByteArray)), this, SLOT(readButtons(QByteArray)));
    connect(ind, SIGNAL(sendNetMsg(QByteArray)), &udp, SLOT(recvAppMsg(QByteArray)));
    connect(this, SIGNAL(transmitShow(QString)), ind, SLOT(recvAppShow(QString)));

    ConfigPWR *pwr = new ConfigPWR(this);
    connect(pwr, SIGNAL(buttonClicked(QByteArray)), this, SLOT(readButtons(QByteArray)));
    connect(pwr, SIGNAL(sendNetMsg(QByteArray)), &udp, SLOT(recvAppMsg(QByteArray)));

    ConfigLoad *load = new ConfigLoad(this);
    connect(load, SIGNAL(buttonClicked(QByteArray)), this, SLOT(readButtons(QByteArray)));
    connect(load, SIGNAL(sendNetMsg(QByteArray)), &udp, SLOT(recvAppMsg(QByteArray)));

    ConfigFG *fg = new ConfigFG(this);
    connect(fg, SIGNAL(buttonClicked(QByteArray)), this, SLOT(readButtons(QByteArray)));
    connect(fg, SIGNAL(sendNetMsg(QByteArray)), &udp, SLOT(recvAppMsg(QByteArray)));

    stack = new QStackedWidget(this);
    stack->addWidget(home);
    stack->addWidget(syst);
    stack->addWidget(wdat);
    stack->addWidget(conf);
    stack->addWidget(test);
    stack->addWidget(dcr);
    stack->addWidget(acw);
    stack->addWidget(inr);
    stack->addWidget(ind);
    stack->addWidget(pwr);
    stack->addWidget(load);
    stack->addWidget(fg);
    readButtons("HomePage");

    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->addWidget(stack);
    layout->setMargin(0);
}

void MainPage::initPLC()
{
    plc = new PLCPage(this);
}

void MainPage::initUdp(QJsonObject obj)
{
    QString host = obj.value("host_addr").toString();
    quint16 port = obj.value("host_port").toString().toInt();
    udp.initSocket(host, port);
    connect(this, SIGNAL(transmitJson(QJsonObject)), &udp, SLOT(recvAppJson(QJsonObject)));
    connect(&udp, SIGNAL(sendNetMsg(QString)), this, SLOT(recvNetMsg(QString)));
    emit transmitJson(obj);
}

void MainPage::recvNetMsg(QString msg)
{
    int a = msg.indexOf(" ");
    int cmd = msg.mid(0, a).toInt();
    QString dat = msg.mid(a+1, msg.size());
    switch (cmd) {
    case 3001:
        break;
    case 3003:
        break;
    case 3005:
        conf->initTypes(dat);
        break;
    case 6005:
        conf->initOther(dat);
        test->updateItems(dat);
        dcr->initData(dat);
        acw->initData(dat);
        inr->initData(dat);
        break;
    default:
        break;
    }
}

void MainPage::readButtons(QByteArray win)
{
    int WinCurrent = stack->currentIndex();
    if (win.isNull()) { //空代表返回
        stack->setCurrentIndex(previous_window.last());
        previous_window.removeLast();
        return;
    }
    for (int i=0; i < stack->count(); i++) {
        if (stack->widget(i)->objectName() == win) {
            emit transmitShow(win);
            stack->setCurrentIndex(i);
            break;
        }
    }
    previous_window.append(WinCurrent);
    if (previous_window.size() > 10) { // 最大嵌套10层
        previous_window.removeFirst();
    }
}

void MainPage::wait(int ms)
{
    QElapsedTimer t;
    t.start();
    while (t.elapsed() < ms)
        QCoreApplication::processEvents();
}

void MainPage::testThread()
{
    testInit();
//    testDCR();
}

void MainPage::testInit()
{
    test->initItems();
    QJsonObject obj;
    obj.insert("TxMessage","3008");
    emit transmitJson(obj);
}

void MainPage::testDCR()
{
    QJsonObject obj;
    obj.insert("TxMessage","6006 DCR");
    emit transmitJson(obj);
}

void MainPage::testINR()
{
    QJsonObject obj;
    obj.insert("TxMessage","6006 IR");
    emit transmitJson(obj);
}

void MainPage::testACW()
{
    QJsonObject obj;
    obj.insert("TxMessage","6006 ACW");
    emit transmitJson(obj);
}

