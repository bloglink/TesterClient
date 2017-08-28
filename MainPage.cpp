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
    initPLC();
    status = STATUS_FRE;
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
    connect(test, SIGNAL(buttonTest3()), this, SLOT(testACW()));
    connect(test, SIGNAL(buttonTest4()), this, SLOT(testIND()));
    connect(test, SIGNAL(buttonTest5()), this, SLOT(testNLD()));
    connect(test, SIGNAL(buttonTest6()), this, SLOT(testLOD()));

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

    hall = new ConfHall(this);
    connect(hall, SIGNAL(buttonClicked(QByteArray)), this, SLOT(readButtons(QByteArray)));
    connect(hall, SIGNAL(sendNetMsg(QByteArray)), &udp, SLOT(recvAppMsg(QByteArray)));
    connect(this, SIGNAL(transmitShow(QString)), hall, SLOT(recvAppShow(QString)));

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
    stack->addWidget(hall);
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
    case 6000:
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
        hall->initData(dat);
        backforce->initData(dat);
        break;
    case 6017:
        conf->initTypes(dat);
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
}

void MainPage::testInit()
{
    test->initItems();
    QJsonObject obj;
    obj.insert("TxMessage","6020 L");
    emit transmitJson(obj);

    QStringList testItems = conf->testItems();
    for (int i=0; i < testItems.size(); i++) {
        int cmd = testItems.at(i).toInt();
        status = cmd;
        switch (cmd) {
        case STATUS_DCR:
            testDCR();
            break;
        case STATUS_INR:
            testINR();
            break;
        case STATUS_IND:
            testIND();
        case STATUS_NLD:
            testNLD();
            break;
        case STATUS_LOD:
            testLOD();
            break;
        default:
            break;
        }
    }
    status = STATUS_FRE;
}

void MainPage::testDCR()
{
    qDebug() << conf->testItems();
    QJsonObject obj;
    obj.insert("TxMessage","6006 DCR L");
    emit transmitJson(obj);
    quint32 timeOut = 0;
    status = STATUS_DCR;
    while (1) {
        wait(10);
        timeOut++;
        if (timeOut > 1000) {
            QMessageBox::warning(this, "电阻", "电阻测试超时", QMessageBox::Ok);
            break;
        }
        if (status != STATUS_DCR)
            break;
    }
}

void MainPage::testINR()
{
    QJsonObject obj;
    obj.insert("TxMessage","6006 IR L");
    emit transmitJson(obj);
}

void MainPage::testACW()
{
    quint16 timeOut = 0x0000;

    plc->send_IO_L(Y03 | Y10);
    timeOut = 0;
    while (1) {
        if (plc->hexL & X04_TARGET) {
            QMessageBox::warning(this, "气缸4", "气缸4到位", QMessageBox::Ok);
            break;
        }
        wait(100);
        timeOut++;
        if (timeOut > 50) {
            QMessageBox::warning(this, "气缸4", "气缸4到位超时", QMessageBox::Ok);
            break;
        }
    }
    wait(3000);
    plc->send_IO_L(Y10);
    timeOut = 0;
    while (1) {
        if (plc->hexL & X04_ORIGIN) {
            QMessageBox::warning(this, "气缸4", "气缸4归位", QMessageBox::Ok);
            break;
        }
        wait(100);
        timeOut++;
        if (timeOut > 50) {
            QMessageBox::warning(this, "气缸4", "气缸4归位超时", QMessageBox::Ok);
            break;
        }
    }
    plc->send_IO_L(0);

    QJsonObject obj;
    obj.insert("TxMessage","6006 ACW L");
    emit transmitJson(obj);
}

void MainPage::testIND()
{
    QJsonObject obj;
    obj.insert("TxMessage","6006 IND L");
    emit transmitJson(obj);
}

void MainPage::testNLD()
{
    quint16 timeOut = 0x0000;
    plc->send_IO_L(Y00 | Y10);
    timeOut = 0;
    while (1) {
        if (plc->hexL & X01_TARGET) {
            QMessageBox::warning(this, "气缸1", "气缸1到位", QMessageBox::Ok);
            break;
        }
        wait(100);
        timeOut++;
        if (timeOut > 50) {
            QMessageBox::warning(this, "气缸1", "气缸1到位超时", QMessageBox::Ok);
            break;
        }
    }
    plc->send_IO_L(Y00 | Y01 | Y10);
    while (1) {
        if (plc->hexL & X02_TARGET) {
            QMessageBox::warning(this, "气缸2", "气缸2到位", QMessageBox::Ok);
            break;
        }
        wait(100);
        timeOut++;
        if (timeOut > 50) {
            QMessageBox::warning(this, "气缸2", "气缸2到位超时", QMessageBox::Ok);
            break;
        }
    }
    plc->send_IO_L(Y00 | Y01 | Y02 | Y10);
    while (1) {
        if (plc->hexL & X03_TARGET) {
            QMessageBox::warning(this, "气缸3", "气缸3到位", QMessageBox::Ok);
            break;
        }
        wait(100);
        timeOut++;
        if (timeOut > 50) {
            QMessageBox::warning(this, "气缸3", "气缸3到位超时", QMessageBox::Ok);
            break;
        }
    }
    wait(3000);
    plc->send_IO_L(Y00 | Y01 | Y10);
    timeOut = 0;
    while (1) {
        if (plc->hexL & X03_ORIGIN) {
            QMessageBox::warning(this, "气缸3", "气缸3归位", QMessageBox::Ok);
            break;
        }
        wait(100);
        timeOut++;
        if (timeOut > 50) {
            QMessageBox::warning(this, "气缸3", "气缸3归位超时", QMessageBox::Ok);
            break;
        }
    }

    QJsonObject obj;
    obj.insert("TxMessage","6006 PWR L");
    emit transmitJson(obj);
}

void MainPage::testLOD()
{
    quint16 timeOut = 0x0000;
    plc->send_IO_L(Y00 | Y01 | Y02 | Y10);
    while (1) {
        if (plc->hexL & X03_TARGET) {
            QMessageBox::warning(this, "气缸3", "气缸3到位", QMessageBox::Ok);
            break;
        }
        wait(100);
        timeOut++;
        if (timeOut > 50) {
            QMessageBox::warning(this, "气缸3", "气缸3到位超时", QMessageBox::Ok);
            break;
        }
    }
    plc->pre_speed();
    QMessageBox::warning(this, "伺服", "伺服速度启动", QMessageBox::Ok);
    for (int i=0; i < 10; i++) {
        plc->add_speed(6000*i);
        wait(100);
    }
    wait(1500);
    for (int i=0; i < 10; i++) {
        plc->add_speed(60000-6000*i);
        wait(100);
    }

    wait(1500);
    plc->end_speed();
    wait(50);
    plc->send_IO_L(Y00 | Y01 | Y10);
    timeOut = 0;
    while (1) {
        if (plc->hexL & X03_ORIGIN) {
            QMessageBox::warning(this, "气缸3", "气缸3归位", QMessageBox::Ok);
            break;
        }
        wait(100);
        timeOut++;
        if (timeOut > 50) {
            QMessageBox::warning(this, "气缸3", "气缸3归位超时", QMessageBox::Ok);
            break;
        }
    }

    QJsonObject obj;
    obj.insert("TxMessage","6006 LOD L");
    emit transmitJson(obj);
}

