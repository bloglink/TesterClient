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

    PageSqlite *wdat = new PageSqlite(this);
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

    resistance = new ConfResistance(this);
    connect(resistance, SIGNAL(buttonClicked(QByteArray)), this, SLOT(readButtons(QByteArray)));
    connect(resistance, SIGNAL(sendNetMsg(QByteArray)), &udp, SLOT(recvAppMsg(QByteArray)));
    connect(this, SIGNAL(transmitShow(QString)), resistance, SLOT(recvAppShow(QString)));

    current_ac = new ConfCurrent_AC(this);
    connect(current_ac, SIGNAL(buttonClicked(QByteArray)), this, SLOT(readButtons(QByteArray)));
    connect(current_ac, SIGNAL(sendNetMsg(QByteArray)), &udp, SLOT(recvAppMsg(QByteArray)));
    connect(this, SIGNAL(transmitShow(QString)), current_ac, SLOT(recvAppShow(QString)));

    insulation = new ConfInsulation(this);
    connect(insulation, SIGNAL(buttonClicked(QByteArray)), this, SLOT(readButtons(QByteArray)));
    connect(insulation, SIGNAL(sendNetMsg(QByteArray)), &udp, SLOT(recvAppMsg(QByteArray)));
    connect(this, SIGNAL(transmitShow(QString)), insulation, SLOT(recvAppShow(QString)));

    inductance = new ConfInductance(this);
    connect(inductance, SIGNAL(buttonClicked(QByteArray)), this, SLOT(readButtons(QByteArray)));
    connect(inductance, SIGNAL(sendNetMsg(QByteArray)), &udp, SLOT(recvAppMsg(QByteArray)));
    connect(this, SIGNAL(transmitShow(QString)), inductance, SLOT(recvAppShow(QString)));

    noloadtest = new ConfNoLoadTest(this);
    connect(noloadtest, SIGNAL(buttonClicked(QByteArray)), this, SLOT(readButtons(QByteArray)));
    connect(noloadtest, SIGNAL(sendNetMsg(QByteArray)), &udp, SLOT(recvAppMsg(QByteArray)));
    connect(this, SIGNAL(transmitShow(QString)), noloadtest, SLOT(recvAppShow(QString)));

    loadtesting = new ConfLoadTesting(this);
    connect(loadtesting, SIGNAL(buttonClicked(QByteArray)), this, SLOT(readButtons(QByteArray)));
    connect(loadtesting, SIGNAL(sendNetMsg(QByteArray)), &udp, SLOT(recvAppMsg(QByteArray)));
    connect(this, SIGNAL(transmitShow(QString)), loadtesting, SLOT(recvAppShow(QString)));

    ConfigFG *fg = new ConfigFG(this);
    connect(fg, SIGNAL(buttonClicked(QByteArray)), this, SLOT(readButtons(QByteArray)));
    connect(fg, SIGNAL(sendNetMsg(QByteArray)), &udp, SLOT(recvAppMsg(QByteArray)));

    backforce = new ConfBackForce(this);
    connect(backforce, SIGNAL(buttonClicked(QByteArray)), this, SLOT(readButtons(QByteArray)));
    connect(backforce, SIGNAL(sendNetMsg(QByteArray)), &udp, SLOT(recvAppMsg(QByteArray)));
    connect(this, SIGNAL(transmitShow(QString)), backforce, SLOT(recvAppShow(QString)));

    stack = new QStackedWidget(this);
    stack->addWidget(home);
    stack->addWidget(syst);
    stack->addWidget(wdat);
    stack->addWidget(conf);
    stack->addWidget(test);
    stack->addWidget(resistance);
    stack->addWidget(current_ac);
    stack->addWidget(insulation);
    stack->addWidget(inductance);
    stack->addWidget(noloadtest);
    stack->addWidget(loadtesting);
    stack->addWidget(fg);
    stack->addWidget(backforce);
    readButtons("HomePage");

    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->addWidget(stack);
    layout->setMargin(0);
}

void MainPage::initPLC()
{
    plc = new CtrlDevice_232(this);
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
        resistance->initData(dat);
        current_ac->initData(dat);
        insulation->initData(dat);
        inductance->initData(dat);
        noloadtest->initData(dat);
        loadtesting->initData(dat);
        break;
    default:
        break;
    }
}

void MainPage::readButtons(QByteArray win)
{
    int WinCurrent = stack->currentIndex();
    if (win.isNull()) { //空代表返回
        emit transmitShow(stack->widget(previous_window.last())->objectName());
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

