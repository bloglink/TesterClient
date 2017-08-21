/*******************************************************************************
 * Copyright [2016]   <青岛艾普智能仪器有限公司>
 * All rights reserved.
 *
 * version:     2.1.0.170427
 * author:      zhaonanlin
 * brief:       堵转模块
*******************************************************************************/
#include "PageLck.h"
#include "ui_PageLck.h"

PageLck::PageLck(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::PageLck)
{
    ui->setupUi(this);
    InitWindows();
    InitButtons();
    InitSettings();
    Mode = LCK_FREE;
}

PageLck::~PageLck()
{
    delete ui;
}

void PageLck::InitWindows()
{
}

void PageLck::InitButtons()
{
    QButtonGroup *btnGroup = new QButtonGroup;
    btnGroup->addButton(ui->BtnSample, Qt::Key_0);
    btnGroup->addButton(ui->BtnExit, Qt::Key_2);
    connect(btnGroup, SIGNAL(buttonClicked(int)), this, SLOT(BtnJudge(int)));
}

void PageLck::BtnJudge(int id)
{
    switch (id) {
    case Qt::Key_0:
        ui->BtnExit->setEnabled(false);
        ui->BtnSample->setEnabled(false);
        for (int i=0; i < ui->BoxSampleTimes->value(); i++) {
            ui->BtnSample->setText(QString("采样中%1").arg(i+1));
            Mode = LCK_SAMPLE;
            SendCanCmdStart(WIN_ID_OUT13);
            if (!WaitTimeOut(500)) {
                ClearResults();
                ui->BtnSample->setText(QString("左工位采样"));
                ui->BtnExit->setEnabled(true);
                ui->BtnSample->setEnabled(true);
                Mode = LCK_FREE;
                return;
            }
            Mode = LCK_FREE;
            Delay(ui->BoxSampleDelay->value()*1000);
        }
        CalculateSample();
        ClearResults();
        ui->BtnSample->setText(QString("左工位采样"));
        ui->BtnExit->setEnabled(true);
        ui->BtnSample->setEnabled(true);
        break;
    case Qt::Key_2:
        SaveSettings();
        emit SendCommand(ADDR, CMD_JUMP, NULL);
        break;
    default:
        break;
    }
}

void PageLck::InitSettings()
{
    QSettings *global = new QSettings(INI_PATH, QSettings::IniFormat);
    global->setIniCodec("GB18030");
    global->beginGroup("GLOBAL");
    PowerSupply = global->value("PowerSupply", "0").toInt();
    FileInUse = global->value("FileInUse", INI_DEFAULT).toString();
    FileInUse.remove(".ini");

    //当前使用的测试项目
    QString t = QString("./config/%1.ini").arg(FileInUse);
    set = new QSettings(t, QSettings::IniFormat);
    set->setIniCodec("GB18030");
    set->beginGroup("PageLck");

    ui->BoxVolt->setValue(set->value("Voltage", "220").toDouble());
    ui->BoxTime->setValue(set->value("TimeUse", "1").toDouble());
    ui->BoxGrade->setValue(set->value("Grade", "1").toDouble());
    ui->BoxVoltMax->setValue(set->value("VoltMax", "255").toDouble());
    ui->BoxVoltMin->setValue(set->value("VoltMin", "1").toDouble());
    ui->BoxCurrMax->setValue(set->value("CurrentMax", "5.000").toDouble());
    ui->BoxCurrMin->setValue(set->value("CurrentMin", "0.001").toDouble());
    ui->BoxPowerMax->setValue(set->value("PowerMax", "1275").toDouble());
    ui->BoxPowerMin->setValue(set->value("PowerMin", "0.1").toDouble());
    ui->BoxFreq->setValue(set->value("Freq", "1").toDouble());
    ui->BoxCurr->setValue(set->value("Current", "0.001").toDouble());
    ui->BoxPower->setValue(set->value("Power", "1").toDouble());
    qDebug() << QTime::currentTime().toString() << "PageLck read OK";
}

void PageLck::SaveSettings()
{
    set->setValue("Voltage", QString::number(ui->BoxVolt->value()));
    set->setValue("TimeUse", QString::number(ui->BoxTime->value()));
    set->setValue("Grade", QString::number(ui->BoxGrade->value()));
    set->setValue("VoltMax", QString::number(ui->BoxVoltMax->value()));
    set->setValue("VoltMin", QString::number(ui->BoxVoltMin->value()));
    set->setValue("CurrentMax", QString::number(ui->BoxCurrMax->value()));
    set->setValue("CurrentMin", QString::number(ui->BoxCurrMin->value()));
    set->setValue("PowerMax", QString::number(ui->BoxPowerMax->value()));
    set->setValue("PowerMin", QString::number(ui->BoxPowerMin->value()));
    set->setValue("Freq", QString::number(ui->BoxFreq->value()));
    set->setValue("Current", QString::number(ui->BoxCurr->value()));
    set->setValue("Power", QString::number(ui->BoxPower->value()));
    qDebug() << QTime::currentTime().toString() << "PageLck save OK";
}

void PageLck::ReadMessage(quint16 addr,  quint16 cmd,  QByteArray msg)
{
    if (addr != ADDR && addr != WIN_ID_LCK && addr != CAN_ID_PWR)
        return;
    switch (cmd) {
    case CMD_CAN:
        ExcuteCanCmd(msg);
        break;
    case CMD_START:
        Mode = LCK_TEST;
        Judge = "OK";
        SendCanCmdStart(msg.toInt());
        if (!WaitTimeOut(500)) {
            Judge = "NG";
            SendTestItemsAllError();
            break;
        }
        SendTestJudge();
        Mode = LCK_FREE;
        break;
    case CMD_INIT:
        InitSettings();
        InitTestItems();
        break;
    default:
        break;
    }
}

void PageLck::ExcuteCanCmd(QByteArray msg)
{
    if (Mode == LCK_FREE)
        return;
    TimeOut = 0;
    //    emit SendCommand(ADDR, CMD_DEBUG, "lck msg:");
    //    emit SendCommand(ADDR, CMD_DEBUG, msg.toHex());
    //    emit SendCommand(ADDR, CMD_DEBUG, "\n");
    if (msg.size() >= 4 && (quint8)msg.at(0) == 0x00)
        ReadCanCmdStatus(msg);
    if (msg.size() >= 8 && (quint8)msg.at(0) == 0x01)
        ReadCanCmdResult(msg);
}

void PageLck::InitTestItems()
{
    Items.clear();
    QStringList s;
    QString U1 = QString::number(ui->BoxVoltMin->value());
    QString U2 = QString::number(ui->BoxVoltMax->value());
    QString C1 = QString::number(ui->BoxCurrMin->value());
    QString C2 = QString::number(ui->BoxCurrMax->value());
    QString P1 = QString::number(ui->BoxPowerMin->value());
    QString P2 = QString::number(ui->BoxPowerMax->value());
    s.append(QString(tr("堵转")));
    s.append(QString("%1~%2V, %3~%4A, %5~%6W").arg(U1).arg(U2).arg(C1).arg(C2).arg(P1).arg(P2));
    s.append(" ");
    s.append(" ");
    Items.append(s.join("@"));
    emit SendCommand(ADDR, CMD_INIT_ITEM, Items.join("\n").toUtf8());
}

void PageLck::SendCanCmdStart(quint8 s)
{
    QByteArray msg;
    QDataStream out(&msg,  QIODevice::ReadWrite);
    out.setVersion(QDataStream::Qt_4_8);
    quint16 t = ui->BoxTime->value()*100;
    quint16 v = ui->BoxVolt->value();
    quint8 p = PowerSupply << 4;
    quint8 g = 0x01;
    if (ui->BoxFreq->value() == 60)
        p += 0x02;
    if (s == WIN_ID_OUT14)
        g  <<= 4;
    out << quint16(0x27) << quint8(0x05) << quint8(0x03) << quint8(g)
        << quint8(t%256) << quint8(p+v/256) << quint8(v%256);
    emit SendCommand(ADDR, CMD_CAN, msg);
}

void PageLck::SendTestItemsAllError()
{
    for (int i=0; i < Items.size(); i++) {
        QStringList s = QString(Items.at(i)).split("@");
        if (s.at(2) == " ")
            s[2] = "---";
        if (s.at(3) == " ")
            s[3] = "NG";
        emit SendCommand(ADDR, CMD_ITEM, s.join("@").toUtf8());
    }
}

void PageLck::SendCanCmdStop()
{
    QByteArray msg;
    QDataStream out(&msg,  QIODevice::ReadWrite);
    out.setVersion(QDataStream::Qt_4_8);
    out << quint16(0x27) << quint8(0x01) << quint8(0x02);
    emit SendCommand(ADDR, CMD_CAN, msg);
}

void PageLck::SendItemJudge()
{
    int num = ui->BoxFreq->value();
    if (Volt.size() < num)
        return;
    if (Curr.size() < num)
        return;
    if (Power.size() < num)
        return;
    double vv = Volt.at(num)/10;
    double rr = Curr.at(num)/1000;
    double pp = Power.at(num)/10;
    if (abs(Power.at(qMax(num, 1)-1)/10-ui->BoxPower->value()) < (abs(pp-ui->BoxPower->value()))) {
        pp = Power.at(qMax(num, 1)-1)/10;
        rr = Curr.at(qMax(num, 1)-1)/1000;
        vv = Volt.at(qMax(num, 1)-1)/10;
    }

    QString t = QString("%1V, %2A, %3W").arg(vv).arg(rr).arg(pp);

    if (rr > ui->BoxCurrMax->value() || rr < ui->BoxCurrMin->value() )
        Judge = "NG";
    if (pp > ui->BoxPowerMax->value() || pp < ui->BoxPowerMin->value() )
        Judge = "NG";
    QStringList s = QString(Items.at(0)).split("@");
    if (s.at(2) == " ")
        s[2] = t;
    if (s.at(3) == " ")
        s[3] = Judge;
    emit SendCommand(ADDR, CMD_ITEM, s.join("@").toUtf8());
    Mode = LCK_FREE;
}

void PageLck::SendTestJudge()
{
    QStringList s;
    s.append("堵转");
    s.append(FileInUse);
    s.append(Judge);
    emit SendCommand(ADDR, CMD_JUDGE, s.join("@").toUtf8());
}

void PageLck::CalculateSample()
{
    int SampleTimes = ui->BoxSampleTimes->value();
    int Num = Power.size()/SampleTimes;

    int Power1 = 0;
    int Variance = 0;
    QList<double> Variances;
    for (int k=0; k < Num; k++) {
        Power1 = 0;
        Variance = 0;
        for (int i=0; i < SampleTimes; i++)
            Power1 += Power.at(i*Num+k);
        Power1 = Power1/SampleTimes;   // 得到平均数
        for (int i=0; i < SampleTimes; i++) // 求取方差
            Variance += ((Power1-Power.at(i*Num+k)))*((Power1-Power.at(i*Num+k)));
        Variance = Variance / SampleTimes;
        Variances.append(Variance);
    }
    Variances.removeAt(Variances.size()-1);
    Variances.removeAt(1);
    Variances.removeAt(0);
    double Trough = Variances.at(0);
    double Trough_Point = 2;

    double pwr_stable_average = 0; // 功率平均数
    for (int i=0; i < SampleTimes; i++) {
        pwr_stable_average += Power.at(i*Num+Num-1);
    }
    pwr_stable_average = pwr_stable_average/SampleTimes;

    for (int i=0; i < Variances.size()-1; i++) {  // 寻找测试点
        if ((Trough > Variances.at(i+1)) && (Trough > 0) && (Trough > 1.5*pwr_stable_average)) {
            Trough = Variances.at(i+1);
            Trough_Point = i+3;
        } else if (Trough < Variances.at(i+1)) {
            break;
        } else if (Trough == 0) {
            break;
        }
    }
    ui->BoxFreq->setValue(Trough_Point+1);

    double Current1 = 0;
    for (int i=0; i < SampleTimes; i++)
        Current1 += Curr.at(i*Num+Trough_Point);

    Current1 = Current1/SampleTimes;
    ui->BoxCurr->setValue(Current1/1000);

    double pwr_average = 0;
    for (int i=0; i < SampleTimes; i++) {
        pwr_average += Power.at(i*Num+Trough_Point);
    }
    pwr_average = pwr_average/SampleTimes;
    ui->BoxPower->setValue(pwr_average/10);
}

void PageLck::ReadCanCmdStatus(QByteArray msg)
{
    int s = quint8(msg.at(1));
    switch (s) {
    case 0x00:
//        emit SendCommand(ADDR, CMD_DEBUG, QString("lck ok, Mode=%1\n").arg(Mode).toUtf8());
        break;
    case 0x01:
        return;
    case 0x02:
        SendWarnning("FLASH_ERROR");
        break;
    case 0x03:
        SendWarnning("HV_ERROR");
        break;
    case 0x04:
        SendWarnning("WAVE_ERROR");
        break;
    default:
        SendWarnning("UNKONW_ERROR");
        break;
    }
    if (Mode == LCK_TEST) {
        SendItemJudge();
        ClearResults();
    }
    if (Mode == LCK_SAMPLE)
        Mode = LCK_FREE;
}

void PageLck::ReadCanCmdResult(QByteArray msg)
{
    double v = quint16(msg.at(1)*256)+quint8(msg.at(2));
    double c = quint16(msg.at(3)*256)+quint8(msg.at(4));
    double p = quint16(msg.at(5)*256)+quint8(msg.at(6));
    if (c == 0)
        return;
    Volt.append(v);
    Curr.append(c);
    Power.append(p);
}

void PageLck::ClearResults()
{
    Volt.clear();
    Curr.clear();
    Power.clear();
}

QString PageLck::CurrentPorwer()
{
    QSettings *ini = new QSettings(INI_PATH, QSettings::IniFormat);
    QString n = ini->value("/GLOBAL/PowerSupply", "0").toString();
    return n;
}

bool PageLck::WaitTimeOut(quint16 t)
{
    TimeOut = 0;
    while (Mode != LCK_FREE) {
        Delay(10);
        TimeOut++;
        if (TimeOut > t)
            return false;
    }
    return true;
}

void PageLck::Delay(int ms)
{
    QElapsedTimer t;
    t.start();
    while (t.elapsed() < ms)
        QCoreApplication::processEvents();
}

void PageLck::showEvent(QShowEvent *e)
{
    ui->BtnExit->setFocus();
    InitSettings();
    e->accept();
}

void PageLck::SendWarnning(QString s)
{
    QVariantHash hash;
    hash.insert("TxAddress", "WinHome");
    hash.insert("TxCommand", "Warnning");
    hash.insert("TxMessage", tr("堵转异常:\n%1").arg(s));
    emit SendVariant(QVariant::fromValue(hash));
}
/*********************************END OF FILE**********************************/
