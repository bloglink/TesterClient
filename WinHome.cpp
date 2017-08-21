/*******************************************************************************
 * Copyright [2016]   <  青岛艾普智能仪器有限公司>
 * All rights reserved.
 *
 * version:     2.1.0.170422
 * author:      zhaonanlin
 * brief:       开机主页
*******************************************************************************/
#include "WinHome.h"
#include "ui_WinHome.h"

WinHome::WinHome(QWidget *parent) :
    QWidget(parent, Qt::FramelessWindowHint),
    ui(new Ui::WinHome)
{
    ui->setupUi(this);
//    InitWindows();
//    InitButtons();
//    InitVersion("V-2.1.0.170704");
//    HomeMode = HOME_FREE;
//    InitThreadAll();
//    isPause = false;
    this->setObjectName("WinHome");
}

WinHome::~WinHome()
{
    delete ui;
}

void WinHome::InitThreadAll()
{
    QTimer *timer = new QTimer(this);
    InitCan();
    InitSql();
    InitTcp();
    InitUdp();
    InitSerial();
    timer->singleShot(50, this, SLOT(InitWindowsAll()));
}

void WinHome::InitWindows()
{
#ifdef __arm__
    ui->btnQuit->hide();
#endif
    ui->keybord->setCurrentIndex(0);
    //设置界面风格
    QFile file;
    QString qss;
    file.setFileName(":/source/SoftRainBlue.css");
    file.open(QFile::ReadOnly);
    qss = QLatin1String(file.readAll());
    qApp->setStyleSheet(qss);
}

void WinHome::InitWindowsAll()
{
    WinBack *winBack = new WinBack(this);
    ui->desktop->addWidget(winBack);
    winBack->setObjectName("WinBack");
    connect(winBack, SIGNAL(SendVariant(QVariant)), this, SLOT(ReadVariant(QVariant)));
    connect(winBack, SIGNAL(SendCommand(quint16, quint16, QByteArray)), this,
            SLOT(ReadMessage(quint16, quint16, QByteArray)));
    connect(this, SIGNAL(SendCommand(quint16, quint16, QByteArray)), winBack,
            SLOT(ReadMessage(quint16, quint16, QByteArray)));
    ShowLogMessage("Initialize WinBack OK\n");

    WinSyst *winSyst = new WinSyst(this);
    ui->desktop->addWidget(winSyst);
    winSyst->setObjectName("WinSyst");
    connect(winSyst, SIGNAL(SendVariant(QVariant)), this, SLOT(ReadVariant(QVariant)));
    connect(winSyst, SIGNAL(SendCommand(quint16, quint16, QByteArray)), this,
            SLOT(ReadMessage(quint16, quint16, QByteArray)));
    connect(this, SIGNAL(SendCommand(quint16, quint16, QByteArray)), winSyst,
            SLOT(ReadMessage(quint16, quint16, QByteArray)));
    ShowLogMessage("Initialize WinSyst OK\n");

    WinType *winType = new WinType(this);
    ui->desktop->addWidget(winType);
    winType->setObjectName("WinType");
    connect(winType, SIGNAL(SendVariant(QVariant)), this, SLOT(ReadVariant(QVariant)));
    connect(winType, SIGNAL(SendCommand(quint16, quint16, QByteArray)), this,
            SLOT(ReadMessage(quint16, quint16, QByteArray)));
    ShowLogMessage("Initialize WinType OK\n");

    WinData *winData = new WinData(this);
    ui->desktop->addWidget(winData);
    winData->setObjectName("WinData");
    connect(winData, SIGNAL(SendVariant(QVariant)), this, SLOT(ReadVariant(QVariant)));
    connect(winData, SIGNAL(SendCommand(quint16, quint16, QByteArray)), this,
            SLOT(ReadMessage(quint16, quint16, QByteArray)));
    ShowLogMessage("Initialize WinData OK\n");

    WinTest *winTest = new WinTest(this);
    ui->desktop->addWidget(winTest);
    winTest->setObjectName("WinTest");
    connect(winTest, SIGNAL(SendVariant(QVariant)), this, SLOT(ReadVariant(QVariant)));
    connect(winTest, SIGNAL(SendCommand(quint16, quint16, QByteArray)), this,
            SLOT(ReadMessage(quint16, quint16, QByteArray)));
    connect(this, SIGNAL(SendCommand(quint16, quint16, QByteArray)), winTest,
            SLOT(ReadMessage(quint16, quint16, QByteArray)));
    ShowLogMessage("Initialize WinTest OK\n");

    PageDcr *pageDcr = new PageDcr(this);
    ui->desktop->addWidget(pageDcr);
    pageDcr->setObjectName("PageDcr");
    connect(pageDcr, SIGNAL(SendVariant(QVariant)), this, SLOT(ReadVariant(QVariant)));
    connect(pageDcr, SIGNAL(SendCommand(quint16, quint16, QByteArray)), this,
            SLOT(ReadMessage(quint16, quint16, QByteArray)));
    connect(this, SIGNAL(SendCommand(quint16, quint16, QByteArray)), pageDcr,
            SLOT(ReadMessage(quint16, quint16, QByteArray)));
    ShowLogMessage("Initialize PageDcr OK\n");

    PageMag *pageMag = new PageMag(this);
    ui->desktop->addWidget(pageMag);
    pageMag->setObjectName("PageMag");
    connect(pageMag, SIGNAL(SendVariant(QVariant)), this, SLOT(ReadVariant(QVariant)));
    connect(pageMag, SIGNAL(SendCommand(quint16, quint16, QByteArray)), this,
            SLOT(ReadMessage(quint16, quint16, QByteArray)));
    connect(this, SIGNAL(SendCommand(quint16, quint16, QByteArray)), pageMag,
            SLOT(ReadMessage(quint16, quint16, QByteArray)));
    ShowLogMessage("Initialize PageMag OK\n");

    PageInr *pageInr = new PageInr(this);
    ui->desktop->addWidget(pageInr);
    pageInr->setObjectName("PageInr");
    connect(pageInr, SIGNAL(SendVariant(QVariant)), this, SLOT(ReadVariant(QVariant)));
    connect(pageInr, SIGNAL(SendCommand(quint16, quint16, QByteArray)), this,
            SLOT(ReadMessage(quint16, quint16, QByteArray)));
    connect(this, SIGNAL(SendCommand(quint16, quint16, QByteArray)), pageInr,
            SLOT(ReadMessage(quint16, quint16, QByteArray)));
    ShowLogMessage("Initialize PageInr OK\n");

    PageAcw *pageAcw = new PageAcw(this);
    ui->desktop->addWidget(pageAcw);
    pageAcw->setObjectName("PageAcw");
    connect(pageAcw, SIGNAL(SendVariant(QVariant)), this, SLOT(ReadVariant(QVariant)));
    connect(pageAcw, SIGNAL(SendCommand(quint16, quint16, QByteArray)), this,
            SLOT(ReadMessage(quint16, quint16, QByteArray)));
    connect(this, SIGNAL(SendCommand(quint16, quint16, QByteArray)), pageAcw,
            SLOT(ReadMessage(quint16, quint16, QByteArray)));
    ShowLogMessage("Initialize PageAcw OK\n");

    PageImp *pageImp = new PageImp(this);
    ui->desktop->addWidget(pageImp);
    pageImp->setObjectName("PageImp");
    connect(pageImp, SIGNAL(SendVariant(QVariant)), this, SLOT(ReadVariant(QVariant)));
    connect(pageImp, SIGNAL(SendCommand(quint16, quint16, QByteArray)), this,
            SLOT(ReadMessage(quint16, quint16, QByteArray)));
    connect(this, SIGNAL(SendCommand(quint16, quint16, QByteArray)), pageImp,
            SLOT(ReadMessage(quint16, quint16, QByteArray)));
    ShowLogMessage("Initialize PageImp OK\n");

    PageInd *pageInd = new PageInd(this);
    ui->desktop->addWidget(pageInd);
    pageInd->setObjectName("PageInd");
    connect(pageInd, SIGNAL(SendVariant(QVariant)), this, SLOT(ReadVariant(QVariant)));
    connect(pageInd, SIGNAL(SendCommand(quint16, quint16, QByteArray)), this,
            SLOT(ReadMessage(quint16, quint16, QByteArray)));
    connect(this, SIGNAL(SendCommand(quint16, quint16, QByteArray)), pageInd,
            SLOT(ReadMessage(quint16, quint16, QByteArray)));
    ShowLogMessage("Initialize PageInd OK\n");

    PagePwr *pagePwr = new PagePwr(this);
    ui->desktop->addWidget(pagePwr);
    pagePwr->setObjectName("PagePwr");
    connect(pagePwr, SIGNAL(SendVariant(QVariant)), this, SLOT(ReadVariant(QVariant)));
    connect(pagePwr, SIGNAL(SendCommand(quint16, quint16, QByteArray)), this,
            SLOT(ReadMessage(quint16, quint16, QByteArray)));
    connect(this, SIGNAL(SendCommand(quint16, quint16, QByteArray)), pagePwr,
            SLOT(ReadMessage(quint16, quint16, QByteArray)));
    ShowLogMessage("Initialize PagePwr OK\n");

    PageLvs *pageLvs = new PageLvs(this);
    ui->desktop->addWidget(pageLvs);
    pageLvs->setObjectName("PageLvs");
    connect(pageLvs, SIGNAL(SendVariant(QVariant)), this, SLOT(ReadVariant(QVariant)));
    connect(pageLvs, SIGNAL(SendCommand(quint16, quint16, QByteArray)), this,
            SLOT(ReadMessage(quint16, quint16, QByteArray)));
    connect(this, SIGNAL(SendCommand(quint16, quint16, QByteArray)), pageLvs,
            SLOT(ReadMessage(quint16, quint16, QByteArray)));
    ShowLogMessage("Initialize PageLvs OK\n");

    PageLck *pageLck = new PageLck(this);
    ui->desktop->addWidget(pageLck);
    pageLck->setObjectName("PageLck");
    connect(pageLck, SIGNAL(SendVariant(QVariant)), this, SLOT(ReadVariant(QVariant)));
    connect(pageLck, SIGNAL(SendCommand(quint16, quint16, QByteArray)), this,
            SLOT(ReadMessage(quint16, quint16, QByteArray)));
    connect(this, SIGNAL(SendCommand(quint16, quint16, QByteArray)), pageLck,
            SLOT(ReadMessage(quint16, quint16, QByteArray)));
    ShowLogMessage("Initialize PageLck OK\n");

    PageOut *pageOut = new PageOut(this);
    ui->desktop->addWidget(pageOut);
    pageOut->setObjectName("PageOut");
    connect(pageOut, SIGNAL(SendVariant(QVariant)), this, SLOT(ReadVariant(QVariant)));
    connect(pageOut, SIGNAL(SendCommand(quint16, quint16, QByteArray)), this,
            SLOT(ReadMessage(quint16, quint16, QByteArray)));
    connect(this, SIGNAL(SendCommand(quint16, quint16, QByteArray)), pageOut,
            SLOT(ReadMessage(quint16, quint16, QByteArray)));
    ShowLogMessage("Initialize PageOut OK\n");

    PageAmp *pageAmp = new PageAmp(this);
    pageOut->setObjectName("pageAmp");
    connect(pageAmp, SIGNAL(SendVariant(QVariant)), this, SLOT(ReadVariant(QVariant)));
    connect(pageAmp, SIGNAL(SendCommand(quint16, quint16, QByteArray)), this,
            SLOT(ReadMessage(quint16, quint16, QByteArray)));
    connect(this, SIGNAL(SendCommand(quint16, quint16, QByteArray)), pageAmp,
            SLOT(ReadMessage(quint16, quint16, QByteArray)));
    ShowLogMessage("Initialize PageAmp OK\n");

    ReadStatusAll();
}

void WinHome::JumpToWindow(QByteArray win)
{
    if (HomeMode == HOME_TEST)
        return;
    int WinCurrent = ui->desktop->currentIndex();
    if (win.isNull()) { //空代表返回
        ui->desktop->setCurrentIndex(previous_window.last());
        previous_window.removeLast();
        return;
    }
    for (int i=0; i < ui->desktop->count(); i++) {
        if (ui->desktop->widget(i)->objectName() == win) {
            ui->desktop->setCurrentIndex(i);
            break;
        }
    }
    previous_window.append(WinCurrent);
    if (previous_window.size() > 10) { // 最大嵌套10层
        previous_window.removeFirst();
    }
}

void WinHome::InitButtons()
{
    QButtonGroup *btnGroup = new QButtonGroup;
    btnGroup->addButton(ui->btnSyst, Qt::Key_1);
    btnGroup->addButton(ui->btnType, Qt::Key_2);
    btnGroup->addButton(ui->btnData, Qt::Key_3);
    btnGroup->addButton(ui->btnTest, Qt::Key_4);
    btnGroup->addButton(ui->btnQuit, Qt::Key_5);
    connect(btnGroup, SIGNAL(buttonClicked(int)), this, SLOT(ReadButtons(int)));
}

void WinHome::ReadButtons(int id)
{
    switch (id) {
    case Qt::Key_1:
        JumpToWindow("WinSyst");
        break;
    case Qt::Key_2:
        JumpToWindow("WinType");
        break;
    case Qt::Key_3:
        JumpToWindow("WinData");
        break;
    case Qt::Key_4:
        JumpToWindow("WinTest");
        break;
    case Qt::Key_5:
        QApplication::closeAllWindows();
        break;
    }
}

void WinHome::InitVersion(QString v)
{
    QSettings *ini = new QSettings(INI_PATH, QSettings::IniFormat);
    ini->setValue("/GLOBAL/Version", v);
    this->setWindowTitle(QString("电机综合测试仪%1").arg(v));
    ui->titleVn->setText(v);
}

void WinHome::InitCan()
{
    thread_can = new QThread(this);
    can.moveToThread(thread_can);
    connect(thread_can, SIGNAL(started()), &can, SLOT(DeviceOpen()));
    connect(thread_can, SIGNAL(finished()), &can, SLOT(DeviceQuit()));
    connect(this, SIGNAL(PutCanData(QByteArray)), &can, SLOT(WriteAll(QByteArray)));
    connect(&can, SIGNAL(GetCanData(QByteArray)), this, SLOT(ReadCanCmd(QByteArray)));
    thread_can->start();
}

void WinHome::InitSql()
{
    thread_sql = new QThread(this);
    sql.moveToThread(thread_sql);
    connect(thread_sql, SIGNAL(started()), &sql, SLOT(DeviceOpen()));
    connect(thread_sql, SIGNAL(finished()), &sql, SLOT(DeviceQuit()));
    connect(this, SIGNAL(WriteSql(QByteArray)), &sql, SLOT(Write(QByteArray)));
    thread_sql->start();
}

void WinHome::InitTcp()
{
    thread_tcp = new QThread(this);
    tcp.moveToThread(thread_tcp);
    connect(thread_tcp, SIGNAL(started()), &tcp, SLOT(TcpInit()));
    connect(thread_tcp, SIGNAL(finished()), &tcp, SLOT(TcpQuit()));
    connect(&tcp, SIGNAL(SendCommand(quint16, quint16, QByteArray)), this,
            SLOT(ReadMessage(quint16, quint16, QByteArray)));
    thread_tcp->start();
}

void WinHome::InitUdp()
{
    thread_udp = new QThread(this);
    udp.moveToThread(thread_udp);
    connect(thread_udp, SIGNAL(started()), &udp, SLOT(Init()));
    connect(thread_udp, SIGNAL(finished()), &udp, SLOT(Quit()));
    connect(&udp, SIGNAL(SendCommand(quint16, quint16, QByteArray)), this,
            SLOT(ReadMessage(quint16, quint16, QByteArray)));
    connect(this, SIGNAL(SendCommand(quint16, quint16, QByteArray)), &udp,
            SLOT(ReadMessage(quint16, quint16, QByteArray)));
    thread_udp->start();
}

void WinHome::InitSerial()
{
    thread_all = new QThread(this);
    serial.moveToThread(thread_all);
    connect(thread_all, SIGNAL(started()), &serial, SLOT(OpenSerial()));
    connect(thread_all, SIGNAL(finished()), &serial, SLOT(CloseSerial()));
    connect(&serial, SIGNAL(SendCommand(quint16, quint16, QByteArray)), this,
            SLOT(ReadMessage(quint16, quint16, QByteArray)));
    connect(this, SIGNAL(SendCommand(quint16, quint16, QByteArray)), &serial,
            SLOT(ReadMessage(quint16, quint16, QByteArray)));
    thread_all->start();
}

void WinHome::ReadCanCmd(QByteArray msg)
{
    if (!msg.isEmpty()) {
        quint16 id;
        quint8 dlc;
        quint8 dat;
        QByteArray cmd;
        QDataStream in(&msg,  QIODevice::ReadWrite);
        in.setVersion(QDataStream::Qt_4_8);

        while (!in.atEnd()) {
            in >> id >> dlc;
            cmd.clear();
            for (int i=0; i < dlc; i++) {
                in >> dat;
                cmd.append(dat);
            }
            if (id == CAN_ID_DCR && quint8(cmd.at(0)) == 0x09)
                ReadButtonBox(cmd);
            else
                emit SendCommand(id, CMD_CAN, cmd);
        }
    }
}

void WinHome::ReadMessage(quint16 addr,  quint16 cmd,  QByteArray msg)
{
    if (addr != ADDR) {
        emit SendCommand(addr, cmd, msg);
        return;
    }
    switch (cmd) {
    case CMD_JUMP:
        JumpToWindow(msg);
        break;
    case CMD_INIT:
        InitTestItems();
        break;
    case CMD_STATUS:
        ReadStatusAll();
        break;
    case CMD_INIT_ITEM:
        if (isPause) {
            TempItems.append(QString(msg).split("\n"));
            break;
        }
        Items.append(QString(msg).split("\n"));
        break;
    case CMD_JUDGE:
        SaveItemJudge(msg);
        break;
    case CMD_ITEM:
        emit WriteSql(msg);
        emit SendCommand(ADDR, cmd, msg);
        break;
    case CMD_ITEM_TEMP:
        emit SendCommand(ADDR, CMD_ITEM, msg);
        break;
    case CMD_TEMP:
    case CMD_WAVE_BYTE:
    case CMD_WAVE_TEST:
    case CMD_WAVE_ITEM:
    case CMD_WAVE_HIDE:
        emit SendCommand(WIN_ID_TEST, cmd, msg);
        break;
    case CMD_DEBUG:
        ShowLogMessage(msg);
        break;
    case CMD_WAVE:
        emit SendCommand(ADDR, CMD_WAVE, msg);
        break;
    case CMD_CAN:
        emit PutCanData(msg);
        break;
    case CMD_START:
        if (HomeMode != HOME_FREE)
            break;
        if (QString(msg).split(" ").size() < 2)
            return;
        if (QString(msg).split(" ").at(1).toInt() != CurrentStartMode())
            return;
        StartTest(QString(msg).split(" ").at(0).toUtf8());
        break;
    case CMD_STOP:
        ItemJudge = "NG";
        SendButtonBox("Ok");
        if (msg.contains("DCR"))
            ShowLogMessage(msg);
        emit SendCommand(ADDR, CMD_STOP, msg);
        if (HomeMode != HOME_FREE)
            HomeMode = HOME_STOP;
        else
            InitTestItems();
        break;
    case CMD_NET:
        ui->IconNet->setPixmap(QPixmap(":/source/wifi.png"));
        break;
    case CMD_SQL:
        ui->IconNet->setPixmap(QPixmap(":/source/radio_1.png"));
        break;
    case CMD_CODE:
        emit WriteSql(msg);
        break;
    default:
        qDebug() << addr << cmd << msg;
        break;
    }
}

void WinHome::InitTestItems()
{
    emit SendCommand(ADDR, CMD_ALARM, QByteArray(1, 0x00));
    QStringList n = CurrentItems();
    if (n.isEmpty())
        return;
    Items.clear();
    if (CurrentStartMode() == 1 || CurrentStartMode() == 2) //滑罩启动
        emit SendCommand(WIN_ID_OUT13, CMD_INIT, QString::number(CurrentStartMode()).toUtf8());

    if (!n.contains(QString::number(WIN_ID_DCR)) && n.contains(QString::number(WIN_ID_PWR))) {
        n.insert(0, QString::number(WIN_ID_DCR));
    }
    if (!n.contains(QString::number(WIN_ID_DCR)) && n.contains(QString::number(WIN_ID_LVS))) {
        n.insert(0, QString::number(WIN_ID_DCR));
    }
    if (!n.contains(QString::number(WIN_ID_DCR)) && n.contains(QString::number(WIN_ID_LCK))) {
        n.insert(0, QString::number(WIN_ID_DCR));
    }
    for (int i=0; i < n.size(); i++) {
        emit SendCommand(n.at(i).toInt(), CMD_INIT, NULL);
    }
    emit SendCommand(WIN_ID_TEST, CMD_INIT, Items.join("\n").toUtf8()); //初始化测试界面
}

void WinHome::ReadStatusAll()
{
    if (HomeMode != HOME_FREE)
        return;
    HomeMode = HOME_TEST;

    emit SendCommand(WIN_ID_AMP, CMD_CHECK, NULL);

    QStringList t = EnableItems();
    for (int i=0; i < t.size(); i++) {
        emit SendCommand(t.at(i).toInt(), CMD_CHECK, NULL);
    }
    QStringList s = EnableOutput();
    for (int i=0; i < s.size(); i++) {
        emit SendCommand(WIN_ID_OUT13, CMD_CHECK, s.at(i).toUtf8());
    }
    qDebug() << QTime::currentTime().toString() << "WinHome read OK";

    ui->Text->hide();
    ui->keybord->setCurrentIndex(1);

    HomeMode = HOME_FREE;
    if (ui->desktop->currentWidget()->objectName() == "MainPage") {
        Delay(1000);
        JumpToWindow("WinTest");
    }
}

void WinHome::StartTest(QByteArray station)
{
    stat = station;
    if (CurrentReStartMode() == 1)
        stat = QString("%1").arg(0x13).toUtf8();
    if (CurrentReStartMode() == 2)
        stat = QString("%1").arg(0x14).toUtf8();
    if (HomeMode == HOME_TEST)
        return;
    if (ui->desktop->currentWidget()->objectName() != "WinTest")
        return;
    HomeMode = HOME_TEST;
    ItemJudge = "OK";

    InitTestItems();
    emit SendCommand(ADDR, CMD_STATUS, "buzy");
    emit SendCommand(WIN_ID_TEST, CMD_START, station);
    emit SendCommand(ADDR, CMD_ALARM, QByteArray(1, 0x02 | 0x00));

    QStringList n = CurrentItems();
    if (!n.contains(QString::number(WIN_ID_DCR)) && n.contains(QString::number(WIN_ID_PWR))) {
        n.insert(0, QString::number(WIN_ID_DCR));
    }
    if (!n.contains(QString::number(WIN_ID_DCR)) && n.contains(QString::number(WIN_ID_LVS))) {
        n.insert(0, QString::number(WIN_ID_DCR));
    }
    if (!n.contains(QString::number(WIN_ID_DCR)) && n.contains(QString::number(WIN_ID_LCK))) {
        n.insert(0, QString::number(WIN_ID_DCR));
    }
    for (int i=0; i < n.size(); i++) {
        Current_Test_Item = n.at(i).toInt();
        emit SendCommand(n.at(i).toInt(), CMD_START, station);
        if (HomeMode == HOME_STOP) {
//            QVariantHash hash;
//            hash.insert("TxMessage", tr("测试中断"));
//            Warnning(hash);
            break;
        }
        Delay(10);
    }
    SaveTestJudge();
    if (ItemJudge == "NG") {
        emit SendCommand(ADDR, CMD_ALARM, QByteArray(1, 0x08 | 0x01));
        Delay(CurrentAlarmTime("NG"));
        emit SendCommand(ADDR, CMD_ALARM, QByteArray(1, 0x08 | 0x00));
    } else {
        emit SendCommand(ADDR, CMD_ALARM, QByteArray(1, 0x04 | 0x01));
        Delay(CurrentAlarmTime("OK"));
        emit SendCommand(ADDR, CMD_ALARM, QByteArray(1, 0x04 | 0x00));
    }
    emit SendCommand(WIN_ID_TEST, CMD_JUDGE, ItemJudge.toUtf8());

    emit SendCommand(ADDR, CMD_STATUS, "ready");
    if (CurrentReStartMode() != 0 && HomeMode != HOME_STOP) {
        QTimer *timer = new QTimer(this);
        timer->singleShot(CurrentDelay(),  this,  SLOT(ReStartTest()));
    }
    HomeMode = HOME_FREE;
}

void WinHome::ReStartTest()
{
    switch (CurrentReStartMode()) {
    case 0:
        break;
    case 1:
        StartTest(QString("%1").arg(0x13).toUtf8());
        break;
    case 2:
        StartTest(QString("%1").arg(0x14).toUtf8());
        break;
    case 3:
        StartTest(stat);
        break;
    case 4:
        if (stat.toInt() == 0x13) {
            StartTest(QString("%1").arg(0x14).toUtf8());
            break;
        }
        if (stat.toInt() == 0x14) {
            StartTest(QString("%1").arg(0x13).toUtf8());
            break;
        }
        break;
    default:
        break;
    }
}

void WinHome::SaveTestJudge()
{
    QString s = QString(tr("总数@%1@%2")).arg(CurrentSettings()).arg(ItemJudge);
    emit WriteSql(s.toUtf8());
}

void WinHome::SaveItemJudge(QByteArray msg)
{
    emit WriteSql(msg);
    QStringList s = QString(msg).split("@");
    if (s.size() < 3)
        return;
    if (s.at(2) == "NG")
        ItemJudge = "NG";
    if (s.at(2) == "NG" && CurrentPauseMode() != 1)
        TestPause();
}

void WinHome::TestPause()
{
    isPause = true;
    TempItems.clear();
    QString text = tr("此项目不合格,是否重测");
    PopupBox *box = new PopupBox(this, "", text, QMessageBox::Retry, QMessageBox::Ok);
    connect(this, SIGNAL(SendVariant(QVariant)), box, SLOT(ReadVariant(QVariant)));
    emit SendCommand(ADDR, CMD_ALARM, QByteArray(1, 0x0A | 0x01));
    Delay(CurrentAlarmTime("NG"));
    emit SendCommand(ADDR, CMD_ALARM, QByteArray(1, 0x0A | 0x00));
    int ret = box->exec();
    if (ret == QMessageBox::Retry)
    {
        ItemJudge = "OK";
        if (HomeMode != HOME_STOP)
            emit SendCommand(ADDR, CMD_ALARM, QByteArray(1, 0x02 | 0x00));
        emit SendCommand(Current_Test_Item, CMD_INIT, stat);
        emit SendCommand(WIN_ID_TEST, CMD_ITEM_REPLACE, TempItems.join("\n").toUtf8());
        emit SendCommand(Current_Test_Item, CMD_START, stat);
        Delay(10);
    } else if (ret == QMessageBox::Ok) {
        if (HomeMode != HOME_STOP)
            emit SendCommand(ADDR, CMD_ALARM, QByteArray(1, 0x02 | 0x00));
    }
    isPause = false;
    TempItems.clear();
}

void WinHome::ShowLogMessage(QByteArray msg)
{
    if (!ui->Text->isHidden()) {
        ui->Text->insertPlainText(msg);
        ui->Text->moveCursor(QTextCursor::EndOfBlock);
        Delay(1);
    }
    emit SendCommand(ADDR, CMD_DEBUG, msg);
}

bool WinHome::WaitTimeOut(quint16 t)
{
    int TimeOut = 0;
    while (HomeMode != HOME_FREE) {
        Delay(10);
        TimeOut++;
        if (TimeOut > t)
            return false;
    }
    return true;
}

void WinHome::Delay(int ms)
{
    QElapsedTimer t;
    t.start();
    while (t.elapsed() < ms)
        QCoreApplication::processEvents();
}

QString WinHome::CurrentSettings()
{
    QSettings *ini = new QSettings(INI_PATH, QSettings::IniFormat);
    QString n = ini->value("/GLOBAL/FileInUse", INI_DEFAULT).toString();
    return n.remove(".ini");
}

QStringList WinHome::CurrentItems()
{
    QString n = QString("./config/%1.ini").arg(CurrentSettings());
    QSettings *ini = new QSettings(n, QSettings::IniFormat);
    QString s = ini->value("/GLOBAL/ProjToTest", "1").toString();
    return s.split(" ");
}

QStringList WinHome::EnableItems()
{
    QSettings *ini = new QSettings(INI_PATH, QSettings::IniFormat);
    QString n = ini->value("/GLOBAL/ItemEnable", INI_DEFAULT).toString();
    return n.split(" ");
}

QStringList WinHome::EnableOutput()
{
    QSettings *ini = new QSettings(INI_PATH, QSettings::IniFormat);
    QString n = ini->value("/GLOBAL/OutEnable", INI_DEFAULT).toString();
    return n.split(" ");
}

int WinHome::CurrentStartMode()
{
    QSettings *ini = new QSettings(INI_PATH, QSettings::IniFormat);
    return ini->value("/GLOBAL/Mode", "0").toInt();
}

int WinHome::CurrentPauseMode()
{
    QString n = QString("./config/%1.ini").arg(CurrentSettings());
    QSettings *ini = new QSettings(n, QSettings::IniFormat);
    return ini->value("/GLOBAL/TestNG", "1").toInt();
}

int WinHome::CurrentAlarmTime(QString msg)
{
    QSettings *ini = new QSettings(INI_PATH, QSettings::IniFormat);
    if (msg == "NG")
        return ini->value("/GLOBAL/TimeNG", "0.2").toDouble()*1000;
    else
        return ini->value("/GLOBAL/TimeOK", "0.1").toDouble()*1000;
}

int WinHome::CurrentReStartMode()
{
    QSettings *ini = new QSettings(INI_PATH, QSettings::IniFormat);
    return ini->value("/GLOBAL/RestartMode", "0").toInt();
}

int WinHome::CurrentDelay()
{
    QSettings *ini = new QSettings(INI_PATH, QSettings::IniFormat);
    return ini->value("/GLOBAL/TestDelay", "1").toDouble()*1000;
}


void WinHome::ReadVariant(QVariant s)
{
    QVariantHash hash = s.toHash();
    if (hash.value("TxAddress") != "WinHome") {
        emit SendVariant(s);
        return;
    }
    if (hash.value("TxCommand") == "Warnning")
        Warnning(hash);
    if (hash.value("TxCommand") == "Error")
        Error(hash);
}

void WinHome::Error(QVariantHash hash)
{
    QString text = hash.value("TxMessage").toString();
    PopupBox *box = new PopupBox(this, "", text, NULL, QMessageBox::NoButton);
    box->exec();
}

void WinHome::Warnning(QVariantHash hash)
{
    QString text = hash.value("TxMessage").toString();
    PopupBox *box = new PopupBox(this, "", text, NULL, QMessageBox::Ok);
    connect(this, SIGNAL(SendVariant(QVariant)), box, SLOT(ReadVariant(QVariant)));
    box->exec();
}

void WinHome::SendButtonBox(QString button)
{
    QVariantHash hash;
    hash.insert("TxAddress", "WinHome");
    hash.insert("TxCommand", "BoxButton");
    hash.insert("TxMessage", button);
    emit SendVariant(QVariant::fromValue(hash));
}

void WinHome::ReadButtonBox(QByteArray msg)
{
    if (quint8(msg.at(1)) != 0)
        SendButtonBox("Retry");
    if (quint8(msg.at(2)) != 0)
        SendButtonBox("Ok");
}
/*********************************END OF FILE**********************************/
