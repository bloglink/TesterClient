/*******************************************************************************
 * Copyright [2016]   <青岛艾普智能仪器有限公司>
 * All rights reserved.
 *
 * version:     2.1.0.170427
 * author:      zhaonanlin
 * brief:       反嵌模块
*******************************************************************************/
#include "PageMag.h"
#include "ui_PageMag.h"

PageMag::PageMag(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::PageMag)
{
    ui->setupUi(this);
    InitWindows();
    InitButtons();
    InitSettings();
    MagMode = MAG_FREE;
}

PageMag::~PageMag()
{
    delete ui;
}

void PageMag::InitWindows()
{
#if (QT_VERSION <= QT_VERSION_CHECK(5, 0, 0))
    ui->TabParams->horizontalHeader()->setResizeMode(0, QHeaderView::Stretch);
    ui->TabParams->horizontalHeader()->setResizeMode(1, QHeaderView::Stretch);
    ui->TabParams->horizontalHeader()->setResizeMode(2, QHeaderView::Stretch);
    ui->TabParams->horizontalHeader()->setResizeMode(3, QHeaderView::Stretch);
    ui->TabParams->setColumnWidth(4, 400);
    ui->TabParams->verticalHeader()->setResizeMode(QHeaderView::Stretch);
#else
    ui->TabParams->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Stretch);
    ui->TabParams->horizontalHeader()->setSectionResizeMode(1, QHeaderView::Stretch);
    ui->TabParams->horizontalHeader()->setSectionResizeMode(2, QHeaderView::Stretch);
    ui->TabParams->horizontalHeader()->setSectionResizeMode(3, QHeaderView::Stretch);
    ui->TabParams->setColumnWidth(4, 400);
    ui->TabParams->verticalHeader()->setSectionResizeMode(QHeaderView::Stretch);
#endif
    ui->BoxDir->setView(new QListView(this));
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

        Max.append(new QDoubleSpinBox(this));
        ui->TabParams->setCellWidget(row, 3, Max.at(row));
        Max.at(row)->setMaximum(100);
        Max.at(row)->setAlignment(Qt::AlignHCenter);
        Max.at(row)->setButtonSymbols(QDoubleSpinBox::NoButtons);

        WaveMag.append(new Waveform(this));
        ui->TabParams->setCellWidget(row, 4, WaveMag.at(row));

        AreaL.append(0);
        FreqL.append(0);
        AreaR.append(0);
        FreqR.append(0);
    }
}

void PageMag::InitButtons()
{
    QButtonGroup *btnGroup = new QButtonGroup;
    btnGroup->addButton(ui->BtnSampleMag, Qt::Key_0);
    btnGroup->addButton(ui->BtnExit, Qt::Key_1);
    btnGroup->addButton(ui->BtnExitMag, Qt::Key_2);
    connect(btnGroup, SIGNAL(buttonClicked(int)), this, SLOT(ReadButtons(int)));
}

void PageMag::ReadButtons(int id)
{
    switch (id) {
    case Qt::Key_0:
        MagMode = MAG_SAMPLE;
        if (ui->BoxStation->currentIndex() == 0) {
            SendCanCmdConfig(WIN_ID_OUT13);
            SendCanCmdSample(WIN_ID_OUT13);
        }
        if (ui->BoxStation->currentIndex() == 1) {
            SendCanCmdConfig(WIN_ID_OUT14);
            SendCanCmdSample(WIN_ID_OUT14);
        }
        break;
    case Qt::Key_1:
        emit SendCommand(ADDR, CMD_JUMP, NULL);
        break;
    case Qt::Key_2:
        SaveSettings();
        emit SendCommand(ADDR, CMD_JUMP, NULL);
        break;
    default:
        break;
    }
}

void PageMag::InitSettings()
{
    //当前使用的测试项目
    QString t = QString("./config/%1.ini").arg(CurrentSettings());
    set = new QSettings(t, QSettings::IniFormat);
    set->setIniCodec("GB18030");
    set->beginGroup("SetMag");

    QStringList temp = (set->value("Other", "0 1 2").toString()).split(" ");
    if (temp.size() >= 3) {
        ui->BoxDir->setCurrentIndex(temp.at(0).toInt());
        ui->BoxMain->setValue(temp.at(1).toInt());
        ui->BoxAuxiliary->setValue(temp.at(2).toInt());
    }
    ui->BoxDirOnly->setChecked(set->value("DirOnly").toBool());
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
    //最大值
    temp = (set->value("Max", "10 10 10 10 10 10 10 10").toString()).split(" ");
    for (int row=0; row < qMin(temp.size(), MAX_ROW); row++)
        Max.at(row)->setValue(temp.at(row).toDouble());
    //频率
    temp = (set->value("FreqL", "0 0 0 0 0 0 0 0").toString()).split(" ");
    for (int row=0; row < qMin(temp.size(), MAX_ROW); row++)
        FreqL[row] = temp.at(row).toInt();
    temp = (set->value("AreaL", "1000 1000 1000 1000 1000 1000 1000 1000").toString()).split(" ");
    for (int row=0; row < qMin(temp.size(), MAX_ROW); row++)
        AreaL[row] = temp.at(row).toInt();
    //频率
    temp = (set->value("FreqR", "0 0 0 0 0 0 0 0").toString()).split(" ");
    for (int row=0; row < qMin(temp.size(), MAX_ROW); row++)
        FreqR[row] = temp.at(row).toInt();
    temp = (set->value("AreaR", "1000 1000 1000 1000 1000 1000 1000 1000").toString()).split(" ");
    for (int row=0; row < qMin(temp.size(), MAX_ROW); row++)
        AreaR[row] = temp.at(row).toInt();
    //波形
    QByteArray w;
    QByteArray def;
    for (int i=0; i < 400; i++) {
        def.append(QByteArray(1, 0x80));
    }
    for (int row=0; row < qMin(WaveMag.size(), MAX_ROW); row++) {
        QString ByteL = "WaveMagL"+QString::number(row);
        w = set->value(ByteL, def.toBase64()).toString().toUtf8();
        WaveMag.at(row)->WaveBytes[0] = QByteArray::fromBase64(w);
        WaveMag.at(row)->WaveByteShow(QByteArray::fromBase64(w));

        QString ByteR = "WaveMagR"+QString::number(row);
        w = set->value(ByteR, def.toBase64()).toString().toUtf8();
        WaveMag.at(row)->WaveBytes[1] = QByteArray::fromBase64(w);

        QString T1 = Terminal1.at(row)->text();
        QString T2 = Terminal2.at(row)->text();
        WaveMag.at(row)->WaveItem = QString(tr("反嵌%1-%2")).arg(T1).arg(T2).toUtf8();
    }
}

void PageMag::SaveSettings()
{
    QStringList temp;
    temp.append(QString::number(ui->BoxDir->currentIndex()));
    temp.append(QString::number(ui->BoxMain->value()));
    temp.append(QString::number(ui->BoxAuxiliary->value()));
    set->setValue("Other", (temp.join(" ").toUtf8()));

    set->setValue("DirOnly", ui->BoxDirOnly->isChecked());
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
    for (int i=0; i < Max.size(); i++)
        temp.append(QString::number(Max.at(i)->value()));
    set->setValue("Max", (temp.join(" ").toUtf8()));
    temp.clear();
    for (int i=0; i < AreaL.size(); i++)
        temp.append(QString::number(AreaL.at(i)));
    set->setValue("AreaL", (temp.join(" ").toUtf8()));
    temp.clear();
    for (int i=0; i < FreqL.size(); i++)
        temp.append(QString::number(FreqL.at(i)));
    set->setValue("FreqL", (temp.join(" ").toUtf8()));
    temp.clear();
    for (int i=0; i < AreaR.size(); i++)
        temp.append(QString::number(AreaR.at(i)));
    set->setValue("AreaR", (temp.join(" ").toUtf8()));
    temp.clear();
    for (int i=0; i < FreqR.size(); i++)
        temp.append(QString::number(FreqR.at(i)));
    set->setValue("FreqR", (temp.join(" ").toUtf8()));

    for (int row=0; row < WaveMag.size(); row++) {
        QString ByteL = "WaveMagL"+QString::number(row);
        QString ByteR = "WaveMagR"+QString::number(row);
        set->setValue(ByteL, WaveMag.at(row)->WaveBytes.at(0).toBase64());
        set->setValue(ByteR, WaveMag.at(row)->WaveBytes.at(1).toBase64());
    }
}

void PageMag::ItemClick(int r,  int c)
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
        input->showNormal();
        break;
    }
}

void PageMag::ItemChange(QString msg)
{
    ui->TabParams->currentItem()->setText(msg);
}

void PageMag::ReadMessage(quint16 addr,  quint16 cmd,  QByteArray msg)
{
    if (addr != ADDR &&
            addr != WIN_ID_MAG &&
            addr != CAN_ID_DCR &&
            addr != CAN_ID_DCR_WAVE)
        return;
    switch (cmd) {
    case CMD_CAN:
        ExcuteCanCmd(addr, msg);
        break;
    case CMD_START:
        MagMode = MAG_TEST;
        SendCanCmdConfig(msg.toInt());
        emit SendCommand(ADDR, CMD_WAVE_HIDE, NULL);
        SendCanCmdStart(msg.toInt());
        Judge = "OK";
        if (!WaitTimeOut(100)) {
            Judge = "NG";
            SendTestItemsAllError();
        }
        SendTestJudge();
        MagMode = MAG_FREE;
        break;
    case CMD_INIT:
        InitSettings();
        SendTestItemsAllEmpty();
        break;
    case CMD_WAVE:
        SendWave(msg);
        break;
    default:
        break;
    }
}

void PageMag::ExcuteCanCmd(int id,  QByteArray msg)
{
    if (MagMode == MAG_FREE)
        return;
    TimeOut = 0;
    if (id == CAN_ID_DCR_WAVE) {
        ReadCanCmdWave(msg);
        return;
    }
    if (msg.size() >= 4 && (quint8)msg.at(0) == 0x00)
        ReadCanCmdStatus(msg);
    if (msg.size() >= 5 && (quint8)msg.at(0) == 0x02)
        ReadCanCmdResult(msg);
    if (msg.size() >= 2 && (quint8)msg.at(0) == 0x03 && (quint8)msg.at(1) != 0xff)
        ReadCanCmdWaveStart(msg);
    if (msg.size() >= 2 && (quint8)msg.at(0) == 0x03 && (quint8)msg.at(1) == 0xff)
        ReadCanCmdWaveOk(msg);
}

void PageMag::ReadCanCmdStatus(QByteArray msg)
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
    if (MagMode == MAG_TEST && ui->BoxDir->currentIndex() != 0)
        CalculateDir();
    MagMode = MAG_FREE;
}

void PageMag::ReadCanCmdResult(QByteArray msg)
{
    CurrentWave = (quint8)msg.at(1);
    quint16 Area1 = 0;
    if (station == WIN_ID_OUT13)
        Area1 = AreaL[CurrentWave];
    if (station == WIN_ID_OUT14) {
        Area1 = AreaR[CurrentWave];
    }
    quint16 Area2 = quint16(msg.at(2)*256)+quint8(msg.at(3));
    if (MagMode == MAG_SAMPLE) {
        if (station == WIN_ID_OUT13) {
            AreaL[CurrentWave] = Area2;
            FreqL[CurrentWave] = quint8(msg.at(4));
        }
        if (station == WIN_ID_OUT14) {
            AreaR[CurrentWave] = Area2;
            FreqR[CurrentWave] = quint8(msg.at(4));
        }
        return;
    }
    if (ui->BoxDir->currentIndex() != 0 && ui->BoxDirOnly->isChecked())
        return;

    quint16 area = abs((Area2-Area1)*100/Area1);
    QString n = QString("%1%").arg(area);
    QString judge = "OK";
    if (CurrentWave >= Max.size() || area > Max.at(CurrentWave)->value()) {
        Judge = "NG";
        judge = "NG";
    }
    QStringList s = QString(Items.at(CurrentWave)).split("@");
    if (s.at(2) == " ")
        s[2] = n;
    if (s.at(3) == " ")
        s[3] = judge;
    emit SendCommand(ADDR, CMD_ITEM, s.join("@").toUtf8());
}

void PageMag::ReadCanCmdWaveStart(QByteArray )
{
    switch (MagMode) {
    case MAG_SAMPLE:
        WaveMag.at(CurrentWave)->WaveByte.clear();
        break;
    case MAG_TEST:
        WaveMag.at(CurrentWave)->WaveTest.clear();
        break;
    default:
        break;
    }
}

void PageMag::ReadCanCmdWave(QByteArray msg)
{
    switch (MagMode) {
    case MAG_SAMPLE:
        WaveMag.at(CurrentWave)->WaveByte.append(msg);
        break;
    case MAG_TEST:
        WaveMag.at(CurrentWave)->WaveTest.append(msg);
        break;
    default:
        break;
    }
}

void PageMag::ReadCanCmdWaveOk(QByteArray )
{
    quint8 num = station - WIN_ID_OUT13;
    QByteArray w;
    QByteArray i;
    if (MagMode == MAG_SAMPLE) {
        w = WaveMag.at(CurrentWave)->WaveByte;
        WaveMag.at(CurrentWave)->WaveBytes[num] = w;
        WaveMag.at(CurrentWave)->WaveByteShow(w);
    }
    if (MagMode == MAG_TEST) {
        w = WaveMag.at(CurrentWave)->WaveTest;
        i = WaveMag.at(CurrentWave)->WaveItem;
        WaveMag.at(CurrentWave)->WaveTests[num] = w;
        if (ui->BoxDir->currentIndex() != 0 && ui->BoxDirOnly->isChecked())
            return;
        emit SendCommand(ADDR, CMD_WAVE_ITEM, i);
        emit SendCommand(ADDR, CMD_WAVE_BYTE, w);
    }
}

void PageMag::SendTestItemsAllEmpty()
{
    Items.clear();
    WaveNumber.clear();
    QStringList n;
    for (int row = 0; row < Enable.size(); row++) {
        QString T1 = Terminal1.at(qMin(row, Terminal1.size()))->text();
        QString T2 = Terminal2.at(qMin(row, Terminal2.size()))->text();
        QString M1 = Max.at(qMin(row, Max.size()))->text();
        QString s = QString(tr("反嵌%1-%2@%3%@ @ ")).arg(T1).arg(T2).arg(M1);
        Items.append(s);
        WaveMag.at(row)->WaveTest.clear();
    }
    for (int row = 0; row < Enable.size(); row++) {
        if (Enable.at(row)->text() == "Y") {
            n.append(Items.at(row));
            WaveNumber.append(row);
        }
    }
    if (ui->BoxDir->currentIndex() != 0) {
        QString s = QString(tr("磁旋@%1@ @ ")).arg(ui->BoxDir->currentText());
        if (ui->BoxDirOnly->isChecked()) {
            Items.clear();
            n.clear();
        }
        Items.append(s);
        n.append(s);
    }
    emit SendCommand(ADDR, CMD_INIT_ITEM, n.join("\n").toUtf8());
}

void PageMag::SendTestItemsAllError()
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
    if (ui->BoxDir->currentIndex() != 0) {
        QStringList s = QString(Items.last()).split("@");
        if (s.at(2) == " ")
            s[2] = "---";
        if (s.at(3) == " ")
            s[3] = "NG";
        emit SendCommand(ADDR, CMD_ITEM, s.join("@").toUtf8());
    }
}

void PageMag::SendTestJudge()
{
    QString s = QString(tr("反嵌@%1@%2")).arg(CurrentSettings()).arg(Judge);
    emit SendCommand(ADDR, CMD_JUDGE, s.toUtf8());
}

void PageMag::SendCanCmdSample(quint8 s)
{
    QByteArray msg;
    QDataStream out(&msg,  QIODevice::ReadWrite);
    out.setVersion(QDataStream::Qt_4_8);
    quint16 tt = 0;
    for (int row=0; row < Enable.size(); row++) {
        if (Enable.at(row)->text() == "Y")
            tt += 0x0001 << row;
    }
    out << quint16(0x22) << quint8(0x06) << quint8(0x01) << quint8(0x02) << quint8(0x01)
        << quint8(s) << quint8(tt/256) << quint8(tt%256);
    emit SendCommand(ADDR, CMD_CAN, msg);
}

void PageMag::SendCanCmdStart(quint8 s)
{
    QByteArray msg;
    QDataStream out(&msg,  QIODevice::ReadWrite);
    out.setVersion(QDataStream::Qt_4_8);
    quint16 tt = 0;
    for (int row=0; row < Enable.size(); row++) {
        if (Enable.at(row)->text() == "Y")
            tt += 0x0001 << row;
    }
    out << quint16(0x22) << quint8(0x06) << quint8(0x01) << quint8(0x02) << quint8(0x00)
        << quint8(s) << quint8(tt/256) << quint8(tt%256);
    emit SendCommand(ADDR, CMD_CAN, msg);
}

void PageMag::SendCanCmdStop()
{
    QByteArray msg;
    QDataStream out(&msg,  QIODevice::ReadWrite);
    out.setVersion(QDataStream::Qt_4_8);
    out << quint16(0x22) << quint8(0x01) << quint8(0x02);
    emit SendCommand(ADDR, CMD_CAN, msg);
}

void PageMag::SendCanCmdConfig(quint8 s)
{
    station = s;
    QByteArray msg;
    QDataStream out(&msg,  QIODevice::ReadWrite);
    out.setVersion(QDataStream::Qt_4_8);
    for (int row=0; row < Enable.size(); row++) {
        if (Enable.at(row)->text() == "Y") {
            quint8 freq = FreqL.at(row);
            if (s == WIN_ID_OUT13)
                freq = FreqL.at(row);
            if (s == WIN_ID_OUT14)
                freq = FreqR.at(row);
            out << quint16(0x22) << quint8(0x05) << quint8(0x04) << quint8(row)
                << quint8(Terminal1.at(row)->text().toInt())
                << quint8(Terminal2.at(row)->text().toInt())
                << quint8(freq);
        }
    }
    emit SendCommand(ADDR, CMD_CAN, msg);
}

void PageMag::SendWave(QByteArray msg)
{
    int t = WaveNumber.size();
    for (int i=0; i < WaveNumber.size(); i++) {
        if (WaveMag.at(WaveNumber.at(i))->WaveItem == msg)
            t = i;
    }
    if (t == WaveNumber.size())
        return;
    for (int i=0; i < qMin(3, WaveNumber.size()-t); i++) {
        QByteArray w;
        w = WaveMag.at(WaveNumber.at(t+i))->WaveItem;
        emit SendCommand(ADDR, CMD_WAVE_ITEM, w);
        w = WaveMag.at(WaveNumber.at(t+i))->WaveTest;
        emit SendCommand(ADDR, CMD_WAVE_BYTE, w);
    }
}

void PageMag::CalculateDir()
{
    //计算主副相的面积，差积，和左右移动的差积
    qint32 area1, area2, diff, diff1, diff2;
    area1 = 0;
    area2 = 0;
    diff = 0;
    diff1 = 0;
    diff2 = 0;
    int b1 = ui->BoxMain->value()-1;
    int b2 = ui->BoxAuxiliary->value()-1;
    int s1 = WaveMag.at(b1)->WaveTest.size();
    int s2 = WaveMag.at(b2)->WaveTest.size();
    for (int i=10; i < qMin(s1, s2)-10; i++) {
        int P1 = quint8(WaveMag.at(b1)->WaveTest.at(i))-0x80;
        int P2 = quint8(WaveMag.at(b2)->WaveTest.at(i))-0x80;
        area1 += P1*P1;
        area2 += P2*P2;
        diff += (P1-P2)*(P1-P2);
        for (int j=1; j < 11; j++) {
            int P3 = quint8(WaveMag.at(b2)->WaveTest.at(i+j))-0x80;
            int P4 = quint8(WaveMag.at(b2)->WaveTest.at(i-j))-0x80;
            diff1 += (P1-P3)*(P1-P3);
            diff2 += (P1-P4)*(P1-P4);
        }
    }
    diff1 /= 10;
    diff2 /= 10;
    QString n;
    QString judge = "OK";

    if (((diff*3 < area1) &&(diff*3 < area2)) || (area1 < (area2 >> 4)) || (area2 < (area1 >> 4)))
        n = tr("不转"); // 偏移不超过1/3, 或面积差大于3/4, 判定为不转
    else if (diff1 < diff2) // 副相在前
        n = tr("正转");
    else if (diff1 > diff2) // 主相在前
        n = tr("反转");
    else
        n = tr("不转");
    if (n != ui->BoxDir->currentText()) {
        Judge = "NG";
        judge = "NG";
    }
    QStringList s = QString(Items.last()).split("@");
    if (s.at(2) == " ")
        s[2] = n;
    if (s.at(3) == " ")
        s[3] = judge;
    emit SendCommand(ADDR, CMD_ITEM, s.join("@").toUtf8());
}

void PageMag::SendWarnning(QString s)
{
    QVariantHash hash;
    hash.insert("TxAddress", "WinHome");
    hash.insert("TxCommand", "Warnning");
    hash.insert("TxMessage", tr("反嵌异常:\n%1").arg(s));
    emit SendVariant(QVariant::fromValue(hash));
}

bool PageMag::WaitTimeOut(quint16 t)
{
    TimeOut = 0;
    while (MagMode != MAG_FREE) {
        Delay(10);
        TimeOut++;
        if (TimeOut > t)
            return false;
    }
    return true;
}

void PageMag::Delay(int ms)
{
    QElapsedTimer t;
    t.start();
    while (t.elapsed() < ms)
        QCoreApplication::processEvents();
}

QString PageMag::CurrentSettings()
{
    QSettings *ini = new QSettings(INI_PATH, QSettings::IniFormat);
    QString n = ini->value("/GLOBAL/FileInUse", INI_DEFAULT).toString();
    return n.remove(".ini");
}

void PageMag::showEvent(QShowEvent *e)
{
    InitSettings();
    e->accept();
}
/*********************************END OF FILE**********************************/
