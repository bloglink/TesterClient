/*******************************************************************************
 * Copyright [2016]   <  青岛艾普智能仪器有限公司>
 * All rights reserved.
 *
 * version:     2.1.0.170419
 * author:      zhaonanlin
 * brief:       交耐配置与测试流程
*******************************************************************************/
#include "PageAcw.h"
#include "ui_PageAcw.h"

PageAcw::PageAcw(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::PageAcw)
{
    ui->setupUi(this);
    InitWindows();
    InitButtons();
    InitSettings();
    Mode = ACW_FREE;
}

PageAcw::~PageAcw()
{
    delete ui;
}

void PageAcw::InitWindows()
{
#if (QT_VERSION <= QT_VERSION_CHECK(5, 0, 0))
    ui->TabParams->horizontalHeader()->setResizeMode(1, QHeaderView::Stretch);
    ui->TabParams->horizontalHeader()->setResizeMode(2, QHeaderView::Stretch);
    ui->TabParams->horizontalHeader()->setResizeMode(3, QHeaderView::Stretch);
    ui->TabParams->horizontalHeader()->setResizeMode(4, QHeaderView::Stretch);
    ui->TabParams->horizontalHeader()->setResizeMode(5, QHeaderView::Stretch);
    ui->TabParams->horizontalHeader()->setResizeMode(6, QHeaderView::Stretch);
    ui->TabParams->horizontalHeader()->setResizeMode(7, QHeaderView::Stretch);
    ui->TabParams->horizontalHeader()->setResizeMode(8, QHeaderView::Stretch);
    ui->TabParams->horizontalHeader()->setResizeMode(9, QHeaderView::Stretch);
    ui->TabParams->verticalHeader()->setResizeMode(QHeaderView::Stretch);
#else
    ui->TabParams->horizontalHeader()->setSectionResizeMode(1, QHeaderView::Stretch);
    ui->TabParams->horizontalHeader()->setSectionResizeMode(2, QHeaderView::Stretch);
    ui->TabParams->horizontalHeader()->setSectionResizeMode(3, QHeaderView::Stretch);
    ui->TabParams->horizontalHeader()->setSectionResizeMode(4, QHeaderView::Stretch);
    ui->TabParams->horizontalHeader()->setSectionResizeMode(5, QHeaderView::Stretch);
    ui->TabParams->horizontalHeader()->setSectionResizeMode(6, QHeaderView::Stretch);
    ui->TabParams->horizontalHeader()->setSectionResizeMode(7, QHeaderView::Stretch);
    ui->TabParams->horizontalHeader()->setSectionResizeMode(8, QHeaderView::Stretch);
    ui->TabParams->horizontalHeader()->setSectionResizeMode(9, QHeaderView::Stretch);
    ui->TabParams->verticalHeader()->setSectionResizeMode(QHeaderView::Stretch);
#endif
    connect(ui->TabParams, SIGNAL(cellClicked(int, int)), this, SLOT(ItemClick(int, int)));
    ui->Input->hide();
    Check << ui->Box1 << ui->Box2 << ui->Box3 << ui->Box4
          << ui->Box5 << ui->Box6 << ui->Box7 << ui->Box8;
    ui->TabParams->setRowCount(ACW_ROW);
    for (int row=0; row < ACW_ROW; row++) {
        Enable.append(new QTableWidgetItem);
        ui->TabParams->setItem(row, 0, Enable.at(row));
        Enable.at(row)->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);
        Enable.at(row)->setTextAlignment(Qt::AlignCenter);

        Terminal1.append(new QTableWidgetItem);
        ui->TabParams->setItem(row, 1, Terminal1.at(row));
        Terminal1.at(row)->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);
        Terminal1.at(row)->setTextAlignment(Qt::AlignCenter);

        Terminal2.append(new QTableWidgetItem);
        ui->TabParams->setItem(row, 2, Terminal2.at(row));
        Terminal2.at(row)->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);
        Terminal2.at(row)->setTextAlignment(Qt::AlignCenter);

        Vol.append(new QDoubleSpinBox(this));
        ui->TabParams->setCellWidget(row, 3, Vol.at(row));
        Vol.at(row)->setMaximum(3000);
        Vol.at(row)->setDecimals(0);
        Vol.at(row)->setAlignment(Qt::AlignHCenter);
        Vol.at(row)->setButtonSymbols(QDoubleSpinBox::NoButtons);
        Vol.at(row)->setFocusPolicy(Qt::ClickFocus);

        Min.append(new QDoubleSpinBox(this));
        ui->TabParams->setCellWidget(row, 4, Min.at(row));
        Min.at(row)->setMaximum(25);
        Min.at(row)->setAlignment(Qt::AlignHCenter);
        Min.at(row)->setButtonSymbols(QDoubleSpinBox::NoButtons);
        Min.at(row)->setFocusPolicy(Qt::ClickFocus);

        Max.append(new QDoubleSpinBox(this));
        ui->TabParams->setCellWidget(row, 5, Max.at(row));
        Max.at(row)->setMaximum(25);
        Max.at(row)->setAlignment(Qt::AlignHCenter);
        Max.at(row)->setButtonSymbols(QDoubleSpinBox::NoButtons);
        Max.at(row)->setFocusPolicy(Qt::ClickFocus);

        Time.append(new QDoubleSpinBox(this));
        ui->TabParams->setCellWidget(row, 6, Time.at(row));
        Time.at(row)->setMaximum(100);
        Time.at(row)->setDecimals(1);
        Time.at(row)->setAlignment(Qt::AlignHCenter);
        Time.at(row)->setButtonSymbols(QDoubleSpinBox::NoButtons);
        Time.at(row)->setFocusPolicy(Qt::ClickFocus);

        Freq.append(new QComboBox(this));
        ui->TabParams->setCellWidget(row, 7, Freq.at(row));
        QStringList t2;
        t2 << "50" << "60";
        Freq.at(row)->setView(new QListView(this));
        Freq.at(row)->addItems(t2);

        Arc.append(new QComboBox(this));
        ui->TabParams->setCellWidget(row, 8, Arc.at(row));
        QStringList t3;
        t3 << "0" << "1" << "2" << "3" << "4" << "5" << "6" << "7" << "8" << "9";
        Arc.at(row)->setView(new QListView(this));
        Arc.at(row)->addItems(t3);

        Offset.append(new QDoubleSpinBox(this));
        ui->TabParams->setCellWidget(row, 9, Offset.at(row));
        Offset.at(row)->setMaximum(25);
        Offset.at(row)->setAlignment(Qt::AlignHCenter);
        Offset.at(row)->setButtonSymbols(QDoubleSpinBox::NoButtons);
        Offset.at(row)->setFocusPolicy(Qt::ClickFocus);
    }
}

void PageAcw::InitButtons()
{
    QButtonGroup *btnGroup = new QButtonGroup;
    btnGroup->addButton(ui->BtnExitAcw, Qt::Key_0);
    btnGroup->addButton(ui->btnInput, Qt::Key_1);
    btnGroup->addButton(ui->BtnOffset, Qt::Key_2);
    connect(btnGroup, SIGNAL(buttonClicked(int)), this, SLOT(ReadButtons(int)));
}

void PageAcw::ReadButtons(int id)
{
    switch (id) {
    case Qt::Key_0:
        SaveSettings();
        emit SendCommand(ADDR, CMD_JUMP, NULL);
        break;
    case Qt::Key_1:
        EnsureInput();
        break;
    case Qt::Key_2:
        for (int row = 0; row < Enable.size(); row++) {
            if (Enable.at(row)->text() == "Y") {
                Mode = ACW_OFFSET;
                TestRow = row;
                SendCanCmdConfig(row);
                Delay(5);
                SendCanCmdStart(WIN_ID_OUT13);
                if (!WaitTimeOut(100)) {
                    ui->Text->setText("Time out");
                    break;
                }
                Delay(100);
                Mode = ACW_FREE;
            }
        }
        Mode = ACW_FREE;
        break;
    default:
        break;
    }
}

void PageAcw::InitSettings()
{
    //当前使用的测试项目
    QString t = QString("./config/%1.ini").arg(CurrentSettings());
    QSettings *ini = new QSettings(t, QSettings::IniFormat);
    ini->setIniCodec("GB18030");
    ini->beginGroup("SetAcw");

    QStringList temp;
    //可用
    temp = (QString(ini->value("Enable", "Y Y Y Y"). toByteArray())).split(" ");
    for (int row=0; row < qMin(temp.size(), ACW_ROW); row++)
        Enable.at(row)->setText(temp.at(row));
    //端一
    temp = (ini->value("Terminal1", "PE 1 4 7").toString()).split(" ");
    for (int row=0; row < qMin(temp.size(), ACW_ROW); row++)
        Terminal1.at(row)->setText(temp.at(row));
    //端二
    temp = (ini->value("Terminal2", "ALL 2 3 5").toString()).split(" ");
    for (int row=0; row < qMin(temp.size(), ACW_ROW); row++)
        Terminal2.at(row)->setText(temp.at(row));
    //电压
    temp = (ini->value("Voltage", "500 500 500 500").toString()).split(" ");
    for (int row=0; row < qMin(temp.size(), ACW_ROW); row++)
        Vol.at(row)->setValue(temp.at(row).toDouble());
    //电流下限
    temp = (ini->value("Min", "0 0 0 0").toString()).split(" ");
    for (int row=0; row < qMin(temp.size(), ACW_ROW); row++)
        Min.at(row)->setValue(temp.at(row).toDouble());
    //电流上限
    temp = (ini->value("Max", "5 5 5 5").toString()).split(" ");
    for (int row=0; row < qMin(temp.size(), ACW_ROW); row++)
        Max.at(row)->setValue(temp.at(row).toDouble());
    //测试时间
    temp = (ini->value("Time", "1 1 1 1").toString()).split(" ");
    for (int row=0; row < qMin(temp.size(), ACW_ROW); row++)
        Time.at(row)->setValue(temp.at(row).toDouble());
    //频率
    temp = (QString(ini->value("Freq", "0 0 0 0").toByteArray())).split(" ");
    for (int row=0; row < qMin(temp.size(), ACW_ROW); row++)
        Freq.at(row)->setCurrentIndex(temp.at(row).toInt());
    //电弧
    temp = (QString(ini->value("Arc", "0 0 0 0").toByteArray())).split(" ");
    for (int row=0; row < qMin(temp.size(), ACW_ROW); row++)
        Arc.at(row)->setCurrentIndex(temp.at(row).toInt());
    //补偿
    temp = (ini->value("Offset", "0 0 0 0").toString()).split(" ");
    for (int row=0; row < qMin(temp.size(), ACW_ROW); row++)
        Offset.at(row)->setValue(temp.at(row).toDouble());
    if (!EnablePhase()) {
        Enable.at(0)->setText("Y");
        Enable.at(1)->setText("N");
        Enable.at(2)->setText("N");
        Enable.at(3)->setText("N");
        ui->TabParams->hideColumn(2);
        ui->TabParams->hideColumn(1);
        ui->TabParams->hideColumn(0);
        ui->TabParams->hideRow(3);
        ui->TabParams->hideRow(2);
        ui->TabParams->hideRow(1);
    } else {
        ui->TabParams->showColumn(0);
        ui->TabParams->showColumn(1);
        ui->TabParams->showColumn(2);
        ui->TabParams->showRow(1);
        ui->TabParams->showRow(2);
        ui->TabParams->showRow(3);
    }
    qDebug() << QTime::currentTime().toString() << "PageAcw read OK";
}

void PageAcw::SaveSettings()
{
    QString t = QString("./config/%1.ini").arg(CurrentSettings());
    QSettings *ini = new QSettings(t, QSettings::IniFormat);
    ini->setIniCodec("GB18030");
    ini->beginGroup("SetAcw");
    QStringList temp;
    temp.clear();
    for (int i=0; i < Enable.size(); i++)
        temp.append(Enable.at(i)->text());
    ini->setValue("Enable", (temp.join(" ").toUtf8()));
    temp.clear();
    for (int i=0; i < Terminal1.size(); i++)
        temp.append(Terminal1.at(i)->text());
    ini->setValue("Terminal1", (temp.join(" ").toUtf8()));
    temp.clear();
    for (int i=0; i < Terminal2.size(); i++)
        temp.append(Terminal2.at(i)->text());
    ini->setValue("Terminal2", (temp.join(" ").toUtf8()));
    temp.clear();
    for (int i=0; i < Vol.size(); i++)
        temp.append(QString::number(Vol.at(i)->value()));
    ini->setValue("Voltage", (temp.join(" ").toUtf8()));
    temp.clear();
    for (int i=0; i < Min.size(); i++)
        temp.append(QString::number(Min.at(i)->value()));
    ini->setValue("Min", (temp.join(" ").toUtf8()));
    temp.clear();
    for (int i=0; i < Max.size(); i++)
        temp.append(QString::number(Max.at(i)->value()));
    ini->setValue("Max", (temp.join(" ").toUtf8()));
    temp.clear();
    for (int i=0; i < Time.size(); i++)
        temp.append(QString::number(Time.at(i)->value()));
    ini->setValue("Time", (temp.join(" ").toUtf8()));
    temp.clear();
    for (int i=0; i < Freq.size(); i++)
        temp.append(QString::number(Freq.at(i)->currentIndex()));
    ini->setValue("Freq", (temp.join(" ").toUtf8()));
    temp.clear();
    for (int i=0; i < Arc.size(); i++)
        temp.append(QString::number(Arc.at(i)->currentIndex()));
    ini->setValue("Arc", (temp.join(" ").toUtf8()));
    temp.clear();
    for (int i=0; i < Offset.size(); i++)
        temp.append(Offset.at(i)->text());
    ini->setValue("Offset", (temp.join(" ").toUtf8()));
    qDebug() << QTime::currentTime().toString() << "PageAcw save OK";
}

void PageAcw::ItemClick(int r, int c)
{
    ui->Input->hide();
    switch (c) {
    case 0:
        if (Enable.at(r)->text() != "Y")
            Enable.at(r)->setText("Y");
        else
            Enable.at(r)->setText("N");
        break;
    case 1:
    case 2:
        if (r == 1 || r == 2 || r == 3)
            InitInput(r, c);
    default:
        break;
    }
}

void PageAcw::ReadMessage(quint16 addr, quint16 cmd, QByteArray msg)
{
    if (addr != ADDR && addr != WIN_ID_ACW && addr != CAN_ID_INR)
        return;
    switch (cmd) {
    case CMD_CAN:
        ExcuteCanCmd(msg);
        break;
    case CMD_START:
        for (int row = 0; row < Enable.size(); row++) {
            if (Enable.at(row)->text() == "Y") {
                Mode = ACW_TEST;
                TestRow = row;
                Judge = "OK";
                SendCanCmdConfig(row);
                Delay(5);
                SendCanCmdStart(msg.toInt());
                if (!WaitTimeOut(100)) {
                    Judge = "NG";
                    SendTestItemsAllError();
                    break;
                }
                Delay(100);
                Mode = ACW_FREE;
            }
        }
        Mode = ACW_FREE;
        SendTestJudge();
        break;
    case CMD_STOP:
        SendCanCmdStop();
        break;
    case CMD_INIT:
        InitSettings();
        SendTestItemsAllEmpty();
        break;
    default:
        break;
    }
}

void PageAcw::ExcuteCanCmd(QByteArray msg)
{
    if (Mode == ACW_FREE)
        return;
    TimeOut = 0;
//    emit SendCommand(ADDR, CMD_DEBUG, "acw msg:");
//    emit SendCommand(ADDR, CMD_DEBUG, msg.toHex());
//    emit SendCommand(ADDR, CMD_DEBUG, "\n");
    if (msg.size() >= 4 && (quint8)msg.at(0) == 0x00)
        ReadCanCmdStatus(msg);
    if (msg.size() >= 7 && (quint8)msg.at(0) == 0x01) {
        if (Mode == ACW_OFFSET)
            ReadCanCmdOffset(msg);
        else
            ReadCanCmdResult(msg);
    }
}
/**
  * @brief   耐压状态, ID:0x61;长度:0x02;命令:0x00 status;
  */
void PageAcw::ReadCanCmdStatus(QByteArray msg)
{
    int s = quint8(msg.at(1));
    switch (s) {
    case 0x00:
        break;
    case 0x01:
        return;
    case 0x02:
        SendWarnning("UNIVALID");
        break;
    default:
        SendWarnning("UNKONW_ERROR");
        break;
    }
    if (Mode == ACW_TEST) {
        SendTestItem();
        ClearResults();
    }
    if (Mode == ACW_OFFSET) {
        CalOffset();
        ClearResults();
    }
    Mode = ACW_FREE;
}
/**
  * @brief   耐压结果, ID:0x61;长度:0x07;命令:0x01 volt volt curr curr grade judge;
  * @brief   测试中不合格直接停止测试
  */
void PageAcw::ReadCanCmdResult(QByteArray msg)
{
    double v = quint16(msg.at(1)*256)+quint8(msg.at(2));
    double tt = quint16(msg.at(3)*256)+quint8(msg.at(4));
    tt *= qPow(10, -quint8(msg.at(5)));
    tt -= qMin(tt, Offset.at(TestRow)->value());
    Volt.append(v);
    Curr.append(tt);
    SendTestItemTemp();

    if (quint8(msg.at(6)) != 0x00) {
        Judge = "NG";
        switch (quint8(msg.at(6))) {
        case 1:
            SendTestItem();
            break;
        case 2:
            SendTestItemError("STOP");
            break;
        case 3:
            SendTestItemError("ARC");
            break;
        case 4:
            SendTestItemError(tr("漏电保护"));
            break;
        case 5:
            SendTestItemError(tr("功率保护"));
            break;
        case 6:
            SendTestItemError(tr("DCPT"));
            break;
        case 7:
            SendTestItemError(">20mA");
            break;
        default:
            SendTestItemError(QString("PT+%1").arg(quint8(msg.at(6))));
            break;
        }
        ClearResults();
    }
}

void PageAcw::ReadCanCmdOffset(QByteArray msg)
{
    double v = quint16(msg.at(1)*256)+quint8(msg.at(2));
    double tt = quint16(msg.at(3)*256)+quint8(msg.at(4));
    tt *= qPow(10, -quint8(msg.at(5)));
    Volt.append(v);
    Curr.append(tt);
}

void PageAcw::SendTestItemsAllEmpty()
{
    Items.clear();
    for (int row = 0; row < Enable.size(); row++) {
        QString T1 = Terminal1.at(row)->text();
        QString U1 = Vol.at(qMin(row, Vol.size()))->text();
        QString M1 = Min.at(qMin(row, Min.size()))->text();
        QString M2 = Max.at(qMin(row, Max.size()))->text();
        QString s;
        s.append(tr("交耐"));
        if (EnablePhase())
            s.append(tr("%1@高端:%2,").arg(row+1).arg(T1));
        else
            s.append(tr("@"));
        s.append(tr(" %1V, %2-%3mA@ @ ").arg(U1).arg(M1).arg(M2));
        Items.append(s);
    }
    QStringList n;
    for (int row = 0; row < Enable.size(); row++) {
        if (Enable.at(row)->text() == "Y") {
            n.append(Items.at(row));
        }
    }
    emit SendCommand(ADDR, CMD_INIT_ITEM, n.join("\n").toUtf8());
}

void PageAcw::SendTestItemsAllError()
{
    QStringList s = QString(Items.at(TestRow)).split("@");
    if (s.at(2) == " ")
        s[2] = "---";
    if (s.at(3) == " ")
        s[3] = "NG";
    emit SendCommand(ADDR, CMD_ITEM, s.join("@").toUtf8());
}
void PageAcw::SendTestItemTemp()
{
    if (Volt.size() < 2 || Curr.size() < 2) {
        return;
    }
    QString rrr = QString::number(Curr.at(Curr.size()-2), 'f', 2);
    QString t = QString("%1mA").arg(rrr);
    QStringList s = QString(Items.at(TestRow)).split("@");
    if (s.at(2) == " ")
        s[2] = t;
    emit SendCommand(ADDR, CMD_ITEM_TEMP, s.join("@").toUtf8());
}
/**
  * @brief   耐压判定, 下位机BUG, 需移除最后一帧数据
  */
void PageAcw::SendTestItem()
{
    if (Volt.isEmpty() || Curr.isEmpty()) {
        Judge = "NG";
        SendTestItemsAllError();
        return;
    }
    if (Volt.size() > 1 || Curr.size() > 1) {
        Volt.removeLast();
        Curr.removeLast();
    }
    QStringList s = QString(Items.at(TestRow)).split("@");
    QString rrr = QString::number(Curr.last(), 'f', 2);
    if (s.at(2) == " ")
        s[2] = QString("%1mA").arg(rrr);
    if (s.at(3) == " ")
        s[3] = Judge;
    emit SendCommand(ADDR, CMD_ITEM, s.join("@").toUtf8());
}

void PageAcw::SendTestItemError(QString e)
{
    QStringList s = QString(Items.at(TestRow)).split("@");
    if (s.at(2) == " ")
        s[2] = e;
    if (s.at(3) == " ")
        s[3] = Judge;
    emit SendCommand(ADDR, CMD_ITEM, s.join("@").toUtf8());
}

void PageAcw::SendTestJudge()
{
    QString s = QString(tr("交耐@%1@%2")).arg(CurrentSettings()).arg(Judge);
    emit SendCommand(ADDR, CMD_JUDGE, s.toUtf8());
}
/**
  * @brief   耐压启动, ID:0x23;长度:0x05;命令:0x01 0x05 0x00 工位 0x00
  */
void PageAcw::SendCanCmdStart(quint8 pos)
{
    QByteArray msg;
    QDataStream out(&msg, QIODevice::ReadWrite);
    out.setVersion(QDataStream::Qt_4_8);
    out << quint16(0x23) << quint8(0x05) << quint8(0x01) << quint8(0x05) << quint8(0x00)
        << quint8(pos) << quint8(0x00);
    emit SendCommand(ADDR, CMD_CAN, msg);
}
/**
  * @brief   耐压停止, ID:0x23;长度:0x01;命令:0x02
  */
void PageAcw::SendCanCmdStop()
{
    QByteArray msg;
    QDataStream out(&msg, QIODevice::ReadWrite);
    out.setVersion(QDataStream::Qt_4_8);
    out << quint16(0x23) << quint8(0x01) << quint8(0x02);
    emit SendCommand(ADDR, CMD_CAN, msg);
}
/**
  * @brief   耐压配置1, ID:0x23;长度:0x08;命令:0x03 0x00 0x05 0x00 0x00 0xff 0xff freq
  * @brief   耐压配置2, ID:0x23;长度:0x08;命令:0x04 0x00 volt volt time time min  min
  * @brief   耐压配置3, ID:0x23;长度:0x07;命令:0x05 0x00 max  max  arc  0x03 0x0A
  */
void PageAcw::SendCanCmdConfig(int row)
{
    QByteArray msg;
    QDataStream out(&msg, QIODevice::ReadWrite);
    out.setVersion(QDataStream::Qt_4_8);
    int freq = Freq.at(row)->currentText().toInt();
    int volt = Vol.at(row)->value();
    int time = Time.at(row)->value()*10;
    int min = Min.at(row)->value()*100;
    int max = Max.at(row)->value()*100;
    int arc = Arc.at(row)->currentIndex();
    quint16 h = GetTeminal(row, 1);
    quint16 l = GetTeminal(row, 2);
    qDebug() << QString::number(h, 16) << QString::number(l, 16);
    out << quint16(0x23) << quint8(0x08) << quint8(0x03) << quint8(0x00) << quint8(0x05)
        << quint8(h/256) << quint8(h%256) << quint8(l/256) << quint8(l%256) << quint8(freq);
    out << quint16(0x23) << quint8(0x08) << quint8(0x04) << quint8(0x00) << quint8(volt/256)
        << quint8(volt%256) << quint8(time/256) << quint8(time%256)
        << quint8(min/256) << quint8(min%256);
    out << quint16(0x23) << quint8(0x07) << quint8(0x05) << quint8(0x00)
        << quint8(max/256) << quint8(max%256) << quint8(arc) << quint8(0x03) << quint8(0x0A);
    emit SendCommand(ADDR, CMD_CAN, msg);
}

void PageAcw::ClearResults()
{
    Volt.clear();
    Curr.clear();
}

void PageAcw::CalOffset()
{
    if (Curr.size() < 2) {
        ui->Text->setText("Offset Error");
        return;
    }
    Curr.removeLast();
    Offset.at(TestRow)->setValue(Curr.last());
}

bool PageAcw::WaitTimeOut(quint16 t)
{
    TimeOut = 0;
    while (Mode != ACW_FREE) {
        Delay(10);
        TimeOut++;
        if (TimeOut  >  t)
            return false;
    }
    return true;
}

void PageAcw::Delay(int ms)
{
    QElapsedTimer t;
    t.start();
    while (t.elapsed() < ms)
        QCoreApplication::processEvents();
}

quint16 PageAcw::GetTeminal(int r, int c)
{
    if (r == 0 && c == 1)
        return 0x8000;
    if (r == 0 && c == 2)
        return 0x7fff;
    quint16 t = 0;
    QString s;
    if (c == 1)
        s = Terminal1.at(r)->text();
    if (c == 2)
        s = Terminal2.at(r)->text();
    if (s.contains("1"))
        t += 0x01;
    if (s.contains("2"))
        t += 0x02;
    if (s.contains("3"))
        t += 0x04;
    if (s.contains("4"))
        t += 0x08;
    if (s.contains("5"))
        t += 0x10;
    if (s.contains("6"))
        t += 0x20;
    if (s.contains("7"))
        t += 0x40;
    if (s.contains("8"))
        t += 0x80;
    return t;
}

void PageAcw::InitInput(int r, int c)
{
    QString s;
    QString x;
    QString s1;
    ui->Input->show();
    for (int i=0; i < Check.size(); i++) {
        Check.at(i)->setChecked(false);
        Check.at(i)->hide();
    }
    if (c == 1) {
        s = Terminal1.at(r)->text();
        x = "147";
        s1 = Terminal2.at(r)->text();
        for (int i=0; i < s1.size(); i++)
            x.remove(s1.mid(i, 1));
    }
    if (c == 2) {
        s = Terminal2.at(r)->text();
        x = "12345678";
        s1 = Terminal1.at(r)->text();
        for (int i=0; i < s1.size(); i++)
            x.remove(s1.mid(i, 1));
    }

    for (int i = 0; i < x.size(); i++) {
        Check.at(x.mid(i, 1).toInt() - 1)->show();
    }
    for (int i = 0; i < s.size(); i++) {
        Check.at(s.mid(i, 1).toInt()-1)->setChecked(true);
    }
}

void PageAcw::EnsureInput()
{
    QString s;
    for (int i = 0; i < Check.size(); i++) {
        if (Check.at(i)->isChecked())
            s.append(Check.at(i)->text());
    }
    int r = ui->TabParams->currentRow();
    int c = ui->TabParams->currentColumn();
    if ( r != 0 && (c == 1 || c == 2))
        ui->TabParams->currentItem()->setText(s);
    ui->Input->hide();
}

QString PageAcw::CurrentSettings()
{
    QSettings *ini = new QSettings(INI_PATH, QSettings::IniFormat);
    QString n = ini->value("/GLOBAL/FileInUse", INI_DEFAULT).toString();
    return n.remove(".ini");
}

bool PageAcw::EnablePhase()
{
    QSettings *ini = new QSettings(INI_PATH, QSettings::IniFormat);
    return ini->value("/GLOBAL/EnablePhase", false).toBool();
}

void PageAcw::showEvent(QShowEvent *e)
{
    ui->BtnExitAcw->setFocus();
    InitSettings();
    e->accept();
}

void PageAcw::SendWarnning(QString s)
{
    QVariantHash hash;
    hash.insert("TxAddress", "WinHome");
    hash.insert("TxCommand", "Warnning");
    hash.insert("TxMessage", tr("交耐异常:\n%1").arg(s));
    emit SendVariant(QVariant::fromValue(hash));
}
/*********************************END OF FILE**********************************/

