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
    initCom();
    status = STATUS_FREE;
    station = 0x13;
    testing = false;
    isNG = false;
    isServo = false;
    loadStopEnable = false;
    isLoadStop = false;
    stopping = false;
    speed = 0;

    load_timer = new QTimer(this);
    connect(load_timer, SIGNAL(timeout()), this, SLOT(waitSendStop()));
}

MainPage::~MainPage()
{
}

bool MainPage::check()
{
#ifdef WIN32
    QProcess* process = new QProcess;
    process->start("tasklist" ,QStringList()<<"/FI"<<"imagename eq TesterClient.exe");
    process->waitForFinished();
    QString outputStr = QString::fromLocal8Bit(process->readAllStandardOutput());
    if (outputStr.count("TesterClient.exe") > 1) {
        QMessageBox::warning(this, "", "程序已开启", QMessageBox::Ok);
        return false;
    }
#endif
    return true;
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

    connect(this, SIGNAL(transmitShow(QString)), this, SLOT(recvAppShow(QString)));

    HomePage *home = new HomePage(this);
    connect(home, SIGNAL(buttonClicked(QByteArray)), this, SLOT(readButtons(QByteArray)));

    WinSyst *syst = new WinSyst(this);
    connect(syst, SIGNAL(buttonClicked(QByteArray)), this, SLOT(readButtons(QByteArray)));

    winBack = new BackPage(this);
    connect(winBack, SIGNAL(buttonClicked(QByteArray)), this, SLOT(readButtons(QByteArray)));

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
    connect(test, SIGNAL(buttonTest(bool, quint16)), this, SLOT(readStart(bool, quint16)));
    connect(test, SIGNAL(buttonTest1()), this, SLOT(testDCR()));
    connect(test, SIGNAL(buttonTest2()), this, SLOT(testINR()));
    connect(test, SIGNAL(buttonTest3()), this, SLOT(testACW()));
    connect(test, SIGNAL(buttonTest4()), this, SLOT(testIND()));
    connect(test, SIGNAL(buttonTest5()), this, SLOT(testNLD()));
    connect(test, SIGNAL(buttonTest6()), this, SLOT(testLOD()));

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
    stack->addWidget(winBack);
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

    warnnig = new PopupBox(this, "", "配置中，请稍后", QMessageBox::Ok);
    warnnig->setWindowFlags(Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint | Qt::Popup);
    warnnig->setStyleSheet("QDialog{border:2px solid cyan;}");
    warnnig->hide();

    box2 = new PopupBox(this, "", "", QMessageBox::Ok);
    box2->setWindowFlags(Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint | Qt::Popup);
    box2->setStyleSheet("QDialog{border:2px solid cyan;}");
    box2->hide();
}

void MainPage::initCom()
{
    iobrdL.initPort("COM3");
    servoL.initPort("COM4");
    mbdktL.initPort("COM5");
    iobrdR.initPort("COM6");
    servoR.initPort("COM7");
    mbdktR.initPort("COM8");

    connect(&iobrdL, SIGNAL(sendStart(bool)), this, SLOT(readStartL(bool)));
    connect(&iobrdL, SIGNAL(iobrdReset(bool)), this, SLOT(iobrdReset()));
    connect(&iobrdR, SIGNAL(sendStart(bool)), this, SLOT(readStartR(bool)));
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
        if (status != STATUS_OVER)
            status = STATUS_FREE;
        break;
    case 6015:  // 空载启动完成
        QTimer::singleShot(10, this, SLOT(readNoLoadStart()));
        break;
    case 6019:
        power = dat.split(" ");
        break;
    case 6021:
        test->updateWave(dat);
        break;
    case 6032:
        test->updateTemp(dat);
        break;
    case 9032:
        readStartL(false);
        readStartR(false);
        QMessageBox::warning(this, "警告", "失去对设备的连接", QMessageBox::Ok);
        break;
    case 6033:
        loadStopEnable = false;
        QTimer::singleShot(10, this, SLOT(readNoLoadStop()));
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
    int ret;
    testing = true;
    isNG = false;
    isServo = false;
    loadStopEnable = false;
    isLoadStop = false;
    stopping = false;
    load_timer->stop();
    test->initItems(station);
    if (!cylinderAction(LED_Y, station)) {
        cylinderAction(0x00, station);
        test->updateResult(STATUS_OVER);
        testing = false;
        return;
    }

    udp.send_command(QString("6020 %1").arg(station));

    QStringList testItems = conf->testItems();
    testing = true;
    status = STATUS_FREE;
    for (int i=0; i < testItems.size(); i++) {
        int cmd = testItems.at(i).toInt();
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
            ret = cylinder_start();
            if (ret == 0)
                testLOD();
            cylinder_stop(ret);
            break;
        default:
            break;
        }
        if (status == STATUS_OVER) {
            testStop();
            break;
        }
        if (isNG) {
            isNG = false;
            if (testPause()) {
                testStop();
                break;
            }
        }
    }
    QString xx;
    xx.append(CurrentSettings());
    xx.append("@");
    xx.append(test->readNumb());
    xx.append("@");
    xx.append(currentUser());
    xx.append("@");
    if (test->updateResult(status)) {
        cylinderAction(LED_G | LED_B, station);
        wait(currentAlarmTime("OK"));
        cylinderAction(LED_G, station);
        xx.append("OK");
    } else {
        cylinderAction(LED_R | LED_B, station);
        wait(currentAlarmTime("NG"));
        cylinderAction(LED_R, station);
        xx.append("NG");
    }
    QJsonObject objs;
    objs.insert("title", xx);
    objs.insert("content", test->readResult());
    winData->saveSql(objs);

    status = STATUS_FREE;
    testing = false;
    qDebug() << "test over";
}

void MainPage::testDCR()
{
    sendUdpCommand("6006 DCR");
    waitTimeOut(STATUS_DCR);
}

void MainPage::testINR()
{
    qDebug() << QTime::currentTime().toString("hh:mm:ss") << "ir start";
    if (!cylinderAction(Y03 | Y10, station))
        status = STATUS_OVER;
    if (status == STATUS_OVER)
        return;
    qDebug() << QTime::currentTime().toString("hh:mm:ss") << "ir cylinder ok";

    sendUdpCommand("6006 IR");
    waitTimeOut(STATUS_INR);

    QStringList s = conf->testItems();
    QList<int> tt;
    for (int i=0; i < s.size(); i++) {
        tt.append(QString(s.at(i)).toInt());
    }
    if (tt.indexOf(STATUS_ACW) - tt.indexOf(STATUS_INR) != 1) {
        if (!cylinderAction(Y10, station))
            status = STATUS_OVER;
    }
    qDebug() << QTime::currentTime().toString("hh:mm:ss") << "ir over";
}

void MainPage::testACW()
{
    qDebug() << QTime::currentTime().toString("hh:mm:ss") << "acw start";
    if (!cylinderAction(Y03 | Y10, station))
        status = STATUS_OVER;
    if (status == STATUS_OVER)
        return;
    qDebug() << QTime::currentTime().toString("hh:mm:ss") << "acw cylinder ok";

    sendUdpCommand("6006 ACW");
    waitTimeOut(STATUS_ACW);

    QStringList s = conf->testItems();
    QList<int> tt;
    for (int i=0; i < s.size(); i++) {
        tt.append(QString(s.at(i)).toInt());
    }
    if (tt.indexOf(STATUS_INR) - tt.indexOf(STATUS_ACW) != 1) {
        if (!cylinderAction(Y10, station)) {
            status = STATUS_OVER;
            return;
        }
    }
    qDebug() << QTime::currentTime().toString("hh:mm:ss") << "acw over";
}

void MainPage::testIND()
{
    sendUdpCommand("6006 IND");
    waitTimeOut(STATUS_IND);
}

void MainPage::testHAL()
{
    sendUdpCommand("6006 HALL");
    waitTimeOut(STATUS_HAL);
    readHall();
}

void MainPage::readHall()
{
    QString vv;
    QString jj = "OK";
    QString text;
    if (power.size() >= 220) {
        QStringList hh;
        hh << power.at(50+60);
        double hMax = math.readMax(hh)*15.3/4095*readHighVolt().toDouble()/100;
        double hMin = math.readMin(hh)*15.3/4095*readHighVolt().toDouble()/100;
        QStringList ll;
        ll << power.at(50+61);
        double lMax = math.readMax(ll)*15.3/4095;
        double lMin = math.readMin(ll)*15.3/4095;
        QStringList dd;
        dd << QString::number(power.at(50+151).toDouble()/1000);
        double dMax = math.readMax(dd);
        double dMin = math.readMin(dd);
        QStringList ff;
        ff << QString::number(power.at(50+150).toDouble()/1000);
        ff << QString::number(power.at(50+154).toDouble()/1000);
        ff << QString::number(power.at(50+158).toDouble()/1000);
        double fMax = ff.at(0).toDouble();
        double fMin = ff.at(0).toDouble();
        vv.append(QString("H:%1V,").arg(QString::number(hMin, 'f', 1)));
        vv.append(QString("L:%1V,").arg(QString::number(lMin, 'f', 1)));
        vv.append(QString("F:%1Hz,").arg(QString::number(fMin, 'f', 1)));
        vv.append(QString("D:%1%").arg(QString::number(dMin, 'f', 1)));

        QStringList limit = halltesting->readLimit();

        if (lMax > limit.at(1).toDouble() || lMin < limit.at(0).toDouble())
            jj = "NG";
        if (hMax > limit.at(3).toDouble() || hMin < limit.at(2).toDouble())
            jj = "NG";
        if (fMax > limit.at(5).toDouble() || fMin < limit.at(4).toDouble())
            jj = "NG";
        if (dMax > limit.at(7).toDouble() || dMin < limit.at(6).toDouble())
            jj = "NG";

        text.append(math.powerShow(power, ff));
        test->setTextHall(text);
    } else {
        vv = "NULL";
        jj = "NG";
    }
    test->updateItem(vv);
    test->updateJudge(jj);
}

void MainPage::testNLD()
{
    qDebug() << QTime::currentTime().toString("hh:mm:ss") << "noload start";
    if (!cylinderAction(Y02 | Y10, station)) {
        status = STATUS_OVER;
        return;
    }
    qDebug() << QTime::currentTime().toString("hh:mm:ss") << "noload cylinder ok";
    isLoadStop = false;
    loadStopEnable = false;
    sendUdpCommand("6006 NOLAOD");          // 启动
    waitTimeOut(STATUS_NLD);                // 等待测试完成
    readNLD();
    if (isLoadStop) {
        status = STATUS_OVER;
    } else {
        // nothing
    }
    isLoadStop = false;
    loadStopEnable = false;

    QStringList s = conf->testItems();
    QList<int> tt;
    for (int i=0; i < s.size(); i++) {
        tt.append(QString(s.at(i)).toInt());
    }
    if (tt.indexOf(STATUS_LOD) - tt.indexOf(STATUS_NLD) != 1) {
        if (!cylinderAction(Y10, station))
            status = STATUS_OVER;
    } else {
        if (!cylinderAction(Y00 | Y10, station)) {
            status = STATUS_OVER;
        }
    }
    qDebug() << QTime::currentTime().toString("hh:mm:ss") << "noload over";
}

void MainPage::readNLD()
{
    if (power.size() >= 8 && status != STATUS_OVER) {
        double rpm = power.at(0).toDouble();
        if (halltesting->readCount() != 0)
            rpm /= halltesting->readCount();
        double crr = power.at(4).toDouble();
        double vlt = power.at(5).toDouble();
        double pwr = power.at(6).toDouble();

        QString tt;
        tt.append(QString("%1A,").arg(QString::number(crr, 'f', 4)));
        tt.append(QString("%1V,").arg(QString::number(vlt, 'f', 1)));
        tt.append(QString("%1W,").arg(QString::number(pwr, 'f', 2)));
        tt.append(QString("%1rpm").arg(QString::number(rpm, 'f', 1)));
        test->updateItem(tt);

        QString jj = "OK";
        QStringList ss = noloadtest->readLimit();

        if (crr < ss.at(1).toDouble() || crr > ss.at(2).toDouble())
            jj = "NG";
        if (pwr < ss.at(3).toDouble() || pwr > ss.at(4).toDouble())
            jj = "NG";
        if (rpm < ss.at(5).toDouble() || rpm > ss.at(6).toDouble())
            jj = "NG";
        test->updateJudge(jj);
    } else {
        test->updateItem("NULL");
        test->updateJudge("NG");
    }
}

void MainPage::testLOD()
{
    mbdktPrevAction(station);
    qDebug() << QTime::currentTime().toString("hh:mm:ss") << "mbdkt ok";
    if (status == STATUS_OVER)
        return;
    isLoadStop = false;
    loadStopEnable = false;
    sendUdpCommand("6006 LOAD");          // 启动
    waitTimeOut(STATUS_LOD);                // 等待测试完成
    readLOD();
    mbdktPrevActionStop(station);
    if (isLoadStop) {
        double tmp = qMax(loadtesting->readTorque()-readTorqueComp().toDouble(), 0.0);
        mbdktActionStop(tmp*2500, station);
        status = STATUS_OVER;
        qDebug() << QTime::currentTime().toString("hh:mm:ss") << "load lower ok";
    } else {
        // nothing
    }
    load_timer->stop();
    isLoadStop = false;
    loadStopEnable = false;
    qDebug() << QTime::currentTime().toString("hh:mm:ss") << "load over";
}

void MainPage::readLOD()
{
    if (power.size() >= 8 && status != STATUS_OVER) {
        double rpm = power.at(0).toDouble();
        if (halltesting->readCount() != 0)
            rpm /= halltesting->readCount();
        double crr = power.at(4).toDouble();
        double vlt = power.at(5).toDouble();
        double pwr = power.at(6).toDouble();

        QString tt;
        tt.append(QString("%1A,").arg(QString::number(crr, 'f', 4)));
        tt.append(QString("%1V,").arg(QString::number(vlt, 'f', 1)));
        tt.append(QString("%1W,").arg(QString::number(pwr, 'f', 2)));
        tt.append(QString("%1rpm,").arg(QString::number(rpm, 'f', 1)));
        if (speed > 32767)
            tt.append(QString("CCW"));
        else
            tt.append(QString("CW"));
        test->updateItem(tt);

        QString jj = "OK";
        QStringList ss = loadtesting->readLimit();

        if (crr < ss.at(1).toDouble() || crr > ss.at(2).toDouble())
            jj = "NG";
        if (pwr < ss.at(3).toDouble() || pwr > ss.at(4).toDouble())
            jj = "NG";
        if (rpm < ss.at(5).toDouble() || rpm > ss.at(6).toDouble())
            jj = "NG";
        test->updateJudge(jj);
    } else {
        test->updateItem("NULL");
        test->updateJudge("NG");
    }
}

int MainPage::cylinder_start()
{
    qDebug() << QTime::currentTime().toString("hh:mm:ss") << "load start";
    if (!cylinderAction(Y00 | Y10, station)) {
        status = STATUS_OVER;
        return 1;
    }
    qDebug() << QTime::currentTime().toString("hh:mm:ss") << "cylinder1 ok";
    if (!cylinderAction(Y00 | Y01 | Y10, station)) {
        status = STATUS_OVER;
        return 2;
    }
    qDebug() << QTime::currentTime().toString("hh:mm:ss") << "cylinder2 ok";
    if (!cylinderAction(Y00 | Y01 | Y02 | Y10, station)) {
        status = STATUS_OVER;
        return 3;
    }
    qDebug() << QTime::currentTime().toString("hh:mm:ss") << "cylinder3 ok";
    qDebug() << QTime::currentTime().toString("hh:mm:ss") << "load start ok";
    return 0;
}

bool MainPage::cylinder_stop(int ret)
{
    qDebug() << QTime::currentTime().toString("hh:mm:ss") << "load cylinder stop" << ret;
    if (ret == 0) {
        if (!cylinderAction(Y00 | Y01 | Y10, station)) {
            status = STATUS_OVER;
            return false;
        }
        if (!cylinderAction(Y00 | Y10, station)) {
            status = STATUS_OVER;
            return false;
        }
    }
    qDebug() << QTime::currentTime().toString("hh:mm:ss") << "cylinder1 ok";
    if (ret == 2 || ret == 3) {
        cylinderAction(Y00 | Y01 | Y10, station);
        cylinderAction(Y00 | Y10, station);
    }
    if (!cylinderAction(Y10, station)) {
        status = STATUS_OVER;
        return false;
    }
    qDebug() << QTime::currentTime().toString("hh:mm:ss") << "load cylinder stop ok";
    return true;
}

void MainPage::testStop()
{
    QJsonObject obj;
    obj.insert("TxMessage",QString("6022 %1").arg(station));
    emit transmitJson(obj);
    status = STATUS_OVER;
}

bool MainPage::waitTimeOut(quint16 s)
{
    status = s;
    while (1) {
        wait(10);
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

QString MainPage::readScale()
{
    QSettings *ini = new QSettings("./nandflash/global.ini", QSettings::IniFormat);
    QString temp = ini->value("/SCALE/bemf", "100").toString();
    return temp;
}

QString MainPage::readVoltScale()
{
    QSettings *ini = new QSettings("./nandflash/global.ini", QSettings::IniFormat);
    QString temp = ini->value("/SCALE/volt", "100").toString();
    return temp;
}

QString MainPage::readHighVolt()
{
    QSettings *ini = new QSettings("./nandflash/global.ini", QSettings::IniFormat);
    QString temp = ini->value("/HALL/volt", "100").toString();
    return temp;
}

QString MainPage::readTorqueComp()
{
    QSettings *ini = new QSettings("./nandflash/global.ini", QSettings::IniFormat);
    QString temp = ini->value("/GLOBAL/torqueComp", "0.1").toString();
    return temp;
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
        wait(100);

        QString msg = "6002 ";
        msg.append(doc.toByteArray());
        xx.insert("TxMessage", msg);
        emit transmitJson(xx);
    }
}

void MainPage::readNoLoadStart()
{  
    if (status == STATUS_LOD) {
        if (isLoadStop) {
            loadStopEnable = true;
            return;
        }
        double tmp = qMax(loadtesting->readTorque()-readTorqueComp().toDouble(), 0.0);
        mbdktAction(tmp*2500, station);
        qDebug() << QTime::currentTime().toString("hh:mm:ss") << "load upper ok";
    }
    loadStopEnable = true;
}

void MainPage::readNoLoadStop()
{
    if (status == STATUS_LOD) {
        if (station == 0x13)
            speed = servoL.readPort();
        else
            speed = servoR.readPort();
        double tmp = qMax(loadtesting->readTorque()-readTorqueComp().toDouble(), 0.0);
        mbdktActionStop(tmp*2500, station);
        qDebug() << QTime::currentTime().toString("hh:mm:ss") << "load lower ok";
    }
}

void MainPage::readStart(bool s, quint16 st)
{
    if (!s)
        st = station;
    if (st == 0x13)
        readStartL(s);
    else if (st == 0x14)
        readStartR(s);
}

void MainPage::readStartL(bool s)
{
    if (s && !testing) {
        if (stack->currentWidget()->objectName() == "TestPage") {
            testing = true;
            station = 0x13;
            QTimer::singleShot(1, this, SLOT(testInit()));
        }
    }
    if (!s && testing) {
        if (station != 0x13)
            return;
        if (stopping)
            return;
        stopping = true;
        qDebug() << QTime::currentTime().toString("hh:mm:ss") << "recv stop";
        if (status == STATUS_LOD || status == STATUS_NLD) {
            isLoadStop = true;
            load_timer->start(10);
        } else {
            iobrdL.quitPort(true);
            testStop();
        }
    }
}

void MainPage::readStartR(bool s)
{
    if (s && !testing) {
        if (stack->currentWidget()->objectName() == "TestPage") {
            testing = true;
            station = 0x14;
            QTimer::singleShot(1, this, SLOT(testInit()));
        }
    }
    if (!s && testing) {
        if (station != 0x14)
            return;
        if (stopping)
            return;
        stopping = true;
        qDebug() << QTime::currentTime().toString("hh:mm:ss") << "recv stop";
        if (status == STATUS_LOD || status == STATUS_NLD) {
            isLoadStop = true;
            load_timer->start(10);
        } else {
            iobrdR.quitPort(true);
            testStop();
        }
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

QStringList MainPage::readOffset()
{
    QSettings *ini = new QSettings("./nandflash/global.ini", QSettings::IniFormat);
    QString temp = ini->value("/OFFSET/hall", "0@0@0@0").toString();
    return temp.split("@");
}

void MainPage::sendUdpCommand(QString cmd)
{
    QJsonObject obj;
    obj.insert("TxMessage", cmd);
    emit transmitJson(obj);
}

bool MainPage::cylinderAction(quint16 cylinder, quint16 s)
{
    if (s == 0x13) {
        iobrdL.sendPort(cylinder);
        return iobrdL.waitPort(cylinder);
    } else if (s == 0x14) {
        iobrdR.sendPort(cylinder);
        return iobrdR.waitPort(cylinder);
    }
    return false;
}

bool MainPage::mbdktPrevAction(quint16 s)
{
    if (s == 0x13) {
        if (!mbdktL.setMode(0))
            QMessageBox::warning(this, "", "伺服模式失败", QMessageBox::Ok);
        wait(100);
        if (!mbdktL.setTorque(0))
            QMessageBox::warning(this, "", "伺服转矩失败", QMessageBox::Ok);
        wait(100);
        if (!mbdktL.setStart(1))
            QMessageBox::warning(this, "", "伺服启动失败", QMessageBox::Ok);
        wait(100);
    } else if (s == 0x14) {
        if (!mbdktR.setMode(0))
            QMessageBox::warning(this, "", "伺服模式失败", QMessageBox::Ok);
        wait(100);
        if (!mbdktR.setTorque(0))
            QMessageBox::warning(this, "", "伺服转矩失败", QMessageBox::Ok);
        wait(100);
        if (!mbdktR.setStart(1))
            QMessageBox::warning(this, "", "伺服启动失败", QMessageBox::Ok);
        wait(100);

    }
    return false;
}

bool MainPage::mbdktPrevActionStop(quint16 s)
{
    if (s == 0x13) {
        if (!mbdktL.setStart(0))
            QMessageBox::warning(this, "", "伺服启动失败", QMessageBox::Ok);
        wait(100);
    } else if (s == 0x14) {
        if (!mbdktR.setStart(0))
            QMessageBox::warning(this, "", "伺服启动失败", QMessageBox::Ok);
    }
    return false;
}

bool MainPage::mbdktAction(int torque, quint16 s)
{
    if (s == 0x13) {
        for (int i=1; i < 11; i++) {
            if (!mbdktL.setTorque(torque*i/10))
                QMessageBox::warning(this, "", "伺服转矩失败", QMessageBox::Ok);
            wait(100);
            if (isLoadStop) {
                return true;
            }
        }
    } else if (s == 0x14) {
        for (int i=1; i < 11; i++) {
            if (!mbdktR.setTorque(torque*i/10))
                QMessageBox::warning(this, "", "伺服转矩失败", QMessageBox::Ok);
            wait(100);
            if (isLoadStop) {
                return true;
            }
        }
    }
    return false;
}

bool MainPage::mbdktActionStop(int torque, quint16 s)
{
    if (s == 0x13) {
        for (int i=1; i < 11; i++) {
            mbdktL.setTorque(torque/10*(10-i));
            wait(100);
        }
        return true;
    } else if (s == 0x14) {
        for (int i=1; i < 11; i++) {
            mbdktR.setTorque(torque/10*(10-i));
            wait(100);
        }
        return true;
    }
    return false;
}

void MainPage::recvAppShow(QString win)
{
    if (win == "TestPage") {
        mbdktL.setStart(0);     // 停止伺服
        mbdktR.setStart(0);     // 停止伺服

        iobrdL.sendPort(0x00);  // 气缸全部归位
        iobrdL.waitPort(0x00);  // 等待气缸归位
        iobrdR.sendPort(0x00);  // 气缸全部归位
        iobrdR.waitPort(0x00);  // 等待气缸归位
    }
}

void MainPage::showDebug(QString s)
{
    box2->setText(s);
    box2->show();
    wait(3000);
    box2->hide();
}

void MainPage::closeEvent(QCloseEvent *e)
{
    int ret = 0;
    if (testing)
        ret = QMessageBox::warning(this, "警告", "程序有一个任务正在运行中,确定要关闭软件吗？", QMessageBox::Ok,QMessageBox::Cancel);
    else
        ret = QMessageBox::warning(this, "警告", "确定要关闭软件吗？", QMessageBox::Ok,QMessageBox::Cancel);
    if (ret == QMessageBox::Ok) {
        status = STATUS_OVER;
        e->accept();
    } else {
        e->ignore();
    }
}

void MainPage::iobrdReset()
{
    if (testing) {
        // nothing
    } else {
        mbdktL.setStart(0);
        mbdktR.setStart(0);
        cylinderAction(0x00, 0x13);
        cylinderAction(0x00, 0x14);
    }
}

void MainPage::waitSendStop()
{
    if (isLoadStop && loadStopEnable) {
        sendUdpCommand(QString("6022 %1").arg(station));
        qDebug() << QTime::currentTime().toString("hh:mm:ss") << "send stop";
        load_timer->stop();
    }
}

void MainPage::showWarnning()
{
    //    QString text2 = tr("不放置带连轴器电机时,请禁止启动测试");
    //    PopupBox *box2 = new PopupBox(this, "", text2, QMessageBox::Ok);
    //    wait(100);
    //    box2->exec();
}

int MainPage::currentAlarmTime(QString msg)
{
    QSettings *ini = new QSettings(INI_PATH, QSettings::IniFormat);
    if (msg == "NG")
        return ini->value("/GLOBAL/TimeNG", "0.2").toDouble()*1000;
    else
        return ini->value("/GLOBAL/TimeOK", "0.1").toDouble()*1000;
}
