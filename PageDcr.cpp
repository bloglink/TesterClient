/*******************************************************************************
 * Copyright [2016]   <青岛艾普智能仪器有限公司>
 * All rights reserved.
 *
 * version:     2.1.0.170427
 * author:      zhaonanlin
 * brief:       电阻模块
*******************************************************************************/
#include "PageDcr.h"
#include "ui_PageDcr.h"

PageDcr::PageDcr(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::PageDcr)
{
    ui->setupUi(this);
    InitWindows();
    InitButtons();
    InitSettings();
    Mode = DCR_FREE;
}

PageDcr::~PageDcr()
{
    delete ui;
}

void PageDcr::InitWindows()
{
#if (QT_VERSION <= QT_VERSION_CHECK(5, 0, 0))
    ui->TabParams->horizontalHeader()->setResizeMode(3, QHeaderView::Stretch);
    ui->TabParams->horizontalHeader()->setResizeMode(4, QHeaderView::Stretch);
    ui->TabParams->horizontalHeader()->setResizeMode(5, QHeaderView::Stretch);
    ui->TabParams->horizontalHeader()->setResizeMode(6, QHeaderView::Stretch);
    ui->TabParams->horizontalHeader()->setResizeMode(7, QHeaderView::Stretch);
    ui->TabParams->horizontalHeader()->setResizeMode(8, QHeaderView::Stretch);
    ui->TabParams->horizontalHeader()->setResizeMode(9, QHeaderView::Stretch);
    ui->TabParams->verticalHeader()->setResizeMode(QHeaderView::Stretch);
#else
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
    input = new PageNum(this);
    QStringList t;
    t  << "1" << "2" << "3" << "4" << "5" << "6" << "7" << "8" << "9" << "10" << "11" << "12";
    input->InitButtons(t);
    connect(input, SIGNAL(ItemChange(QString)), this, SLOT(ItemChange(QString)));
    input->hide();

    ui->TabParams->setRowCount(MAX_ROW);
    for (int row=0; row < MAX_ROW; row++) {
        Enable.append(new QTableWidgetItem);
        ui->TabParams->setItem(row, 0, Enable.at(row));
        Enable.at(row)->setFlags(Qt::ItemIsSelectable|Qt::ItemIsEnabled);
        Enable.at(row)->setTextAlignment(Qt::AlignCenter);

        Terminal1.append(new QTableWidgetItem);
        ui->TabParams->setItem(row, 1, Terminal1.at(row));
        Terminal1.at(row)->setFlags(Qt::ItemIsSelectable|Qt::ItemIsEnabled);
        Terminal1.at(row)->setTextAlignment(Qt::AlignCenter);

        Terminal2.append(new QTableWidgetItem);
        ui->TabParams->setItem(row, 2, Terminal2.at(row));
        Terminal2.at(row)->setFlags(Qt::ItemIsSelectable|Qt::ItemIsEnabled);
        Terminal2.at(row)->setTextAlignment(Qt::AlignCenter);

        Metal.append(new QComboBox(this));
        ui->TabParams->setCellWidget(row, 3, Metal.at(row));
        QStringList t1;
        t1  << tr("铜") << tr("铝") << tr("铜铝");
        Metal.at(row)->setView(new QListView(this));
        Metal.at(row)->addItems(t1);

        Unit.append(new QComboBox(this));
        ui->TabParams->setCellWidget(row, 4, Unit.at(row));
        QStringList t2;
        t2  << "mohm" << "ohm" << "kohm";
        Unit.at(row)->setView(new QListView(this));
        Unit.at(row)->addItems(t2);

        Min.append(new QDoubleSpinBox(this));
        ui->TabParams->setCellWidget(row, 5, Min.at(row));
        Min.at(row)->setMaximum(9999);
        Min.at(row)->setAlignment(Qt::AlignHCenter);
        Min.at(row)->setButtonSymbols(QDoubleSpinBox::NoButtons);

        Max.append(new QDoubleSpinBox(this));
        ui->TabParams->setCellWidget(row, 6, Max.at(row));
        Max.at(row)->setMaximum(9999);
        Max.at(row)->setAlignment(Qt::AlignHCenter);
        Max.at(row)->setButtonSymbols(QDoubleSpinBox::NoButtons);

        Std.append(new QDoubleSpinBox(this));
        ui->TabParams->setCellWidget(row, 7, Std.at(row));
        Std.at(row)->setMaximum(9999);
        Std.at(row)->setAlignment(Qt::AlignHCenter);
        Std.at(row)->setButtonSymbols(QDoubleSpinBox::NoButtons);

        Offset.append(new QDoubleSpinBox(this));
        ui->TabParams->setCellWidget(row, 8, Offset.at(row));
        Offset.at(row)->setMaximum(9999);
        Offset.at(row)->setAlignment(Qt::AlignHCenter);
        Offset.at(row)->setButtonSymbols(QDoubleSpinBox::NoButtons);

        OffsetR.append(new QDoubleSpinBox(this));
        ui->TabParams->setCellWidget(row, 9, OffsetR.at(row));
        OffsetR.at(row)->setMaximum(9999);
        OffsetR.at(row)->setAlignment(Qt::AlignHCenter);
        OffsetR.at(row)->setButtonSymbols(QDoubleSpinBox::NoButtons);
    }
    connect(Metal.at(0), SIGNAL(currentIndexChanged(int)), this, SLOT(AutoChangeMetal(int)));
    connect(Unit.at(0), SIGNAL(currentIndexChanged(int)), this, SLOT(AutoChangeUnit(int)));
    connect(Min.at(0), SIGNAL(valueChanged(double)), this, SLOT(AutoChangeMin(double)));
    connect(Max.at(0), SIGNAL(valueChanged(double)), this, SLOT(AutoChangeMax(double)));
    //    connect(Std.at(0), SIGNAL(valueChanged(double)), this, SLOT(AutoChangeStd(double)));
}

void PageDcr::InitButtons()
{
    QButtonGroup *btnGroup = new QButtonGroup;
    btnGroup->addButton(ui->BtnSDLRAuto, Qt::Key_0);
    btnGroup->addButton(ui->BtnOffset, Qt::Key_1);
    btnGroup->addButton(ui->BtnSDLRExit, Qt::Key_2);
    btnGroup->addButton(ui->BtnOffsetR, Qt::Key_3);
    connect(btnGroup, SIGNAL(buttonClicked(int)), this, SLOT(ReadButtons(int)));
}

void PageDcr::ReadButtons(int id)
{
    switch (id) {
    case Qt::Key_0:
        AutoCalculateMinAndMax();
        break;
    case Qt::Key_1:
        stat = WIN_ID_OUT13;
        SendCanCmdConfig();
        Mode = DCR_OFFSET;
        SendCanCmdStart(stat);
        WaitTimeOut(100);
        Mode = DCR_FREE;
        break;
    case Qt::Key_2:
        if (CheckSetting())
            SaveSettings();
        emit SendCommand(ADDR, CMD_JUMP, NULL);
        break;
    case Qt::Key_3:
        stat = WIN_ID_OUT14;
        SendCanCmdConfig();
        Mode = DCR_OFFSET;
        SendCanCmdStart(stat);
        WaitTimeOut(100);
        Mode = DCR_FREE;
        break;
    default:
        break;
    }
}

void PageDcr::InitSettings()
{
    //当前使用的测试项目
    QString t = QString("./config/%1.ini").arg(CurrentSettings());
    QSettings *ini = new QSettings(t, QSettings::IniFormat);
    ini->setIniCodec("GB18030");
    ini->beginGroup("SetDcr");

    QStringList temp = (ini->value("Other", "20 0 0.5 10 10 0").toString()).split(" ");
    if (temp.size() >= 6) {
        ui->BoxStd->setValue(temp.at(0).toDouble());
        ui->BoxOffset->setValue(temp.at(1).toDouble());
        ui->BoxTime->setValue(temp.at(2).toDouble());
        ui->BoxMin->setValue(temp.at(3).toDouble());
        ui->BoxMax->setValue(temp.at(4).toDouble());
        ui->BoxUnbalance->setValue(temp.at(5).toDouble());
    }
    //可用
    temp = (QString(ini->value("Enable", "Y Y Y N N N N N").toByteArray())).split(" ");
    for (int row=0; row < qMin(temp.size(), MAX_ROW); row++)
        Enable.at(row)->setText(temp.at(row));
    //端一
    temp = (ini->value("Terminal1", "1 2 1 4 5 6 7 8").toString()).split(" ");
    for (int row=0; row < qMin(temp.size(), MAX_ROW); row++)
        Terminal1.at(row)->setText(temp.at(row));
    //端二
    temp = (ini->value("Terminal2", "2 3 3 5 6 7 8 1").toString()).split(" ");
    for (int row=0; row < qMin(temp.size(), MAX_ROW); row++)
        Terminal2.at(row)->setText(temp.at(row));
    //材料
    temp = (QString(ini->value("Metal", "0 0 0 0 0 0 0 0").toByteArray())).split(" ");
    for (int row=0; row < qMin(temp.size(), MAX_ROW); row++)
        Metal.at(row)->setCurrentIndex(temp.at(row).toInt());
    //单位
    temp = (QString(ini->value("Unit", "1 1 1 1 1 1 1 1").toByteArray())).split(" ");
    for (int row=0; row < qMin(temp.size(), MAX_ROW); row++)
        Unit.at(row)->setCurrentIndex(temp.at(row).toInt());
    //最小值
    temp = (ini->value("Min", "0.0 0.0 0.0 0.0 0.0 0.0 0.0 0.0").toString()).split(" ");
    for (int row=0; row < qMin(temp.size(), MAX_ROW); row++)
        Min.at(row)->setValue(temp.at(row).toDouble());

    //最大值
    temp = (ini->value("Max", "200 200 200 200 200 200 200 200").toString()).split(" ");
    for (int row=0; row < qMin(temp.size(), MAX_ROW); row++)
        Max.at(row)->setValue(temp.at(row).toDouble());
    //标准值
    temp = (ini->value("Std", "100 100 100 100 100 100 100 100").toString()).split(" ");
    for (int row=0; row < qMin(temp.size(), MAX_ROW); row++)
        Std.at(row)->setValue(temp.at(row).toDouble());
    //补偿
    temp = (ini->value("Offset", "0.0 0.0 0.0 0.0 0.0 0.0 0.0 0.0").toString()).split(" ");
    for (int row=0; row < qMin(temp.size(), MAX_ROW); row++)
        Offset.at(row)->setValue(temp.at(row).toDouble());
    //补偿
    temp = (ini->value("OffsetR", "0.0 0.0 0.0 0.0 0.0 0.0 0.0 0.0").toString()).split(" ");
    for (int row=0; row < qMin(temp.size(), MAX_ROW); row++)
        OffsetR.at(row)->setValue(temp.at(row).toDouble());
    qDebug() << QTime::currentTime().toString() << "PageDcr read OK";
}

void PageDcr::SaveSettings()
{
    //当前使用的测试项目
    QString t = QString("./config/%1.ini").arg(CurrentSettings());
    QSettings *ini = new QSettings(t, QSettings::IniFormat);
    ini->setIniCodec("GB18030");
    ini->beginGroup("SetDcr");

    QStringList temp;
    temp.append(QString::number(ui->BoxStd->value()));
    temp.append(QString::number(ui->BoxOffset->value()));
    temp.append(QString::number(ui->BoxTime->value()));
    temp.append(QString::number(ui->BoxMin->value()));
    temp.append(QString::number(ui->BoxMax->value()));
    temp.append(QString::number(ui->BoxUnbalance->value()));
    ini->setValue("Other", (temp.join(" ").toUtf8()));

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
    for (int i=0; i < Metal.size(); i++)
        temp.append(QString::number(Metal.at(i)->currentIndex()));
    ini->setValue("Metal", (temp.join(" ").toUtf8()));
    temp.clear();
    for (int i=0; i < Unit.size(); i++)
        temp.append(QString::number(Unit.at(i)->currentIndex()));
    ini->setValue("Unit", (temp.join(" ").toUtf8()));
    temp.clear();
    for (int i=0; i < Min.size(); i++)
        temp.append(QString::number(Min.at(i)->value()));
    ini->setValue("Min", (temp.join(" ").toUtf8()));
    temp.clear();
    for (int i=0; i < Max.size(); i++)
        temp.append(QString::number(Max.at(i)->value()));
    ini->setValue("Max", (temp.join(" ").toUtf8()));
    temp.clear();
    for (int i=0; i < Std.size(); i++)
        temp.append(QString::number(Std.at(i)->value()));
    ini->setValue("Std", (temp.join(" ").toUtf8()));
    temp.clear();
    for (int i=0; i < Offset.size(); i++)
        temp.append(Offset.at(i)->text());
    ini->setValue("Offset", (temp.join(" ").toUtf8()));
    temp.clear();
    for (int i=0; i < OffsetR.size(); i++)
        temp.append(OffsetR.at(i)->text());
    ini->setValue("OffsetR", (temp.join(" ").toUtf8()));
    qDebug() << QTime::currentTime().toString() << "PageDcr save OK";
}

void PageDcr::AutoCalculateMinAndMax()
{
    for (int i=0; i < ui->TabParams->rowCount(); i++) {
        double std = Std.at(i)->value();
        double min = std*(100-ui->BoxMin->value())/100;
        double max = std*(100+ui->BoxMax->value())/100;
        Min.at(i)->setValue(min);
        Max.at(i)->setValue(max);
    }
}

bool PageDcr::CheckSetting()
{
    for (int i=0; i < qMin(Terminal1.size(), Terminal2.size()); i++) {
        int t1 = Terminal1.at(i)->text().toInt();
        int t2 = Terminal2.at(i)->text().toInt();
        if (abs(t1-t2)%3 == 0) {
            SendWarnning(QString("行%1:\n端子差不能为3, 保存失败").arg(i+1));
            return false;
        }
    }
    for (int i=0; i < qMin(Max.size(), Min.size()); i++) {
        if (Min.at(i)->value() > Max.at(i)->value()) {
            SendWarnning(QString("行%1:\n上限大于下限, 保存失败").arg(i+1));
            return false;
        }
    }
    return true;
}

void PageDcr::ItemClick(int r,  int c)
{
    switch (c) {
    case 0:
        if (Enable.at(r)->text() != "Y")
            Enable.at(r)->setText("Y");
        else
            Enable.at(r)->setText("N");
        break;
    case 1:
    case 2:
        input->show();
        break;
    default:
        break;
    }
}

void PageDcr::ItemChange(QString msg)
{
    int t = ui->TabParams->currentColumn();
    if (t == 1 || t == 2)
        ui->TabParams->currentItem()->setText(msg);
}

void PageDcr::ReadMessage(quint16 addr,  quint16 cmd,  QByteArray msg)
{
    if (addr != ADDR && addr != WIN_ID_DCR && addr != CAN_ID_DCR && addr != CAN_ID_PWR)
        return;
    switch (cmd) {
    case CMD_CAN:
        if (addr == CAN_ID_DCR)
            ExcuteCanCmd(msg);
        if (addr == CAN_ID_PWR)
            ExcuteCanCmdPwr(msg);
        break;
    case CMD_CHECK:
        Mode = DCR_INIT;
        SendCanCmdStatus();
        if (!WaitTimeOut(30))
            SendWarnning("超时");
        Mode = DCR_FREE;
        break;
    case CMD_START:
        Mode = DCR_TEST;
        Judge = "OK";
        stat = msg.toInt();
        if (IsPowerOn()) {
            pwr.clear();
            SendCanCmdPwr(stat);
            WaitTimeOut(60);
        }
        if (Mode == DCR_FREE)
            break;
        SendCanCmdStart(stat);
        if (!WaitTimeOut(ui->BoxTime->value()*100+100)) {
            Judge = "NG";
            SendTestItemsAllError();
        }
        SendTestJudge();
        Mode = DCR_FREE;
        break;
    case CMD_STOP:
        SendAlarm(QByteArray(1, 0x00));
        break;
    case CMD_INIT:
        InitSettings();
        SendTestItemsAllEmpty();
        SendCanCmdConfig();
        break;
    case CMD_ALARM:
        SendAlarm(msg);
        break;
    default:
        break;
    }
}

void PageDcr::ExcuteCanCmd(QByteArray msg)
{
    if (Mode == DCR_FREE)
        return;
    TimeOut = 0;

    if (msg.size() >= 4 && (quint8)msg.at(0) == 0x00) {
        ReadCanCmdStatus(msg);
    }
    if (msg.size() >= 7 && (quint8)msg.at(0) == 0x01) {
        if (Mode == DCR_OFFSET)
            ReadOffset(msg);
        else
            ReadCanCmdResult(msg);
    }
}

void PageDcr::ExcuteCanCmdPwr(QByteArray msg)
{
    if (Mode == DCR_FREE)
        return;
    if (msg.size() == 8 && (quint8)msg.at(0) == 0x01) {
        pwr.append(quint16(msg.at(1)*256)+quint8(msg.at(2)));
        if (pwr.size() > 4 && pwr.last() > 50) {
            emit SendCommand(ADDR, CMD_STOP, "DCR PWR");
            SendWarnning(tr("请等待电机转动停止再测试"));
            Mode = DCR_FREE;
        }
    }
}

void PageDcr::ReadCanCmdStatus(QByteArray msg)
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
    case 0x03:
        SendWarnning("FLASH_ERROR");
        break;
    case 0x04:
        SendWarnning("ZERO_ERROR");
        break;
    case 0x05:
        SendWarnning("AMP11_ERROR");
        break;
    case 0x06:
        SendWarnning("AMP121_ERROR");
        break;
    case 0x07:
        SendWarnning("REFER1_ERROR");
        break;
    case 0x08:
        SendWarnning("REFER2_ERROR");
        break;
    case 0x09:
        SendWarnning("CUR_ERROR");
        break;
    case 0x0A:
        SendWarnning("CHAN_ERROR");
        break;
    default:
        SendWarnning("UNKONW_ERROR");
        break;
    }
    double offset = ui->BoxOffset->value();
    double temp = (quint16(msg.at(2)*256)+quint8(msg.at(3)))/10-50+offset;
    QString t = QString(tr("温度:%1°C")).arg(temp);
    emit SendCommand(ADDR, CMD_TEMP, t.toUtf8());
    Mode = DCR_FREE;
}

void PageDcr::ReadCanCmdResult(QByteArray msg)
{
    quint8 number = quint8(msg.at(1));
    quint8 grade = quint8(msg.at(2));
    double temp = (quint16)(msg.at(3)*256)+(quint8)msg.at(4);
    double tt = (quint16)(msg.at(5)*256)+quint8(msg.at(6));

    if (ui->BoxOffsetEnable->isChecked()) {
        double offset = CalculateOffset(tt, number);
        temp *= offset;
    }
    QString t;
    QString JudgeItem = "OK";
    double offset = 0;
    if (stat == WIN_ID_OUT13)
        offset = Offset.at(number)->value();
    if (stat == WIN_ID_OUT14)
        offset = OffsetR.at(number)->value();
    if (Unit.at(number)->currentText() == "mohm")
        offset /= 1000;
    if (Unit.at(number)->currentText() == "kohm")
        offset *= 1000;

    temp *= qPow(10, (grade-6));
    temp -= qMin(temp, offset);
    if (grade == 1 || grade == 2)
        t = QString("%1mohm").arg(temp*1000, 0, 'r', (3-grade%3));
    if (grade == 3 || grade == 4 || grade == 5)
        t = QString("%1ohm").arg(temp, 0, 'r', (3-grade%3));
    if (grade == 6 || grade == 7)
        t = QString("%1kohm").arg(temp/1000, 0, 'r', (3-grade%3));

    double gg = 2 * qPow(10, (grade-2)) * 1.1;
    if (temp > gg)
        t = QString(">%1ohm").arg(gg);

    Results.append(temp);
    if (Unit.at(number)->currentText() == "mohm")
        temp *= 1000;
    if (Unit.at(number)->currentText() == "kohm")
        temp /= 1000;
    if (temp<Min.at(number)->value() || temp>Max.at(number)->value()) {
        Judge = "NG";
        JudgeItem = "NG";
    }

    QStringList s = QString(Items.at(number)).split("@");
    if (s.at(2) == " ")
        s[2] = t;
    if (s.at(3) == " ")
        s[3] = JudgeItem;
    emit SendCommand(ADDR, CMD_ITEM, s.join("@").toUtf8());

    if (temp < (Max.at(number)->value()/10)) {
        emit SendCommand(ADDR, CMD_STOP, "DCR LOW");
        SendWarnning(tr("电阻值小于10%，请检查线路连接"));
    }

    CalculateBalance();
}

void PageDcr::ReadOffset(QByteArray msg)
{
    quint8 number = quint8(msg.at(1));
    quint8 grade = quint8(msg.at(2));
    double temp = (quint16)(msg.at(3)*256)+(quint8)msg.at(4);

    if (Unit.at(number)->currentText() == "mohm")
        temp *= qPow(10, (grade-3));
    if (Unit.at(number)->currentText() == "ohm")
        temp *= qPow(10, (grade-6));
    if (Unit.at(number)->currentText() == "kohm")
        temp *= qPow(10, (grade-9));

    if (temp*20 > Max.at(number)->value())
        temp = 0;
    if (stat == WIN_ID_OUT13)
        Offset.at(number)->setValue(temp);
    if (stat == WIN_ID_OUT14)
        OffsetR.at(number)->setValue(temp);
}

void PageDcr::SendTestItemsAllEmpty()
{
    Items.clear();
    Results.clear();
    QStringList n;
    for (int row = 0; row < Enable.size(); row++) {
        QString T1 = Terminal1.at(qMin(row, Terminal1.size()))->text();
        QString T2 = Terminal2.at(qMin(row, Terminal2.size()))->text();
        QString U1 = Unit.at(qMin(row, Unit.size()))->currentText();
        QString M1 = Min.at(qMin(row, Min.size()))->text();
        QString M2 = Max.at(qMin(row, Max.size()))->text();
        QString s = QString(tr("电阻%1-%2@%3~%4%5@ @ ")).arg(T1).arg(T2).arg(M1).arg(M2).arg(U1);
        Items.append(s);
    }
    for (int row = 0; row < Enable.size(); row++) {
        if (Enable.at(row)->text() == "Y") {
            n.append(Items.at(row));
        }
    }
    if (ui->BoxUnbalance->value() != 0 && Items.size() >= 3) {
        QString s = QString(tr("电阻平衡@%1%@ @ ")).arg(ui->BoxUnbalance->value());
        Items.append(s);
        n.append(Items.last());
    }
    emit SendCommand(ADDR, CMD_INIT_ITEM, n.join("\n").toUtf8());
}

void PageDcr::SendTestItemsAllError()
{
    for (int row = 0; row < Enable.size(); row++) {
        if (Enable.at(row)->text() == "Y") {
            QStringList s = QString(Items.at(row)).split("@");
            if (s.at(2) == " ")
                s[2] = "---";
            if (s.at(3) == " ")
                s[3] = "NG";
            emit SendCommand(ADDR, CMD_ITEM, s.join("@").toUtf8());
        }
    }
    if (ui->BoxUnbalance->value() != 0) {
        QStringList s = QString(Items.last()).split("@");
        if (s.at(2) == " ")
            s[2] = "---";
        if (s.at(3) == " ")
            s[3] = "NG";
        emit SendCommand(ADDR, CMD_ITEM, s.join("@").toUtf8());
    }
}

void PageDcr::SendTestJudge()
{
    QString s = QString(tr("电阻@%1@%2")).arg(CurrentSettings()).arg(Judge);
    emit SendCommand(ADDR, CMD_JUDGE, s.toUtf8());
}

void PageDcr::SendCanCmdStatus()
{
    QByteArray msg;
    QDataStream out(&msg,  QIODevice::ReadWrite);
    out.setVersion(QDataStream::Qt_4_8);
    out << quint16(0x22) << quint8(0x01) << quint8(0x00);
    emit SendCommand(ADDR, CMD_CAN, msg);
}

void PageDcr::SendCanCmdStart(quint8 pos)
{
    QByteArray msg;
    QDataStream out(&msg,  QIODevice::ReadWrite);
    out.setVersion(QDataStream::Qt_4_8);
    quint16 tt = 0;
    for (int row=0; row < Enable.size(); row++) {
        if (Enable.at(row)->text() == "Y")
            tt += 0x0001 << row;
    }
    out << quint16(0x22) << quint8(0x06) << quint8(0x01) << quint8(0x01) << quint8(0x00)
        << quint8(pos) << quint8(tt/256) << quint8(tt%256);
    emit SendCommand(ADDR, CMD_CAN, msg);
}

void PageDcr::SendCanCmdStop()
{
    QByteArray msg;
    QDataStream out(&msg,  QIODevice::ReadWrite);
    out.setVersion(QDataStream::Qt_4_8);
    out << quint16(0x22) << quint8(0x01) << quint8(0x02);
    emit SendCommand(ADDR, CMD_CAN, msg);
}

void PageDcr::SendCanCmdConfig()
{
    QByteArray msg;
    QDataStream out(&msg,  QIODevice::ReadWrite);
    out.setVersion(QDataStream::Qt_4_8);
    for (int row=0; row < Enable.size(); row++) {
        if (Enable.at(row)->text() == "Y") {
            out << quint16(0x22) << quint8(0x06) << quint8(0x03) << quint8(row)
                << quint8(Terminal1.at(row)->text().toInt())
                << quint8(Terminal2.at(row)->text().toInt())
                << quint8(CalculateGear(row))
                << quint8(ui->BoxTime->value()*10);
        }
    }
    emit SendCommand(ADDR, CMD_CAN, msg);
}

void PageDcr::SendAlarm(QByteArray addr)
{
    quint8 t = 0x00;
    if (addr.at(0) & 0x01)
        t += 0x08;
    if (addr.at(0) & 0x02)
        t += 0x04;
    if (addr.at(0) & 0x04)
        t += 0x02;
    if (addr.at(0) & 0x08)
        t += 0x01;
    QByteArray msg;
    QDataStream out(&msg,  QIODevice::ReadWrite);
    out.setVersion(QDataStream::Qt_4_8);
    out << quint16(0x22) << quint8(0x02) << quint8(0x09) << quint8(t);
    emit SendCommand(ADDR, CMD_CAN, msg);
}

void PageDcr::SendCanCmdPwr(quint8 s)
{
    QByteArray msg;
    QDataStream out(&msg,  QIODevice::ReadWrite);
    out.setVersion(QDataStream::Qt_4_8);
    quint8 p = CurrentPorwer().toInt() << 4;
    quint8 g = 0x01;
    if (s == WIN_ID_OUT14)
        g  <<= 4;
    out << quint16(0x27) << quint8(0x08) << quint8(0x01) << quint8(g)
        << quint8(0x00) << quint8(0x05) << quint8(p) << quint8(0x00)
        << quint8(0x00) << quint8(0x00);
    emit SendCommand(ADDR, CMD_CAN, msg);
}

void PageDcr::SendWarnning(QString s)
{
    QVariantHash hash;
    hash.insert("TxAddress", "WinHome");
    hash.insert("TxCommand", "Warnning");
    hash.insert("TxMessage", tr("电阻异常:\n%1").arg(s));
    emit SendVariant(QVariant::fromValue(hash));
}

double PageDcr::CalculateOffset(double t,  quint8 num)
{
    double temp;
    double res = 0;
    switch (Metal.at(num)->currentIndex()) {
    case 0:
        res = ResCu;
        break;
    case 1:
        res = ResCu_Al;
        break;
    case 2:
        res = ResAl;
        break;
    default:
        break;
    }
    temp = 1+res*(ui->BoxStd->value()-(t/10-50)-ui->BoxOffset->value());
    return temp;
}

void PageDcr::CalculateBalance()
{
    //计算不平衡度
    if ((ui->BoxUnbalance->value() != 0) && (Results.size() == 3)) {
        QString JudgeItem = "OK";
        double sum = 0;
        double avr = 0;
        QString u;
        for (int i=0; i < Results.size(); i++) {
            sum += Results.at(i);
        }
        avr = sum/Results.size();
        for (int i=0; i < Results.size(); i++) {
            double un;
            if (avr == 0)
                un = 0;
            else
                un = fabs(Results.at(i)-avr)*100/avr;
            u.append(QString::number(un, 'f', 1));
            u.append("% ");
            if (un >= ui->BoxUnbalance->value()) {
                JudgeItem = "NG";
                Judge = "NG";
            }
        }
        QStringList s = QString(Items.last()).split("@");
        if (s.at(2) == " ")
            s[2] = u;
        if (s.at(3) == " ")
            s[3] = JudgeItem;
        emit SendCommand(ADDR, CMD_ITEM, s.join("@").toUtf8());
    }
}

int PageDcr::CalculateGear(int row)
{
    if (row >= Max.size())
        return 7;
    if (row >= Unit.size())
        return 7;
    double r = Max.at(row)->value();
    if (Unit.at(row)->currentText() == "mohm")
        r /= 1000;
    if (Unit.at(row)->currentText() == "kohm")
        r *= 1000;
    if (r <= 0.2)
        return 1;
    if (r <= 2)
        return 2;
    if (r <= 20)
        return 3;
    if (r <= 200)
        return 4;
    if (r <= 2000)
        return 5;
    if (r <= 20000)
        return 6;
    else
        return 7;
}

bool PageDcr::WaitTimeOut(quint16 t)
{
    TimeOut = 0;
    while (Mode != DCR_FREE) {
        Delay(10);
        TimeOut++;
        if (TimeOut > t)
            return false;
    }
    return true;
}

void PageDcr::Delay(int ms)
{
    QElapsedTimer t;
    t.start();
    while (t.elapsed() < ms)
        QCoreApplication::processEvents();
}

QString PageDcr::CurrentSettings()
{
    QSettings *ini = new QSettings(INI_PATH, QSettings::IniFormat);
    QString n = ini->value("/GLOBAL/FileInUse", INI_DEFAULT).toString();
    return n.remove(".ini");
}

QString PageDcr::CurrentPorwer()
{
    QSettings *ini = new QSettings(INI_PATH, QSettings::IniFormat);
    QString n = ini->value("/GLOBAL/PowerSupply", "0").toString();
    return n;
}

bool PageDcr::IsPowerOn()
{
    QString s = QString("./config/%1.ini").arg(CurrentSettings());
    QSettings *ini = new QSettings(s,  QSettings::IniFormat);
    QStringList n = ini->value("/GLOBAL/ProjToTest",  "1").toString().split(" ");
    if (n.contains(QString::number(WIN_ID_PWR)))
        return true;
    if (n.contains(QString::number(WIN_ID_LVS)))
        return true;
    if (n.contains(QString::number(WIN_ID_LCK)))
        return true;
    return false;
}

void PageDcr::AutoChangeMetal(int index)
{
    for (int i=1; i < Metal.size(); i++)
        Metal.at(i)->setCurrentIndex(index);
}

void PageDcr::AutoChangeUnit(int index)
{
    for (int i=1; i < Unit.size(); i++)
        Unit.at(i)->setCurrentIndex(index);
}

void PageDcr::AutoChangeMin(double x)
{
    for (int i=1; i < Min.size(); i++)
        Min.at(i)->setValue(x);
}

void PageDcr::AutoChangeMax(double x)
{
    for (int i=1; i < Max.size(); i++)
        Max.at(i)->setValue(x);
}

void PageDcr::AutoChangeStd(double x)
{
    for (int i=1; i < Std.size(); i++)
        Std.at(i)->setValue(x);
}

void PageDcr::showEvent(QShowEvent *e)
{
    InitSettings();
    CheckSetting();
    e->accept();
}
/*********************************END OF FILE**********************************/
