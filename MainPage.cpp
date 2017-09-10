/*******************************************************************************
 * Copyright [2017]   <青岛艾普智能仪器有限公司>
 * All rights reserved.
 *
 * version:     0.1
 * author:      zhaonanlin
 * brief:       主界面
*******************************************************************************/
#include "MainPage.h"

MainPage::MainPage(QWidget *parent) : QWidget(parent)
{
    initUI();
    initPLC();
    status = STATUS_FREE;
    station = 0x13;
    testing = false;
    isNG = false;
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

    winData = new PageSqlite(this);
    connect(winData, SIGNAL(buttonClicked(QByteArray)), this, SLOT(readButtons(QByteArray)));

    conf = new ConfPage(this);
    connect(conf, SIGNAL(sendAppCmd(QJsonObject)), this, SLOT(recvAppCmd(QJsonObject)));
    connect(conf, SIGNAL(buttonClicked(QByteArray)), this, SLOT(readButtons(QByteArray)));
    connect(conf, SIGNAL(sendNetMsg(QByteArray)), &udp, SLOT(recvAppMsg(QByteArray)));
    connect(this, SIGNAL(transmitShow(QString)), conf, SLOT(recvAppShow(QString)));
    connect(conf, SIGNAL(typeUpdate()), this, SLOT(readSettings()));

    test = new TestPage(this);
    connect(test, SIGNAL(buttonClicked(QByteArray)), this, SLOT(readButtons(QByteArray)));
    connect(test, SIGNAL(sendNetMsg(QByteArray)), &udp, SLOT(recvAppMsg(QByteArray)));
    connect(this, SIGNAL(transmitShow(QString)), test, SLOT(recvAppShow(QString)));
    connect(test, SIGNAL(buttonTest()), this, SLOT(readBtnStart()));
    connect(test, SIGNAL(buttonStop()), this, SLOT(readBtnStop()));
    connect(test, SIGNAL(buttonTest1()), this, SLOT(testDCR()));
    connect(test, SIGNAL(buttonTest2()), this, SLOT(testINR()));
    connect(test, SIGNAL(buttonTest3()), this, SLOT(testACW()));
    connect(test, SIGNAL(buttonTest4()), this, SLOT(testIND()));
    connect(test, SIGNAL(buttonTest5()), this, SLOT(testNLD()));
    connect(test, SIGNAL(buttonTest6()), this, SLOT(testEMF()));

    resistance = new ConfResistance(this);
    connect(resistance, SIGNAL(sendAppCmd(QJsonObject)), this, SLOT(recvAppCmd(QJsonObject)));
    connect(resistance, SIGNAL(buttonClicked(QByteArray)), this, SLOT(readButtons(QByteArray)));

    current_ac = new ConfCurrent_AC(this);
    connect(current_ac, SIGNAL(sendAppCmd(QJsonObject)), this, SLOT(recvAppCmd(QJsonObject)));
    connect(current_ac, SIGNAL(buttonClicked(QByteArray)), this, SLOT(readButtons(QByteArray)));

    insulation = new ConfInsulation(this);
    connect(insulation, SIGNAL(sendAppCmd(QJsonObject)), this, SLOT(recvAppCmd(QJsonObject)));
    connect(insulation, SIGNAL(buttonClicked(QByteArray)), this, SLOT(readButtons(QByteArray)));

    inductance = new ConfInductance(this);
    connect(inductance, SIGNAL(sendAppCmd(QJsonObject)), this, SLOT(recvAppCmd(QJsonObject)));
    connect(inductance, SIGNAL(buttonClicked(QByteArray)), this, SLOT(readButtons(QByteArray)));

    noloadtest = new ConfNoLoadTest(this);
    connect(noloadtest, SIGNAL(sendAppCmd(QJsonObject)), this, SLOT(recvAppCmd(QJsonObject)));
    connect(noloadtest, SIGNAL(buttonClicked(QByteArray)), this, SLOT(readButtons(QByteArray)));

    loadtesting = new ConfLoadTesting(this);
    connect(loadtesting, SIGNAL(sendAppCmd(QJsonObject)), this, SLOT(recvAppCmd(QJsonObject)));
    connect(loadtesting, SIGNAL(buttonClicked(QByteArray)), this, SLOT(readButtons(QByteArray)));

    halltesting = new ConfHall(this);
    connect(halltesting, SIGNAL(sendAppCmd(QJsonObject)), this, SLOT(recvAppCmd(QJsonObject)));
    connect(halltesting, SIGNAL(buttonClicked(QByteArray)), this, SLOT(readButtons(QByteArray)));

    backemftest = new ConfBackEMFTest(this);
    connect(backemftest, SIGNAL(sendAppCmd(QJsonObject)), this, SLOT(recvAppCmd(QJsonObject)));
    connect(backemftest, SIGNAL(buttonClicked(QByteArray)), this, SLOT(readButtons(QByteArray)));

    stack = new QStackedWidget(this);
    stack->addWidget(home);
    stack->addWidget(syst);
    stack->addWidget(winData);
    stack->addWidget(conf);
    stack->addWidget(test);
    stack->addWidget(resistance);
    stack->addWidget(current_ac);
    stack->addWidget(insulation);
    stack->addWidget(inductance);
    stack->addWidget(noloadtest);
    stack->addWidget(loadtesting);
    stack->addWidget(halltesting);
    stack->addWidget(backemftest);
    readButtons("HomePage");

    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->addWidget(stack);
}

void MainPage::initPLC()
{
    iobrd.initPort("COM3");
    servo.initPort("COM5");
    plc.initPort("COM6");
    connect(&iobrd, SIGNAL(sendStart(bool)), this, SLOT(readStart(bool)));
}

void MainPage::initUdp(QJsonObject obj)
{
    QString host = obj.value("host_addr").toString();
    quint16 port = obj.value("host_port").toString().toInt();
    QString user = obj.value("host_user").toString();
    QString pass = obj.value("host_pass").toString();
    if (user == "admin" && pass == currentPassword()) {
        qDebug() << "login ok";
        setCurrentUser("admin");
        emit sendNetMsg("6001");
    } else if(user == "guest") {
        setCurrentUser("guest");
    }else {
        qDebug() << "login error";
        emit sendNetMsg("6000");
        return;
    }

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
    case 6001:  // 自检信息
        readSelfCheck(dat);
        break;
    case 6005:  // 上传配置
        if (dat.contains("NG"))
            isNG = true;
        test->updateItems(dat);
        break;
    case 6007:  // 单项测试完成
        status = STATUS_FREE;
        break;
    case 6015:  // 空载启动完成
        readNoLoad();
        break;
    case 6019:
        power = dat.split(" ");
        break;
    case 6021:
        test->updateWave(dat);
        break;
    default:
        break;
    }
}

void MainPage::readButtons(QByteArray win)
{
    if (testing)
        return;
    int WinCurrent = stack->currentIndex();
    if (win.isNull()) { //空代表返回
        emit transmitShow(stack->widget(previous_window.last())->objectName());
        stack->setCurrentIndex(previous_window.last());
        previous_window.removeLast();
        testDebug();
        return;
    }
    for (int i=0; i < stack->count(); i++) {
        if (stack->widget(i)->objectName() == win) {
            stack->setCurrentIndex(i);
            emit transmitShow(win);
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

void MainPage::testInit()
{
    test->initItems(station);
    QJsonObject obj;
    obj.insert("TxMessage",QString("6020 %1").arg(station));
    emit transmitJson(obj);

    iobrd.sendPort(Y10);
    readCylinder(X01_ORIGIN | X03_ORIGIN);

    QStringList testItems = conf->testItems();
    testing = true;
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
        case STATUS_ACW:
            testACW();
            break;
        case STATUS_IND:
            testIND();
            break;
        case STATUS_HAL:
            testHAL();
            break;
        case STATUS_NLD:
            testNLD();
            break;
        case STATUS_LOD:
            testLOD();
            break;
        case STATUS_EMF:
            testEMF();
            break;
        default:
            break;
        }
        if (status == STATUS_OVER) {
            testStop();
            testTimeOut();
            break;
        }
        if (isNG) {
            isNG = false;
            if (testPause())
                break;
        }
    }
    QString xx;
    xx.append(CurrentSettings());
    xx.append("@");
    xx.append(currentUser());
    xx.append("@");
    if (test->updateResult()) {
        iobrd.sendPort(Y08 | Y09);  // 绿灯加蜂鸣器
        wait(currentAlarmTime("OK"));
        iobrd.sendPort(Y08);  // 绿灯
        xx.append("OK");
    } else {
        iobrd.sendPort(Y09 | Y11);  // 红灯加蜂鸣器
        wait(currentAlarmTime("NG"));
        iobrd.sendPort(Y11);  // 红灯
        xx.append("NG");
    }
    QJsonObject objs;
    objs.insert("title", xx);
    objs.insert("content", test->readResult());
    winData->saveSql(objs);

    status = STATUS_FREE;
    testing = false;
}

void MainPage::testDCR()
{
    QJsonObject obj;
    obj.insert("TxMessage","6006 DCR");
    emit transmitJson(obj);
    waitTimeOut(STATUS_DCR);
    wait(100);
}

void MainPage::testINR()
{
    QJsonObject obj;
    obj.insert("TxMessage","6006 IR");
    emit transmitJson(obj);
    waitTimeOut(STATUS_INR);
    wait(100);
}

void MainPage::testACW()
{
    QJsonObject obj;
    obj.insert("TxMessage","6006 ACW");
    emit transmitJson(obj);
    waitTimeOut(STATUS_ACW);
    wait(100);
}

void MainPage::testIND()
{
    QJsonObject obj;
    obj.insert("TxMessage","6006 IND");
    emit transmitJson(obj);
    waitTimeOut(STATUS_IND);
    wait(100);
}

void MainPage::testHAL()
{
    QJsonObject obj;
    obj.insert("TxMessage","6006 HALL");
    emit transmitJson(obj);
    waitTimeOut(STATUS_HAL);
    wait(100);
    readHall();
}

void MainPage::readHall()
{
    QString tmp;
    QStringList names;
    names << "UF1:" << "UF2:" << "UF3:"
          << "VF1:" << "VF2:" << "VF3:"
          << "WF1:" << "WF2:" << "WF3:"
          << "UH1:" << "UH2:" << "UH3:"
          << "VH1:" << "VH2:" << "VH3:"
          << "WH1:" << "WH2:" << "WH2:"
          << "UW1:" << "UW2:" << "UW3:"
          << "VU1:" << "VU2:" << "VU3:"
          << "WV1:" << "WV2:" << "WV3:"
          << "WV-U1:" << "WV-U2:" << "WV-U3:"
          << "UV-W1:" << "UV-W2:" << "UV-W3:"
          << "WU-V1:" << "WU-V2:" << "WU-V3:";
    QList<int> squn;
    squn << 0 << 1 << 2
         << 12 << 13 << 14
         << 24 << 25 << 26
         << 3 << 4 << 5
         << 15 << 16 << 17
         << 27 << 28 << 29
         << 6 << 7 << 8
         << 18 << 19 << 20
         << 30 << 31 << 32
         << 9 << 10 << 11
         << 21 << 22 << 23
         << 33 << 34 << 35;
    int r = 60*100000/(backemftest->readSpeed()*halltesting->readCount());
    QStringList full;
    QStringList half;
    QStringList with;
    QStringList hall;
    if (power.size() > 36) {
        for (int i=0; i < 9; i++) {
            full.append(QString::number(power.at(squn.at(i+0)).toDouble()*360/r));
            half.append(QString::number(power.at(squn.at(i+9)).toDouble()*360/r));
            with.append(QString::number(power.at(squn.at(i+18)).toDouble()*360/r));
            hall.append(QString::number(power.at(squn.at(i+27)).toDouble()*360/r));
        }
        for (int i=0; i < names.size(); i++) {
            double angle = power.at(squn.at(i)).toDouble()*360/r;
            QString t = QString("%1%2").arg(names.at(i)).arg(QString::number(angle, 'f', 1));
            tmp.append(t);
            if (i%3 == 2) {
                tmp.append("\n");
            } else {
                tmp.append("\t\t");
            }
            if (i%9 == 8)
                tmp.append("\n");
        }
    }
    QStringList item;
    item << "H:" << "L:" << "A:" << "Max:" << "Min:" << "Z:"
         << "ZC:" << "F:" << "D:" << "HZ:" << "C:";
    for (int i=50; i < power.size(); i++) {
        int t = (i-50)%20;
        if (t==3 || t > 11)
            continue;
        if (t > 3)
            t--;
        tmp.append(item.at(t));
        tmp.append(power.at(i));
        if (t == 10)
            tmp.append("\n");
        else
            tmp.append(",");
    }
    test->setTextHall(tmp);
    QString v;
    if (power.size() >= 120) {
        v.append(QString("H:%1V,").arg(power.at(50+60).toDouble()));
        v.append(QString("L:%1V,").arg(power.at(50+61).toDouble()));
        v.append(QString("F:%1Hz,").arg(power.at(50+68).toDouble()));
    }
    v.append(QString("%1°,").arg(QString::number(readWorst(360,full), 'f', 1)));
    v.append(QString("%1°,").arg(QString::number(readWorst(180,half), 'f', 1)));
    v.append(QString("%1°,").arg(QString::number(readWorst(120,with), 'f', 1)));
    v.append(QString("%1°").arg(QString::number(readWorst(32.5,hall), 'f', 1)));

    test->updateItem(v);
}

double MainPage::readWorst(double std, QStringList s)
{
    QList<double> w;
    for (int i=0; i < s.size(); i++) {
        w.append(abs(std-s.at(i).toDouble()));
    }
    double max = w.at(0);
    int t = 0;
    for (int i=0; i < w.size(); i++) {
        if (max < w.at(i)) {
            max = w.at(i);
            t = i;
        }
    }
    return s.at(t).toDouble();
}

void MainPage::testNLD()
{
    bool cylinder = false;

    iobrd.sendPort(Y02 | Y10);  // 气缸3压紧
    cylinder = readCylinder(X01_ORIGIN | X03_TARGET);
    if (!cylinder) {
        status = STATUS_OVER;
        return;
    }
    wait(100);

    if (!wt330.initPort("COM4")) {
        QMessageBox::warning(this, "", "电参失败", QMessageBox::Ok);
        status = STATUS_OVER;
        return;
    }

    QJsonObject obj;
    obj.insert("TxMessage","6006 NOLAOD");
    emit transmitJson(obj);
    waitTimeOut(STATUS_NLD);
    if (status != STATUS_OVER) {
        QStringList tmp = wt330.readMeter();
        if (tmp.size() >= 30) {
            double I1 = tmp.at(1).toDouble();
            double I2 = tmp.at(11).toDouble();
            double I3 = tmp.at(21).toDouble();
            double rpm = -1;
            QString tt;
            tt.append(QString("%1A,").arg(I1));
            tt.append(QString("%1A,").arg(I2));
            tt.append(QString("%1A,").arg(I3));
            if (power.size() > 1)
                rpm = power.at(0).toDouble()*1000;
            tt.append(QString("%1rpm").arg(rpm));
            test->updateItem(tt);
            QString qq;
            qq.append(QString("U相电压:%1V\n").arg(tmp.at(0).toDouble()));
            qq.append(QString("U相电流:%1A\n").arg(tmp.at(1).toDouble()));
            qq.append(QString("U相功率:%1W\n").arg(tmp.at(3).toDouble()));
            qq.append(QString("V相电压:%1V\n").arg(tmp.at(10).toDouble()));
            qq.append(QString("V相电流:%1A\n").arg(tmp.at(11).toDouble()));
            qq.append(QString("V相功率:%1W\n").arg(tmp.at(13).toDouble()));
            qq.append(QString("W相电压:%1V\n").arg(tmp.at(20).toDouble()));
            qq.append(QString("W相电流:%1A\n").arg(tmp.at(21).toDouble()));
            qq.append(QString("W相功率:%1W\n").arg(tmp.at(23).toDouble()));
            test->setTextLoad(qq);
            QString jj = "OK";
            QStringList ss = noloadtest->readLimit();
            if (I1 < ss.at(1).toDouble() || I1 > ss.at(2).toDouble())
                jj = "NG";
            if (I2 < ss.at(1).toDouble() || I2> ss.at(2).toDouble())
                jj = "NG";
            if (I3 < ss.at(1).toDouble() || I3 > ss.at(2).toDouble())
                jj = "NG";
            if (rpm < ss.at(5).toDouble() || rpm > ss.at(6).toDouble())
                jj = "NG";
            test->updateJudge(jj);
        } else {
            QMessageBox::warning(this, "", "电参失败", QMessageBox::Ok);
            test->updateItem("NULL");
            test->updateJudge("NG");
        }
    }
    QStringList s = conf->testItems();
    QList<int> tt;
    for (int i=0; i < s.size(); i++) {
        tt.append(QString(s.at(i)).toInt());
    }
    if (tt.indexOf(STATUS_EMF) - tt.indexOf(STATUS_NLD) != 1) {
        iobrd.sendPort(Y10);  // 气缸全部归位
        cylinder = readCylinder(X01_ORIGIN | X03_ORIGIN);
        if (!cylinder) {
            status = STATUS_OVER;
            return;
        }
        wait(100);
    }
    wt330.initPort(NULL);
}

void MainPage::testLOD()
{
    //    bool cylinder = false;
    //    iobrd.sendPort(Y10);  // 气缸全部归位
    //    cylinder = readCylinderL(X01_ORIGIN | X03_ORIGIN);
    //    if (!cylinder) {
    //        status = STATUS_OVER;
    //        return;
    //    }
    //    wait(100);

    //    iobrd.sendPort(Y02 | Y10);  // 气缸3压紧
    //    cylinder = readCylinderL(X01_ORIGIN | X03_TARGET);
    //    if (!cylinder) {
    //        status = STATUS_OVER;
    //        return;
    //    }
    //    wait(100);

    //    iobrd.sendPort(Y00 | Y02 | Y10);  // 气缸1上升
    //    cylinder = readCylinderL(X01_ORIGIN | X03_TARGET);
    //    if (!cylinder) {
    //        status = STATUS_OVER;
    //        return;
    //    }
    //    wait(100);

    //    QJsonObject obj;
    //    obj.insert("TxMessage","6006 LOAD");
    //    emit transmitJson(obj);
    //    wait(1500);

    //    plc->pre_speed();
    //    int load = loadtesting->readLoad()*2446;

    //    for (int i=0; i < 11; i++) {
    //        plc->add_speed(load/10*i);
    //        wait(100);
    //    }
    //    plc->readPlc();
    //    quint16 speed = plc->speed;
    //    quint16 torque = plc->torque;
    //    QString s = QString("转速:%1,转矩:%2\n").arg(speed).arg(torque);
    //    QMessageBox::warning(this, "伺服", s, QMessageBox::Ok);
    //    wait(1500);
    //    for (int i=0; i < 11; i++) {
    //        plc->add_speed(load/10*(10-i));
    //        wait(100);
    //    }

    //    wait(1500);
    //    plc->end_speed();
    //    wait(100);

}

void MainPage::testEMF()
{
    bool cylinder = false;
    iobrd.sendPort(Y02 | Y10);  // 气缸3压紧
    cylinder = readCylinder(X01_ORIGIN | X03_TARGET);
    if (!cylinder) {
        status = STATUS_OVER;
        return;
    }
    wait(100);

    iobrd.sendPort(Y00 | Y02 | Y10);  // 气缸1上升
    cylinder = readCylinder(X01_TARGET | X03_TARGET);
    if (!cylinder) {
        status = STATUS_OVER;
        return;
    }
    wait(100);

    plc.setMode(1);
    wait(50);
    plc.setTurn(backemftest->readTurn());
    wait(50);
    plc.setStart(1);
    wait(50);
    plc.setSpeed(100);
    wait(50);
    int speed = backemftest->readSpeed();
    for (int i=1; i < 11; i++) {
        plc.setSpeed(speed*i/10);
        wait(100);
    }
    QJsonObject obj;
    obj.insert("TxMessage","6006 BEMF");
    emit transmitJson(obj);
    waitTimeOut(STATUS_EMF);

    QStringList volt;
    QString v;
    if (power.size() > 92) {
        volt.append(power.at(52));
        volt.append(power.at(72));
        volt.append(power.at(92));
        v.append(QString::number(power.at(52).toDouble()/100));
        v.append(",");
        v.append(QString::number(power.at(72).toDouble()/100));
        v.append(",");
        v.append(QString::number(power.at(92).toDouble()/100));
    } else {
        v.append("NULL");
    }
    v.append(",");
    int b = readBalance(volt);
    if (b == -1)
        v.append("NULL");
    else
        v.append(QString("%1%").arg(b));
    if (status != STATUS_OVER) {
        test->updateItem(v);
    }

    for (int i=1; i < 11; i++) {
        plc.setSpeed(speed*(10-i)/10);
        wait(100);
    }
    plc.setStart(0);

    iobrd.sendPort(Y02 | Y10);  // 气缸1下降
    cylinder = readCylinder(X01_ORIGIN | X03_TARGET);
    if (!cylinder) {
        status = STATUS_OVER;
        return;
    }
    wait(100);

    QStringList s = conf->testItems();
    QList<int> tt;
    for (int i=0; i < s.size(); i++) {
        tt.append(QString(s.at(i)).toInt());
    }
    if (tt.indexOf(STATUS_NLD) - tt.indexOf(STATUS_EMF) != 1) {
        iobrd.sendPort(Y10);  // 气缸全部归位
        cylinder = readCylinder(X01_ORIGIN | X03_ORIGIN);
        if (!cylinder) {
            status = STATUS_OVER;
            return;
        }
        wait(100);
    }
}

int MainPage::readBalance(QStringList s)
{
    if (s.isEmpty())
        return -1;
    double max = s.at(0).toDouble();
    double min = s.at(0).toDouble();
    double avr = 0;
    for (int i=0; i < s.size(); i++) {
        avr += s.at(i).toDouble();
        max = qMax(max, s.at(i).toDouble());
        min = qMin(min, s.at(i).toDouble());
    }
    avr /= s.size();
    if (avr == 0)
        return -1;
    return ((max-min) * 100 / avr);
}

void MainPage::testStop()
{
    QJsonObject obj;
    obj.insert("TxMessage",QString("6022 %1").arg(station));
    emit transmitJson(obj);
    status = STATUS_OVER;
}

void MainPage::testStopAction()
{
    iobrd.sendPort(0x00);  // 气缸全部归位
    readCylinder(X01_ORIGIN | X03_ORIGIN);
}

void MainPage::testTimeOut()
{
    QJsonObject obj;
    obj.insert("TxMessage",QString("6026"));
    emit transmitJson(obj);
    status = STATUS_OVER;
}

bool MainPage::readCylinder(quint16 s)
{
    quint16 timeOut = 0x0000;
    while (1) {
        if ((iobrd.readPort() & 0xFF00) == s)
            return true;
        wait(10);
        timeOut++;
        if (timeOut > 500) {
            QMessageBox::warning(this, "气缸", "气缸到位超时", QMessageBox::Ok);
            return false;
        }
        if (status == STATUS_OVER)
            return false;
    }
}

bool MainPage::waitTimeOut(quint16 s)
{
    status = s;
    timeOut = 0;
    while (1) {
        wait(10);
        timeOut++;
        if (timeOut > 3000) {
            QMessageBox::warning(this, "超时", QString("测试超时:%1").arg(s), QMessageBox::Ok);
            testTimeOut();
            return false;
        }
        if (status != s)
            return true;
    }
}

bool MainPage::testPause()
{
    if (currentPauseMode() == 0) {
        return false;
    }
    QString text = tr("此项目不合格,是否继续");
    PopupBox *box = new PopupBox(this, "", text, QMessageBox::Retry, QMessageBox::Ok);
    int ret = box->exec();
    if (ret == QMessageBox::Retry)
        return true;
    else
        return false;
}

void MainPage::readSettings()
{
    QString t = QString("./config/%1.ini").arg(CurrentSettings());
    QSettings *ini = new QSettings(t, QSettings::IniFormat);
    ini->setIniCodec("GB18030");

    QStringList names_cnf;
    QJsonObject obj_cnf;
    QJsonObject obj_array;
    names_cnf << "color" << "type" << "test";
    ini->beginGroup("Conf");
    for (int i=0; i < names_cnf.size(); i++) {
        QString def = "0,0,0,0,0,0,0,0";
        if (names_cnf.at(i) == "type")
            def = "M1S1";
        if (names_cnf.at(i) == "test")
            def = "0";
        obj_cnf.insert(names_cnf.at(i), ini->value(names_cnf.at(i), def).toString());
    }
    ini->endGroup();
    obj_array.insert("Conf", obj_cnf);
    conf->initSettings(obj_cnf);

    sendXmlCmd(obj_array);

    QStringList names_sys;
    QJsonObject obj_sys;
    names_sys << "Test_Item";
    ini->beginGroup("Sys");
    for (int i=0; i < names_sys.size(); i++) {
        QString def = "1";
        obj_sys.insert(names_sys.at(i), ini->value(names_sys.at(i), def).toString());
    }
    ini->endGroup();
    obj_array.insert("Sys", obj_sys);
    conf->initSysItems(obj_sys);
    QJsonObject tmp;
    tmp.insert("Sys", obj_sys);
    sendXmlCmd(tmp);

    QStringList names_dcr;
    QJsonObject obj_dcr;
    names_dcr << "test" << "port1" << "port2" << "wire" << "unit" << "min" << "max" << "std"
              << "wire_comp1" << "wire_comp2" << "std_temp" << "temp_comp" << "noun" << "time";
    ini->beginGroup("DCR");
    for (int i=0; i < names_dcr.size(); i++) {
        QString def = "0,0,0,0,0,0,0,0";
        if (names_dcr.at(i) == "std_temp")
            def = "20";
        if (names_dcr.at(i) == "temp_comp")
            def = "0";
        if (names_dcr.at(i) == "noun")
            def = "0";
        if (names_dcr.at(i) == "time")
            def = "0";
        obj_dcr.insert(names_dcr.at(i), ini->value(names_dcr.at(i), def).toString());
    }
    ini->endGroup();
    obj_array.insert("DCR", obj_dcr);
    resistance->initSettings(obj_dcr);

    QStringList names_inr;
    QJsonObject obj_inr;
    names_inr << "test" << "port1" << "port2" << "volt"
              << "min" << "max" << "time";;
    ini->beginGroup("IR");
    for (int i=0; i < names_inr.size(); i++) {
        QString def = "0,0,0,0,0";
        obj_inr.insert(names_inr.at(i), ini->value(names_inr.at(i), def).toString());
    }
    ini->endGroup();
    obj_array.insert("IR", obj_inr);
    insulation->initSettings(obj_inr);

    QStringList names_acw;
    QJsonObject obj_acw;
    names_acw << "test" << "port1" << "port2" << "volt"
              << "min" << "max" << "time" << "freq" << "arc";
    ini->beginGroup("ACW");
    for (int i=0; i < names_acw.size(); i++) {
        QString def = "0,0,0,0,0";
        obj_acw.insert(names_acw.at(i), ini->value(names_acw.at(i), def).toString());
    }
    ini->endGroup();
    obj_array.insert("ACW", obj_acw);
    current_ac->initSettings(obj_acw);

    QStringList names_ind;
    QJsonObject obj_ind;
    names_ind << "test" << "port1" << "port2" << "unit"
              << "min" << "max" << "qmin" << "qmax" << "std"
              << "wire_comp1" << "wire_comp2" << "mode" << "noun";
    ini->beginGroup("IND");
    for (int i=0; i < names_ind.size(); i++) {
        QString def = "0,0,0,0,0,0,0,0";
        if (names_ind.at(i) == "mode")
            def = "0,0,0,0";
        if (names_ind.at(i) == "noun")
            def = "0";
        obj_ind.insert(names_ind.at(i), ini->value(names_ind.at(i), def).toString());
    }
    ini->endGroup();
    obj_array.insert("IND", obj_ind);
    inductance->initSettings(obj_ind);

    QStringList names_hal;
    QJsonObject obj_hal;
    names_hal << "volt_low_min" << "volt_low_max" << "volt_up_min" << "volt_up_max"
              << "freq_min" << "freq_max" << "duty_min" << "duty_max"
              << "skewing_min_f" << "skewing_max_f" << "skewing_min_h" << "skewing_max_h"
              << "skewing_min_s" << "skewing_max_s" << "count" << "vcc_volt" << "time" << "mode";
    ini->beginGroup("HALL");
    for (int i=0; i < names_hal.size(); i++) {
        QString def = "0";
        obj_hal.insert(names_hal.at(i), ini->value(names_hal.at(i), def).toString());
    }
    ini->endGroup();
    obj_array.insert("HALL", obj_hal);
    halltesting->initSettings(obj_hal);

    QStringList names_lod;
    QJsonObject obj_lod;
    names_lod << "volt" << "curr_min" << "curr_max"
              << "pwr_min" << "pwr_max"
              << "speed_min" << "speed_max" << "torque"
              << "vcc_volt" << "vsp_volt" << "time" << "driver" << "power"<< "sequence";
    ini->beginGroup("LOAD");
    for (int i=0; i < names_lod.size(); i++) {
        QString def = "0";
        if (names_lod.at(i) == "sequence")
            def = "1,1,1,1,1,1,1,1,1,1,1,1,1,1";
        obj_lod.insert(names_lod.at(i), ini->value(names_lod.at(i), def).toString());
    }
    ini->endGroup();
    obj_array.insert("LOAD", obj_lod);
    loadtesting->initSettings(obj_lod);

    QStringList names_nld;
    QJsonObject obj_nld;
    names_nld << "volt" << "curr_min" << "curr_max"
              << "pwr_min" << "pwr_max"
              << "speed_min" << "speed_max"
              << "vcc_volt" << "vsp_volt" << "time" << "driver" << "power" << "sequence" << "turn";
    ini->beginGroup("NOLOAD");
    for (int i=0; i < names_nld.size(); i++) {
        QString def = "0";
        if (names_nld.at(i) == "sequence")
            def = "1,1,1,1,1,1,1,1,1,1";
        if (names_nld.at(i) == "turn")
            def = "0,0";
        obj_nld.insert(names_nld.at(i), ini->value(names_nld.at(i), def).toString());
    }
    ini->endGroup();
    obj_array.insert("NOLOAD", obj_nld);
    noloadtest->initSettings(obj_nld);

    QStringList names_bmf;
    QJsonObject obj_bmf;
    names_bmf << "volt_min" << "volt_max" << "bemf_min" << "bemf_max"
              << "speed" << "turn" << "skewing_min" << "skewing_max"
              << "volt_vcc" << "time" << "noun";
    ini->beginGroup("BEMF");
    for (int i=0; i < names_bmf.size(); i++) {
        QString def = "0";
        obj_bmf.insert(names_bmf.at(i), ini->value(names_bmf.at(i), def).toString());
    }
    ini->endGroup();
    obj_array.insert("BEMF", obj_bmf);
    backemftest->initSettings(obj_bmf);

    //    conf_array.remove("Conf");
    //    sendXmlCmd(conf_array);
    //    conf_array.insert("Conf", obj_cnf);
    conf_array = obj_array;
}

void MainPage::saveSettings()
{
    QString t = QString("./config/%1.ini").arg(CurrentSettings());
    QSettings *ini = new QSettings(t, QSettings::IniFormat);
    ini->setIniCodec("GB18030");

    QStringList keys = conf_array.keys();
    for (int i=0; i < keys.size(); i++) {
        QJsonObject obj = conf_array.value(keys.at(i)).toObject();
        QStringList temp = obj.keys();
        ini->beginGroup(keys.at(i));
        for (int i=0; i < temp.size(); i++) {
            ini->setValue(temp.at(i), obj.value(temp.at(i)).toString());
        }
        ini->endGroup();
    }
}

QString MainPage::CurrentSettings()
{
    QSettings *ini = new QSettings("./nandflash/global.ini", QSettings::IniFormat);
    QString n = ini->value("/GLOBAL/FileInUse", "Base_File").toString();
    return n.remove(".ini");
}

QString MainPage::currentPassword()
{
    QSettings *ini = new QSettings("./nandflash/global.ini", QSettings::IniFormat);
    QString n = ini->value("/GLOBAL/Password", "6").toString();
    return n;
}

QString MainPage::currentUser()
{
    QSettings *ini = new QSettings("./nandflash/global.ini", QSettings::IniFormat);
    QString temp = ini->value("/GLOBAL/User", "0").toString();;
    if (temp == "0")
        return "guest";
    else
        return "admin";
}

int MainPage::currentPauseMode()
{
    QString n = QString("./config/%1.ini").arg(CurrentSettings());
    QSettings *ini = new QSettings(n, QSettings::IniFormat);
    return ini->value("/Conf/test", "1").toInt();
}

void MainPage::setCurrentUser(QString s)
{
    QSettings *ini = new QSettings("./nandflash/global.ini", QSettings::IniFormat);
    if (s == "admin")
        ini->setValue("/GLOBAL/User", 1);
    else
        ini->setValue("/GLOBAL/User", 0);
}

void MainPage::recvAppCmd(QJsonObject obj)
{
    QStringList keys = obj.keys();
    for (int i=0; i < keys.size(); i++)
        conf_array.insert(keys.at(i), obj.value(keys.at(i)).toObject());

    saveSettings();
    sendXmlCmd(obj);
}

void MainPage::sendXmlCmd(QJsonObject obj)
{
    QStringList keys = obj.keys();
    for (int i=0; i < keys.size(); i++) {
        QDomDocument doc;
        QDomElement root;
        root = doc.createElement(keys.at(i));
        doc.appendChild(root);

        QJsonObject temp = obj.value(keys.at(i)).toObject();
        QStringList key = temp.keys();
        for (int t=0; t < key.size(); t++) {
            QDomElement element = doc.createElement(key.at(t));
            QDomText text = doc.createTextNode(temp.value(key.at(t)).toString());
            element.appendChild(text);
            root.appendChild(element);
        }
        QJsonObject xx;

        QString s = "6004 ";
        s.append(keys.at(i));
        xx.insert("TxMessage", s);
        emit transmitJson(xx);

        QString msg = "6002 ";
        msg.append(doc.toByteArray());
        xx.insert("TxMessage", msg);
        emit transmitJson(xx);
    }
}

void MainPage::readNoLoad()
{
    //    QStringList m = wt330.readMeter();
    //    if (m.size() < 30) {
    //        QMessageBox::warning(this, "", "电参Error", QMessageBox::Ok);
    //        return;
    //    }
    //    meter = m;
    //    QMessageBox::warning(this, "", m.join(","), QMessageBox::Ok);
}

void MainPage::readBtnStart()
{
    readStart(true);
}

void MainPage::readBtnStop()
{
    readStart(false);
}

void MainPage::readStart(bool s)
{
    if (s && !testing)
        QTimer::singleShot(50, this, SLOT(testInit()));
    if (!s && testing) {
        if (status == STATUS_FREE)
            testStopAction();
        testStop();
        testTimeOut();
    }
}

void MainPage::readSelfCheck(QString s)
{
    emit sendNetMsg("6001");
    if (!s.isEmpty()) {
        QString text2 = tr("板卡错误%1").arg(s);
        PopupBox *box2 = new PopupBox(this, "", text2, QMessageBox::Ok);
        wait(100);
        box2->exec();
    }
}

void MainPage::testDebug()
{
}

void MainPage::showWarnning()
{
    QString text2 = tr("不放置带连轴器电机时,请禁止启动测试");
    PopupBox *box2 = new PopupBox(this, "", text2, QMessageBox::Ok);
    wait(100);
    box2->exec();
}

int MainPage::currentAlarmTime(QString msg)
{
    QSettings *ini = new QSettings(INI_PATH, QSettings::IniFormat);
    if (msg == "NG")
        return ini->value("/GLOBAL/TimeNG", "0.2").toDouble()*1000;
    else
        return ini->value("/GLOBAL/TimeOK", "0.1").toDouble()*1000;
}
