/*******************************************************************************
 * Copyright [2016]   <青岛艾普智能仪器有限公司>
 * All rights reserved.
 *
 * version:     2.1.0.170427
 * author:      zhaonanlin
 * brief:       电感模块
*******************************************************************************/
#include "PageInd.h"
#include "ui_PageInd.h"

PageInd::PageInd(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::PageInd)
{
    ui->setupUi(this);
    InitWindows();
    InitButtons();
    InitSettings();
    Mode = IND_FREE;
}

PageInd::~PageInd()
{
    delete ui;
}

void PageInd::InitWindows()
{
#if (QT_VERSION <= QT_VERSION_CHECK(5, 0, 0))
    ui->TabParams->horizontalHeader()->setResizeMode(4, QHeaderView::Stretch);
    ui->TabParams->horizontalHeader()->setResizeMode(5, QHeaderView::Stretch);
    ui->TabParams->horizontalHeader()->setResizeMode(6, QHeaderView::Stretch);
    ui->TabParams->horizontalHeader()->setResizeMode(7, QHeaderView::Stretch);
    ui->TabParams->horizontalHeader()->setResizeMode(8, QHeaderView::Stretch);
    ui->TabParams->verticalHeader()->setResizeMode(QHeaderView::Stretch);
#else
    ui->TabParams->horizontalHeader()->setSectionResizeMode(4, QHeaderView::Stretch);
    ui->TabParams->horizontalHeader()->setSectionResizeMode(5, QHeaderView::Stretch);
    ui->TabParams->horizontalHeader()->setSectionResizeMode(6, QHeaderView::Stretch);
    ui->TabParams->horizontalHeader()->setSectionResizeMode(7, QHeaderView::Stretch);
    ui->TabParams->horizontalHeader()->setSectionResizeMode(8, QHeaderView::Stretch);
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

        Unit.append(new QComboBox(this));
        ui->TabParams->setCellWidget(row, 3, Unit.at(row));
        QStringList t2;
        t2  << "uH" << "mH";
        Unit.at(row)->setView(new QListView(this));
        Unit.at(row)->addItems(t2);

        Min.append(new QDoubleSpinBox(this));
        ui->TabParams->setCellWidget(row, 4, Min.at(row));
        Min.at(row)->setMaximum(2000);
        Min.at(row)->setAlignment(Qt::AlignHCenter);
        Min.at(row)->setButtonSymbols(QDoubleSpinBox::NoButtons);

        Max.append(new QDoubleSpinBox(this));
        ui->TabParams->setCellWidget(row, 5, Max.at(row));
        Max.at(row)->setMaximum(2000);
        Max.at(row)->setAlignment(Qt::AlignHCenter);
        Max.at(row)->setButtonSymbols(QDoubleSpinBox::NoButtons);

        QMin.append(new QDoubleSpinBox(this));
        ui->TabParams->setCellWidget(row, 6, QMin.at(row));
        QMin.at(row)->setMaximum(100);
        QMin.at(row)->setAlignment(Qt::AlignHCenter);
        QMin.at(row)->setButtonSymbols(QDoubleSpinBox::NoButtons);

        QMax.append(new QDoubleSpinBox(this));
        ui->TabParams->setCellWidget(row, 7, QMax.at(row));
        QMax.at(row)->setMaximum(100);
        QMax.at(row)->setAlignment(Qt::AlignHCenter);
        QMax.at(row)->setButtonSymbols(QDoubleSpinBox::NoButtons);

        Std.append(new QDoubleSpinBox(this));
        ui->TabParams->setCellWidget(row, 8, Std.at(row));
        Std.at(row)->setMaximum(2000);
        Std.at(row)->setAlignment(Qt::AlignHCenter);
        Std.at(row)->setButtonSymbols(QDoubleSpinBox::NoButtons);

        Offset.append(new QDoubleSpinBox(this));
        ui->TabParams->setCellWidget(row, 9, Offset.at(row));
        Offset.at(row)->setMaximum(100);
        Offset.at(row)->setAlignment(Qt::AlignHCenter);
        Offset.at(row)->setButtonSymbols(QDoubleSpinBox::NoButtons);
    }
}

void PageInd::InitButtons()
{
    QButtonGroup *btnGroup = new QButtonGroup;
    btnGroup->addButton(ui->BtnSDLRAuto, Qt::Key_0);
    btnGroup->addButton(ui->BtnOffset, Qt::Key_1);
    btnGroup->addButton(ui->BtnSDLRExit, Qt::Key_2);
    connect(btnGroup, SIGNAL(buttonClicked(int)), this, SLOT(ReadButtons(int)));
}

void PageInd::ReadButtons(int id)
{
    switch (id) {
    case Qt::Key_0:
        CalculateAuto();
        break;
    case Qt::Key_1:
        Mode = IND_OFFSET;
        SendCanCmdConfig();
        SendCanCmdStart(WIN_ID_OUT13);
        WaitTimeOut(100);
        Mode = IND_FREE;
        break;
    case Qt::Key_2:
        SaveSettings();
        emit SendCommand(ADDR, CMD_JUMP, NULL);
        break;
    default:
        break;
    }
}

void PageInd::InitSettings()
{
    //当前使用的测试项目
    QString t = QString("./config/%1.ini").arg(CurrentSettings());
    set = new QSettings(t, QSettings::IniFormat);
    set->setIniCodec("GB18030");
    set->beginGroup("PageInd");

    QStringList temp = (set->value("Other", "1 0 20 20 0 0").toString()).split(" ");
    if (temp.size() >= 6) {
        ui->BoxTime->setValue(temp.at(0).toDouble());
        ui->BoxUnbalance->setValue(temp.at(1).toDouble());
        ui->BoxMin->setValue(temp.at(2).toDouble());
        ui->BoxMax->setValue(temp.at(3).toDouble());
        ui->BoxFreq->setCurrentIndex(temp.at(4).toInt());
        ui->BoxMode->setCurrentIndex(temp.at(5).toInt());
    }
    //可用
    temp = (QString(set->value("Enable", "Y Y Y N N N N N").toByteArray())).split(" ");
    for (int row=0; row < qMin(temp.size(), MAX_ROW); row++)
        Enable.at(row)->setText(temp.at(row));
    //端一
    temp = (set->value("Terminal1", "1 2 1 4 5 6 7 8").toString()).split(" ");
    for (int row=0; row < qMin(temp.size(), MAX_ROW); row++)
        Terminal1.at(row)->setText(temp.at(row));
    //端二
    temp = (set->value("Terminal2", "2 3 3 5 6 7 8 1").toString()).split(" ");
    for (int row=0; row < qMin(temp.size(), MAX_ROW); row++)
        Terminal2.at(row)->setText(temp.at(row));
    //单位
    temp = (QString(set->value("Unit", "1 1 1 1 1 1 1 1").toByteArray())).split(" ");
    for (int row=0; row < qMin(temp.size(), MAX_ROW); row++)
        Unit.at(row)->setCurrentIndex(temp.at(row).toInt());
    //最小值
    temp = (set->value("Min", "0.0 0.0 0.0 0.0 0.0 0.0 0.0 0.0").toString()).split(" ");
    for (int row=0; row < qMin(temp.size(), MAX_ROW); row++)
        Min.at(row)->setValue(temp.at(row).toDouble());
    //最大值
    temp = (set->value("Max", "200 200 200 200 200 200 200 200").toString()).split(" ");
    for (int row=0; row < qMin(temp.size(), MAX_ROW); row++)
        Max.at(row)->setValue(temp.at(row).toDouble());
    //最小值
    temp = (set->value("QMin", "0.0 0.0 0.0 0.0 0.0 0.0 0.0 0.0").toString()).split(" ");
    for (int row=0; row < qMin(temp.size(), MAX_ROW); row++)
        QMin.at(row)->setValue(temp.at(row).toDouble());
    //最大值
    temp = (set->value("QMax", "200 200 200 200 200 200 200 200").toString()).split(" ");
    for (int row=0; row < qMin(temp.size(), MAX_ROW); row++)
        QMax.at(row)->setValue(temp.at(row).toDouble());
    //标准值
    temp = (set->value("Std", "100 100 100 100 100 100 100 100").toString()).split(" ");
    for (int row=0; row < qMin(temp.size(), MAX_ROW); row++)
        Std.at(row)->setValue(temp.at(row).toDouble());
    //补偿
    temp = (set->value("Offset", "0.0 0.0 0.0 0.0 0.0 0.0 0.0 0.0").toString()).split(" ");
    for (int row=0; row < qMin(temp.size(), MAX_ROW); row++)
        Offset.at(row)->setValue(temp.at(row).toDouble());
    qDebug() << QTime::currentTime().toString() << "PageInd read OK";
}

void PageInd::SaveSettings()
{
    QStringList temp;
    temp.append(QString::number(ui->BoxTime->value()));
    temp.append(QString::number(ui->BoxUnbalance->value()));
    temp.append(QString::number(ui->BoxMin->value()));
    temp.append(QString::number(ui->BoxMax->value()));
    temp.append(QString::number(ui->BoxFreq->currentIndex()));
    temp.append(QString::number(ui->BoxMode->currentIndex()));
    set->setValue("Other", (temp.join(" ").toUtf8()));

    temp.clear();
    for (int i=0; i < Enable.size(); i++)
        temp.append(Enable.at(i)->text());
    set->setValue("Enable", (temp.join(" ").toUtf8()));
    temp.clear();
    for (int i=0; i < Terminal1.size(); i++)
        temp.append(Terminal1.at(i)->text());
    set->setValue("Terminal1", (temp.join(" ").toUtf8()));
    temp.clear();
    for (int i=0; i < Terminal2.size(); i++)
        temp.append(Terminal2.at(i)->text());
    set->setValue("Terminal2", (temp.join(" ").toUtf8()));
    temp.clear();
    for (int i=0; i < Unit.size(); i++)
        temp.append(QString::number(Unit.at(i)->currentIndex()));
    set->setValue("Unit", (temp.join(" ").toUtf8()));
    temp.clear();
    for (int i=0; i < Min.size(); i++)
        temp.append(QString::number(Min.at(i)->value()));
    set->setValue("Min", (temp.join(" ").toUtf8()));
    temp.clear();
    for (int i=0; i < Max.size(); i++)
        temp.append(QString::number(Max.at(i)->value()));
    set->setValue("Max", (temp.join(" ").toUtf8()));
    temp.clear();
    for (int i=0; i < QMin.size(); i++)
        temp.append(QString::number(QMin.at(i)->value()));
    set->setValue("QMin", (temp.join(" ").toUtf8()));
    temp.clear();
    for (int i=0; i < QMax.size(); i++)
        temp.append(QString::number(QMax.at(i)->value()));
    set->setValue("QMax", (temp.join(" ").toUtf8()));
    temp.clear();
    for (int i=0; i < Std.size(); i++)
        temp.append(QString::number(Std.at(i)->value()));
    set->setValue("Std", (temp.join(" ").toUtf8()));
    temp.clear();
    for (int i=0; i < Offset.size(); i++)
        temp.append(Offset.at(i)->text());
    set->setValue("Offset", (temp.join(" ").toUtf8()));
    qDebug() << QTime::currentTime().toString() << "PageInd save OK";
}

void PageInd::CalculateAuto()
{
    for (int i=0; i < ui->TabParams->rowCount(); i++) {
        double std = Std.at(i)->value();
        double min = std*(100-ui->BoxMin->value())/100;
        double max = std*(100+ui->BoxMax->value())/100;
        Min.at(i)->setValue(min);
        Max.at(i)->setValue(max);
    }
}

void PageInd::ItemClick(int r,  int c)
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

void PageInd::ItemChange(QString msg)
{
    int t = ui->TabParams->currentColumn();
    if (t == 1 || t == 2)
        ui->TabParams->currentItem()->setText(msg);
}

void PageInd::ReadMessage(quint16 addr,  quint16 cmd,  QByteArray msg)
{
    if (addr != ADDR && addr != WIN_ID_IND && addr != CAN_ID_IND)
        return;
    switch (cmd) {
    case CMD_CAN:
        ExcuteCanCmd(msg);
        break;
    case CMD_CHECK:
        Mode = IND_INIT;
        SendCanCmdStatus();
        if (!WaitTimeOut(30))
            SendWarnning("超时");
        Mode = IND_FREE;
        break;
    case CMD_START:
        Mode = IND_TEST;
        Judge = "OK";
        SendCanCmdStart(msg.toInt());
        if (!WaitTimeOut(ui->BoxTime->value()*100+100)) {
            Judge = "NG";
            SendTestItemsAllError();
        }
        SendTestJudge();
        Mode = IND_FREE;
        break;
    case CMD_STOP:
        SendCanCmdStop();
        Mode = IND_FREE;
        break;
    case CMD_INIT:
        InitSettings();
        SendTestItemsAllEmpty();
        SendCanCmdConfig();
        break;
    default:
        break;
    }
}

void PageInd::ExcuteCanCmd(QByteArray msg)
{
    if (Mode == IND_FREE)
        return;
    TimeOut = 0;
    if (msg.size() == 8 && (quint8)msg.at(0) == 0x00) {
        ReadCanCmdStatus(msg);
    }
    if (msg.size() == 8 && (quint8)msg.at(0) == 0x01) {
        if (Mode == IND_OFFSET)
            ReadCanCmdOffset(msg);
        else
            ReadCanCmdResult(msg);
    }
}

void PageInd::ReadCanCmdStatus(QByteArray msg)
{
    int s = quint8(msg.at(1));
    switch (s) {
    case 0x00:
        break;
    case 0x01:
        return;
    case 0x02:
        break;
    case 0x03:
        break;
    case 0x04:
        SendWarnning("CALIZERO");
        break;
    case 0x05:
        SendWarnning("OFFSETTE");
        break;
    case 0x06:
        SendWarnning("GAINTEST");
        break;
    case 0x07:
        SendWarnning("ERRORGAIN");
        break;
    case 0x08:
        SendWarnning("ERROROFFSET");
        break;
    case 0x09:
        SendWarnning("ERRORREALY");
        break;
    default:
        SendWarnning("UNKONW_ERROR");
        break;
    }
    Mode = IND_FREE;
}

void PageInd::ReadCanCmdResult(QByteArray msg)
{
    quint8 number = quint8(msg.at(1));
    if (number >0 )
        number--;
    if (quint8(msg.at(3) == 0x00)) {
        Result1.dat[0] = quint8(msg.at(4));
        Result1.dat[1] = quint8(msg.at(5));
        Result1.dat[2] = quint8(msg.at(6));
        Result1.dat[3] = quint8(msg.at(7));
    }
    if (quint8(msg.at(3) == 0x01)) {
        Result2.dat[0] = quint8(msg.at(4));
        Result2.dat[1] = quint8(msg.at(5));
        Result2.dat[2] = quint8(msg.at(6));
        Result2.dat[3] = quint8(msg.at(7));
        QString t = "---";
        if (Result1.Result <= 1000)
            t = QString::number(Result1.Result, 'f', 1) + "uH";
        else if (Result1.Result <= 10000)
            t = QString::number(Result1.Result/1000, 'f', 3) + "mH";
        else if (Result1.Result <= 100000)
            t = QString::number(Result1.Result/1000, 'f', 2) + "mH";
        else if (Result1.Result <= 1000000)
            t = QString::number(Result1.Result/1000, 'f', 1) + "mH";
        else if (Result1.Result <= 10000000)
            t = QString::number(Result1.Result/1000, 'f', 0) + "mH";

        Results.append(Result1.Result);

        t +=", " + QString::number(Result2.Result, 'f', 2);
        double max = Max.at(number)->value();
        double min = Min.at(number)->value();
        double qmax = QMax.at(number)->value();
        double qmin = QMin.at(number)->value();
        if (Unit.at(number)->currentText() == "mH") {
            max *= 1000;
            min *= 1000;
        }
        QString judge = "OK";
        if (Result1.Result < min || Result1.Result > max
                || Result2.Result < qmin || Result2.Result > qmax) {
            Judge = "NG";
            judge = "NG";
        }
        QStringList s = QString(Items.at(number)).split("@");
        if (s.at(2) == " ")
            s[2] = t;
        if (s.at(3) == " ")
            s[3] = judge;
        emit SendCommand(ADDR, CMD_ITEM, s.join("@").toUtf8());

        if ((ui->BoxUnbalance->value() != 0) && (Results.size() == 3)) {
            double sum = 0;
            double avr = 0;
            QString u;
            for (int i=0; i < Results.size(); i++) {
                sum += Results.at(i);
            }
            avr = sum/Results.size();
            judge = "OK";
            for (int i=0; i < Results.size(); i++) {
                double un = fabs(Results.at(i)-avr)*100/avr;
                qDebug()  <<  un  <<  avr;
                u.append(QString::number(un, 'f', 1));
                u.append("% ");
                if (un >= ui->BoxUnbalance->value()) {
                    Judge = "NG";
                    judge = "NG";
                }
            }
            QStringList s = QString(Items.last()).split("@");
            if (s.at(2) == " ")
                s[2] = u;
            if (s.at(3) == " ")
                s[3] = judge;
            emit SendCommand(ADDR, CMD_ITEM, s.join("@").toUtf8());
        }
    }
}

void PageInd::ReadCanCmdOffset(QByteArray msg)
{
    quint8 number = quint8(msg.at(1));
    if (number >0 )
        number--;
    Resultunion  Result;
    if (quint8(msg.at(3) == 0x00)) {
        Result.dat[0] = quint8(msg.at(4));
        Result.dat[1] = quint8(msg.at(5));
        Result.dat[2] = quint8(msg.at(6));
        Result.dat[3] = quint8(msg.at(7));
        if (Unit.at(number)->currentText() == "mH")
            Offset.at(number)->setValue(Result.Result/1000);
        else
            Offset.at(number)->setValue(Result.Result);
    }
}

void PageInd::SendTestItemsAllEmpty()
{
    Items.clear();
    Results.clear();
    QStringList n;
    for (int row = 0; row < Enable.size(); row++) {
        QStringList s;
        QString T1 = Terminal1.at(qMin(row, Terminal1.size()))->text();
        QString T2 = Terminal2.at(qMin(row, Terminal2.size()))->text();
        QString M1 = Min.at(qMin(row, Min.size()))->text();
        QString M2 = Max.at(qMin(row, Max.size()))->text();
        QString Q1 = QMin.at(qMin(row, QMin.size()))->text();
        QString Q2 = QMax.at(qMin(row, QMax.size()))->text();
        s.append(QString(tr("电感%1-%2")).arg(T1).arg(T2));
        s.append(QString("%1~%2, %3~%4").arg(M1).arg(M2).arg(Q1).arg(Q2));
        s.append(" ");
        s.append(" ");
        Items.append(s.join("@"));
    }
    for (int row = 0; row < Enable.size(); row++) {
        if (Enable.at(row)->text() == "Y") {
            n.append(Items.at(row));
        }
    }
    if (ui->BoxUnbalance->value() != 0 && n.size() >= 3) {
        QStringList s;
        s.append("电感平衡");
        s.append(QString("%1%").arg(ui->BoxUnbalance->value()));
        s.append(" ");
        s.append(" ");
        Items.append(s.join("@"));
        n.append(Items.last());
    }
    emit SendCommand(ADDR, CMD_INIT_ITEM, n.join("\n").toUtf8());
}

void PageInd::SendTestItemsAllError()
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

void PageInd::SendTestJudge()
{
    QString s = QString(tr("电感@%1@%2")).arg(CurrentSettings()).arg(Judge);
    emit SendCommand(ADDR, CMD_JUDGE, s.toUtf8());
}

void PageInd::SendCanCmdStatus()
{
    QByteArray msg;
    QDataStream out(&msg,  QIODevice::ReadWrite);
    out.setVersion(QDataStream::Qt_4_8);
    out << quint16(0x26) << quint8(0x01) << quint8(0x00);
    emit SendCommand(ADDR, CMD_CAN, msg);
}

void PageInd::SendCanCmdStart(quint8 pos)
{
    QByteArray msg;
    QDataStream out(&msg,  QIODevice::ReadWrite);
    out.setVersion(QDataStream::Qt_4_8);
    quint16 tt = 0;
    for (int row=0; row < Enable.size(); row++) {
        if (Enable.at(row)->text() == "Y")
            tt += 0x0001 << row;
    }
    out << quint16(0x26) << quint8(0x06) << quint8(0x01) << quint8(0x00) << quint8(0x00)
        << quint8(pos) << quint8(tt/256) << quint8(tt%256);
    emit SendCommand(ADDR, CMD_CAN, msg);
}

void PageInd::SendCanCmdStop()
{
    QByteArray msg;
    QDataStream out(&msg,  QIODevice::ReadWrite);
    out.setVersion(QDataStream::Qt_4_8);
    out << quint16(0x26) << quint8(0x01) << quint8(0x02);
    emit SendCommand(ADDR, CMD_CAN, msg);
}

void PageInd::SendCanCmdConfig()
{
    QByteArray msg;
    QDataStream out(&msg,  QIODevice::ReadWrite);
    out.setVersion(QDataStream::Qt_4_8);
    for (int i=0; i < Enable.size(); i++) {
        if (Enable.at(i)->text() == "Y") {
            out << quint16(0x26) << quint8(0x07) << quint8(0x03) << quint8(i)
                << quint8(Terminal1.at(i)->text().toInt())
                << quint8(Terminal2.at(i)->text().toInt())
                << quint8(ui->BoxTime->value())
                << quint8(CalculateGear(i)) << quint8(CalculateMode(i));
        }
    }
    emit SendCommand(ADDR, CMD_CAN, msg);
}

int PageInd::CalculateGear(int row)
{
    int t = 0;
    double s = Std.at(row)->text().toDouble();
    if (Unit.at(row)->currentText() == "mH") {
        switch (ui->BoxFreq->currentIndex()) {
        case 0:
            if (2*3.14*s*0.1/1000 < 1)
                t = 0x09;
            else if (2*3.14*s*0.1/1000 < 10)
                t = 0x29;
            else
                t = 0x39;
            break;
        case 1:
            if (2*3.14*s*0.12/1000 < 1)
                t = 0x0A;
            else if (2*3.14*s*0.12/1000 < 10)
                t = 0x2A;
            else
                t = 0x3A;
            break;
        case 2:
            if (2*3.14*s*1/1000 < 1)
                t = 0x0A;
            else if (2*3.14*s*1/1000 < 10)
                t = 0x2A;
            else
                t = 0x3A;
            break;
        case 3:
            if (2*3.14*s*10/1000 < 1)
                t = 0x0C;
            else if (2*3.14*s*10/1000 < 10)
                t = 0x2C;
            else
                t = 0x3C;
            break;
        default:
            break;
        }
    } else {
        switch (ui->BoxFreq->currentIndex()) {
        case 0:
            t = 0x09;
            break;
        case 1:
            t = 0x0A;
            break;
        case 2:
            t = 0x0B;
            break;
        case 3:
            t = 0x0C;
            break;
        default:
            break;
        }
    }
    return t;
}

int PageInd::CalculateMode(int row)
{
    int t = 0;
    int s = 0;
    if (ui->BoxMode->currentIndex() == 0)
        s = 0x00;
    else
        s = 0x40;

    if (Unit.at(row)->currentText() == "uH" && Max.at(row)->text().toInt() < 500)
        t = s | 0x0F;
    else
        t = s | 0x09;
    return t;
}

bool PageInd::WaitTimeOut(quint16 t)
{
    TimeOut = 0;
    while (Mode != IND_FREE) {
        Delay(10);
        TimeOut++;
        if (TimeOut > t)
            return false;
    }
    return true;
}

void PageInd::Delay(int ms)
{
    QElapsedTimer t;
    t.start();
    while (t.elapsed() < ms)
        QCoreApplication::processEvents();
}

QString PageInd::CurrentSettings()
{
    QSettings *ini = new QSettings(INI_PATH, QSettings::IniFormat);
    QString n = ini->value("/GLOBAL/FileInUse", INI_DEFAULT).toString();
    return n.remove(".ini");
}

void PageInd::showEvent(QShowEvent *e)
{
    InitSettings();
    e->accept();
}

void PageInd::SendWarnning(QString s)
{
    QVariantHash hash;
    hash.insert("TxAddress", "WinHome");
    hash.insert("TxCommand", "Warnning");
    hash.insert("TxMessage", tr("电感异常:\n%1").arg(s));
    emit SendVariant(QVariant::fromValue(hash));
}
