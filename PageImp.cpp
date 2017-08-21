/*******************************************************************************
 * Copyright [2016]   <  青岛艾普智能仪器有限公司>
 * All rights reserved.
 *
 * version:     2.1.0.170417
 * author:      zhaonanlin
 * brief:       匝间测试模块
*******************************************************************************/
#include "PageImp.h"
#include "ui_PageImp.h"

PageImp::PageImp(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::PageImp)
{
    ui->setupUi(this);
    InitWindows();
    InitButtons();
    InitSettings();
}

PageImp::~PageImp()
{
    delete ui;
}

void PageImp::InitWindows()
{
#if (QT_VERSION <= QT_VERSION_CHECK(5, 0, 0))
    ui->TabParams->horizontalHeader()->setResizeMode(0, QHeaderView::Stretch);
    ui->TabParams->horizontalHeader()->setResizeMode(1, QHeaderView::Stretch);
    ui->TabParams->horizontalHeader()->setResizeMode(2, QHeaderView::Stretch);
    ui->TabParams->setColumnWidth(3, 50);
    ui->TabParams->horizontalHeader()->setResizeMode(4, QHeaderView::Stretch);
    ui->TabParams->horizontalHeader()->setResizeMode(5, QHeaderView::Stretch);
    ui->TabParams->horizontalHeader()->setResizeMode(6, QHeaderView::Stretch);
    ui->TabParams->horizontalHeader()->setResizeMode(7, QHeaderView::Stretch);
    ui->TabParams->horizontalHeader()->setResizeMode(8, QHeaderView::Stretch);
    ui->TabParams->setColumnWidth(9, 400);
    ui->TabParams->verticalHeader()->setResizeMode(QHeaderView::Stretch);
#else
    ui->TabParams->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Stretch);
    ui->TabParams->horizontalHeader()->setSectionResizeMode(1, QHeaderView::Stretch);
    ui->TabParams->horizontalHeader()->setSectionResizeMode(2, QHeaderView::Stretch);
    ui->TabParams->setColumnWidth(3, 50);
    ui->TabParams->horizontalHeader()->setSectionResizeMode(4, QHeaderView::Stretch);
    ui->TabParams->horizontalHeader()->setSectionResizeMode(5, QHeaderView::Stretch);
    ui->TabParams->horizontalHeader()->setSectionResizeMode(6, QHeaderView::Stretch);
    ui->TabParams->horizontalHeader()->setSectionResizeMode(7, QHeaderView::Stretch);
    ui->TabParams->horizontalHeader()->setSectionResizeMode(8, QHeaderView::Stretch);
    ui->TabParams->setColumnWidth(9, 400);
    ui->TabParams->verticalHeader()->setSectionResizeMode(QHeaderView::Stretch);
#endif
    connect(ui->TabParams, SIGNAL(cellClicked(int, int)), this, SLOT(ItemClick(int, int)));
    input = new PageNum(this);
    QStringList t;
    t  << "1" << "2" << "3" << "4" << "5" << "6" << "7" << "8" << "9" << "10" << "11" << "12";
    input->InitButtons(t);
    connect(input, SIGNAL(ItemChange(QString)), this, SLOT(ItemChange(QString)));
    input->hide();
    ui->WindgetSetImp->setCurrentIndex(0);

    connect(ui->LabelWave, SIGNAL(BlockClick(int)), this, SLOT(BlockClick(int)));
    connect(ui->BoxStart, SIGNAL(valueChanged(int)), this, SLOT(BlockClick(int)));
    connect(ui->BoxEnd, SIGNAL(valueChanged(int)), this, SLOT(BlockClick(int)));

    ui->TabParams->setRowCount(MAX_ROW);
    for (int row = 0; row  <  MAX_ROW; row++) {
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

        Volt.append(new QDoubleSpinBox(this));
        ui->TabParams->setCellWidget(row, 3, Volt.at(row));
        Volt.at(row)->setMaximum(3000);
        Volt.at(row)->setAlignment(Qt::AlignHCenter);
        Volt.at(row)->setButtonSymbols(QDoubleSpinBox::NoButtons);
        Volt.at(row)->setDecimals(0);

        Time.append(new QDoubleSpinBox(this));
        ui->TabParams->setCellWidget(row, 4, Time.at(row));
        Time.at(row)->setMaximum(10);
        Time.at(row)->setAlignment(Qt::AlignHCenter);
        Time.at(row)->setButtonSymbols(QDoubleSpinBox::NoButtons);
        Time.at(row)->setDecimals(0);

        Flut.append(new QDoubleSpinBox(this));
        ui->TabParams->setCellWidget(row, 5, Flut.at(row));
        Flut.at(row)->setMaximum(9999);
        Flut.at(row)->setAlignment(Qt::AlignHCenter);
        Flut.at(row)->setButtonSymbols(QDoubleSpinBox::NoButtons);
        Flut.at(row)->setDecimals(0);

        Phase.append(new QDoubleSpinBox(this));
        ui->TabParams->setCellWidget(row, 6, Phase.at(row));
        Phase.at(row)->setMaximum(9999);
        Phase.at(row)->setAlignment(Qt::AlignHCenter);
        Phase.at(row)->setButtonSymbols(QDoubleSpinBox::NoButtons);
        Phase.at(row)->setDecimals(0);

        Area.append(new QDoubleSpinBox(this));
        ui->TabParams->setCellWidget(row, 7, Area.at(row));
        Area.at(row)->setMaximum(9999);
        Area.at(row)->setAlignment(Qt::AlignHCenter);
        Area.at(row)->setButtonSymbols(QDoubleSpinBox::NoButtons);
        Area.at(row)->setDecimals(0);

        Diff.append(new QDoubleSpinBox(this));
        ui->TabParams->setCellWidget(row, 8, Diff.at(row));
        Diff.at(row)->setMaximum(9999);
        Diff.at(row)->setAlignment(Qt::AlignHCenter);
        Diff.at(row)->setButtonSymbols(QDoubleSpinBox::NoButtons);
        Diff.at(row)->setDecimals(0);

        Freq.append(7);
        Block0.append(0);
        Block1.append(395);
        VoltTest.append(0);

        WaveImp.append(new Waveform(this));
        ui->TabParams->setCellWidget(row, 9, WaveImp.at(row));
    }
    station = WIN_ID_OUT13;
    connect(Volt.at(0), SIGNAL(editingFinished()), this, SLOT(AutoChangeVolt()));
    AvrCount = 0;
    ImpMode = IMP_FREE;
}

void PageImp::InitButtons()
{
    QButtonGroup *btnGroup = new QButtonGroup;
    btnGroup->addButton(ui->BtnSampleImp, Qt::Key_0);
    btnGroup->addButton(ui->BtnExitWave, Qt::Key_1);
    btnGroup->addButton(ui->BtnExitImp, Qt::Key_2);
    btnGroup->addButton(ui->BtnFrequcy1, Qt::Key_3);
    btnGroup->addButton(ui->BtnFrequcy2, Qt::Key_4);
    btnGroup->addButton(ui->BtnFrequcy3, Qt::Key_5);
    btnGroup->addButton(ui->BtnAvarage, Qt::Key_6);
    btnGroup->addButton(ui->BtnExit, Qt::Key_7);
    connect(btnGroup, SIGNAL(buttonClicked(int)), this, SLOT(ReadButtons(int)));
}

void PageImp::ReadButtons(int id)
{
    if (ImpMode != IMP_FREE)
        return;
    switch (id) {
    case Qt::Key_0: //自动采样
        InitStation();
        if (ui->BoxStation->currentIndex()  ==  0) {
            ImpMode = IMP_SAMPLE;
            SendCanCmdConfig();
            SendCanCmdSampleAuto();
        } else {
            ImpMode = IMP_SAMPLE_OTHER;
            SendCanCmdConfig();
            SendCanCmdStart(station);
        }
        AvrCount = 0;
        break;
    case Qt::Key_1:
        ui->WindgetSetImp->setCurrentIndex(0);
        break;
    case Qt::Key_2: //退出
        SaveSettings();
        emit SendCommand(ADDR, CMD_JUMP, NULL);
        break;
    case Qt::Key_3: //减频采样
        ImpMode = IMP_SAMPLE;
        InitStation();
        if (Freq[ui->TabParams->currentRow()] != 0)
            Freq[ui->TabParams->currentRow()]--;
        SendCanCmdConfig();
        SendCanCmdSample(ui->TabParams->currentRow());
        AvrCount = 0;
        break;
    case Qt::Key_4: //定频采样
        ImpMode = IMP_SAMPLE;
        InitStation();
        SendCanCmdConfig();
        SendCanCmdSample(ui->TabParams->currentRow());
        AvrCount = 0;
        break;
    case Qt::Key_5: //加频采样
        ImpMode = IMP_SAMPLE;
        InitStation();
        if (Freq[ui->TabParams->currentRow()] != 14)
            Freq[ui->TabParams->currentRow()]++;
        SendCanCmdConfig();
        SendCanCmdSample(ui->TabParams->currentRow());
        AvrCount = 0;
        break;
    case Qt::Key_6: //添加样品
        ImpMode = IMP_SAMPLE_ADD;
        InitStation();
        SendCanCmdConfig();
        SendCanCmdStart(station);
        WaitTimeOut(1000);
        CalculateAvarageWave();
        break;
    case Qt::Key_7:
        emit SendCommand(ADDR, CMD_JUMP, NULL);
        break;
    default:
        break;
    }
}

void PageImp::InitStation()
{
    if (ui->BoxStation->currentIndex()  ==  0)
        station = WIN_ID_OUT13;
    if (ui->BoxStation->currentIndex()  ==  1)
        station = WIN_ID_OUT14;
}

void PageImp::InitSettings()
{
    QSettings *global = new QSettings(INI_PATH, QSettings::IniFormat);
    global->setIniCodec("GB18030");
    global->beginGroup("GLOBAL");
    FileInUse = global->value("FileInUse", INI_DEFAULT).toString();
    FileInUse.remove(".ini");

    //当前使用的测试项目
    QString t = QString("./config/%1.ini").arg(FileInUse);
    set = new QSettings(t, QSettings::IniFormat);
    set->setIniCodec("GB18030");
    set->beginGroup("SetImp");
    QStringList temp;
    ui->BoxOffset->setChecked(set->value("Offset", true).toBool());
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
    //电压
    temp = (set->value("Volt", "500 500 500 500 500 500 500 500").toString()).split(" ");
    for (int row=0; row < qMin(temp.size(), MAX_ROW); row++)
        Volt.at(row)->setValue(temp.at(row).toDouble());
    temp = (set->value("VoltTest", "500 500 500 500 500 500 500 500").toString().split(" "));
    for (int row=0; row < qMin(temp.size(), MAX_ROW); row++)
        VoltTest[row] = temp.at(row).toInt();
    //次数
    temp = (set->value("Time", "1 1 1 1 1 1 1 1").toString()).split(" ");
    for (int row=0; row < qMin(temp.size(), MAX_ROW); row++)
        Time.at(row)->setValue(temp.at(row).toDouble());
    //电晕
    temp = (set->value("Flut", "0 0 0 0 0 0 0 0").toString()).split(" ");
    for (int row=0; row < qMin(temp.size(), MAX_ROW); row++)
        Flut.at(row)->setValue(temp.at(row).toDouble());
    //相位
    temp = (set->value("Phase", "10 10 10 10 10 10 10 10").toString()).split(" ");
    for (int row=0; row < qMin(temp.size(), MAX_ROW); row++)
        Phase.at(row)->setValue(temp.at(row).toDouble());
    //面积
    temp = (set->value("Area", "10 10 10 10 10 10 10 10").toString()).split(" ");
    for (int row=0; row < qMin(temp.size(), MAX_ROW); row++)
        Area.at(row)->setValue(temp.at(row).toDouble());
    //差积
    temp = (set->value("Diff", "10 10 10 10 10 10 10 10").toString()).split(" ");
    for (int row=0; row < qMin(temp.size(), MAX_ROW); row++)
        Diff.at(row)->setValue(temp.at(row).toDouble());
    //频率
    temp = (set->value("Freq", "7 7 7 7 7 7 7 7").toString()).split(" ");
    for (int row=0; row < qMin(temp.size(), MAX_ROW); row++)
        Freq[row] = temp.at(row).toInt();
    //计算起点
    temp = (set->value("Block0", "5 5 5 5 5 5 5 5").toString()).split(" ");
    for (int row=0; row < qMin(temp.size(), MAX_ROW); row++)
        Block0[row] = temp.at(row).toInt();
    //计算终点
    temp = (set->value("Block1", "395 395 395 395 395 395 395 395").toString()).split(" ");
    for (int row=0; row < qMin(temp.size(), MAX_ROW); row++)
        Block1[row] = temp.at(row).toInt();
    //波形
    QByteArray w;
    QByteArray def;
    for (int i=0; i < 400; i++) {
        def.append(QByteArray(1, 0x02));
        def.append(QByteArray(1, 0x04));
    }
    for (int row=0; row < qMin(WaveImp.size(), MAX_ROW); row++) {
        QString ByteL = "WaveImpL"+QString::number(row);
        w = set->value(ByteL, def.toBase64()).toString().toUtf8();
        WaveImp.at(row)->WaveBytes[0] = QByteArray::fromBase64(w);

        QString ByteR = "WaveImpR"+QString::number(row);
        w = set->value(ByteR, def.toBase64()).toString().toUtf8();
        WaveImp.at(row)->WaveBytes[1] = QByteArray::fromBase64(w);

        QString T1 = Terminal1.at(row)->text();
        QString T2 = Terminal2.at(row)->text();
        WaveImp.at(row)->WaveItem = QString(tr("匝间%1-%2")).arg(T1).arg(T2).toUtf8();
    }
    qDebug() << QTime::currentTime().toString() << "PageImp read OK";
}

void PageImp::SaveSettings()
{
    set->setValue("Offset", ui->BoxOffset->isChecked());
    QStringList temp;
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
    for (int i=0; i < Volt.size(); i++)
        temp.append(Volt.at(i)->text());
    set->setValue("Volt", (temp.join(" ").toUtf8()));
    temp.clear();
    for (int i=0; i < VoltTest.size(); i++)
        temp.append(QString::number(VoltTest.at(i)));
    set->setValue("VoltTest", (temp.join(" ").toUtf8()));
    temp.clear();
    for (int i=0; i < Time.size(); i++)
        temp.append(Time.at(i)->text());
    set->setValue("Time", (temp.join(" ").toUtf8()));
    temp.clear();
    for (int i=0; i < Flut.size(); i++)
        temp.append(Flut.at(i)->text());
    set->setValue("Flut", (temp.join(" ").toUtf8()));
    temp.clear();
    for (int i=0; i < Phase.size(); i++)
        temp.append(Phase.at(i)->text());
    set->setValue("Phase", (temp.join(" ").toUtf8()));
    temp.clear();
    for (int i=0; i < Area.size(); i++)
        temp.append(Area.at(i)->text());
    set->setValue("Area", (temp.join(" ").toUtf8()));
    temp.clear();
    for (int i=0; i < Diff.size(); i++)
        temp.append(Diff.at(i)->text());
    set->setValue("Diff", (temp.join(" ").toUtf8()));
    temp.clear();
    for (int i=0; i < Freq.size(); i++)
        temp.append(QString::number(Freq.at(i)));
    set->setValue("Freq", (temp.join(" ").toUtf8()));
    temp.clear();
    for (int i=0; i < Block0.size(); i++)
        temp.append(QString::number(Block0.at(i)));
    set->setValue("Block0", (temp.join(" ").toUtf8()));
    temp.clear();
    for (int i=0; i < Block1.size(); i++)
        temp.append(QString::number(Block1.at(i)));
    set->setValue("Block1", (temp.join(" ").toUtf8()));

    for (int row=0; row < qMin(WaveImp.size(), ui->TabParams->rowCount()); row++) {
        QString ByteL = "WaveImpL"+QString::number(row);
        QString ByteR = "WaveImpR"+QString::number(row);
        QString L = WaveImp.at(row)->WaveBytes.at(0).toBase64();
        QString R = WaveImp.at(row)->WaveBytes.at(1).toBase64();
        set->setValue(ByteL, L);
        set->setValue(ByteR, R);
    }

    qDebug() << QTime::currentTime().toString() << "PageImp save OK";
}

void PageImp::ItemClick(int r,  int c)
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
    case 9:
        ui->WindgetSetImp->setCurrentIndex(1);
        ui->LabelWave->WaveByteShow(WaveImp.at(r)->WaveByte);
        ui->LabelWave->WaveItemShow(WaveImp.at(r)->WaveItem);
        ui->LabelWave->WaveBlock(Block0.at(r), Block1.at(r));
        ui->BoxStart->setValue(Block0.at(r));
        ui->BoxEnd->setValue(Block1.at(r));
        CurrentWave = r;
        if (ui->BoxStation->currentIndex() == 1) {
            ui->BtnFrequcy1->setEnabled(false);
            ui->BtnFrequcy3->setEnabled(false);
        } else {
            ui->BtnFrequcy1->setEnabled(true);
            ui->BtnFrequcy3->setEnabled(true);
        }
        break;
    default:
        break;
    }
}

void PageImp::ItemChange(QString msg)
{
    ui->TabParams->currentItem()->setText(msg);
}

void PageImp::BlockClick(int x)
{
    if (x >=200) {
        ui->LabelWave->WaveBlock(ui->LabelWave->Block0, x);
        ui->BoxEnd->setValue(x);
        Block1[CurrentWave] = x;
    } else {
        ui->LabelWave->WaveBlock(x, ui->LabelWave->Block1);
        ui->BoxStart->setValue(x);
        Block0[CurrentWave] = x;
    }
}

void PageImp::ReadMessage(quint16 addr,  quint16 cmd,  QByteArray msg)
{
    switch (addr) {
    case ADDR:
    case WIN_ID_IMP:
    case CAN_ID_IMP:
    case CAN_ID_IMP_WAVE:
        break;
    default:
        return;
        break;
    }
    switch (cmd) {
    case CMD_CAN:
        ExcuteCanCmd(addr, msg);
        break;
    case CMD_CHECK:
        ImpMode = IMP_INIT;
        SendCanCmdStatus();
        if (!WaitTimeOut(30))
            SendWarnning("超时");
        ImpMode = IMP_FREE;
        break;
    case CMD_START:
        ImpMode = IMP_TEST;
        emit SendCommand(ADDR, CMD_WAVE_HIDE, NULL);
        SendCanCmdStart(msg.toInt());
        JudgeAll = "OK";
        if (!WaitTimeOut(1000)) {
            JudgeAll = "NG";
            SendTestItemsAllError();
        }
        SendTestJudge();
        ImpMode = IMP_FREE;
        break;
    case CMD_INIT:
        InitSettings();
        InitTestItems();
        SendCanCmdConfig();
        break;
    case CMD_WAVE:
        SendWave(msg);
        break;
    default:
        break;
    }
}

void PageImp::ExcuteCanCmd(int id, QByteArray msg)
{
    if (ImpMode  ==  IMP_FREE)
        return;
    TimeOut = 0;
    if (id  ==  CAN_ID_IMP_WAVE) {
        ReadCanCmdWave(msg);
        return;
    }
    if (msg.size() == 4 && (quint8)msg.at(0) == 0x00)
        ReadCanCmdStatus(msg);
    if (msg.size() == 7 && (quint8)msg.at(0) == 0x02)
        ReadCanCmdSample(msg);
    if (msg.size() == 5 && (quint8)msg.at(0) == 0x03)
        ReadCanCmdWaveStart(msg);
    if (msg.size() == 2 && (quint8)msg.at(0) == 0x03 && (quint8)msg.at(1) == 0xff)
        ReadCanCmdWaveOk(msg);
}

void PageImp::InitTestItems()
{
    Items.clear();
    WaveNumber.clear();
    QStringList n;
    for (int row = 0; row < Enable.size(); row++) {
        QStringList s;
        QString T1 = Terminal1.at(qMin(row, Terminal1.size()))->text();
        QString T2 = Terminal2.at(qMin(row, Terminal2.size()))->text();
        QString V = Volt.at(qMin(row, Volt.size()))->text();
        QString C = Flut.at(qMin(row, Flut.size()))->text();
        QString P = Phase.at(qMin(row, Phase.size()))->text();
        QString A = Area.at(qMin(row, Area.size()))->text();
        QString D = Diff.at(qMin(row, Diff.size()))->text();

        s.append(QString(tr("匝间%1-%2")).arg(T1).arg(T2));
        if (C.toInt()  ==  0)
            s.append(QString("%1V, %2, %3, %4").arg(V).arg(P).arg(A).arg(D));
        else
            s.append(QString("%1V, %2, %3, %4, %5").arg(V).arg(C).arg(P).arg(A).arg(D));
        s.append(" ");
        s.append(" ");
        Items.append(s.join("@"));
        WaveImp.at(row)->WaveTest.clear();
    }
    for (int row = 0; row < Enable.size(); row++) {
        if (Enable.at(row)->text()  ==  "Y") {
            n.append(Items.at(row));
            WaveNumber.append(row);
        }
    }
    emit SendCommand(ADDR, CMD_INIT_ITEM, n.join("\n").toUtf8());
}

void PageImp::SendTestItemsAllError()
{
    for (int row = 0; row < Enable.size(); row++) {
        if (Enable.at(row)->text()  ==  "Y") {
            QStringList s = QString(Items.at(row)).split("@");
            if (s.at(2)  ==  " ")
                s[2] = "---";
            if (s.at(3)  ==  " ")
                s[3] = "NG";
            emit SendCommand(ADDR, CMD_ITEM, s.join("@").toUtf8());
        }
    }
}

void PageImp::SendCanCmdStatus()
{
    QByteArray msg;
    QDataStream out(&msg,  QIODevice::ReadWrite);
    out.setVersion(QDataStream::Qt_4_8);
    out << quint16(0x24) << quint8(0x01) << quint8(0x00);
    emit SendCommand(ADDR, CMD_CAN, msg);
}

void PageImp::SendCanCmdSampleAuto()
{
    QByteArray msg;
    QDataStream out(&msg,  QIODevice::ReadWrite);
    out.setVersion(QDataStream::Qt_4_8);
    quint16 tt = 0;
    for (int row=0; row < Enable.size(); row++) {
        if (Enable.at(row)->text()  ==  "Y")
            tt += 0x0001 << row;
    }
    quint8 mode = 0x01;
    if (ui->BoxOffset->isChecked())
        mode = 0x04;
    out << quint16(0x24) << quint8(0x05) << quint8(0x01) << quint8(mode) << quint8(station)
        << quint8(tt/256) << quint8(tt%256);
    emit SendCommand(ADDR, CMD_CAN, msg);
}

void PageImp::SendCanCmdSample(quint16 t)
{
    QByteArray msg;
    QDataStream out(&msg,  QIODevice::ReadWrite);
    out.setVersion(QDataStream::Qt_4_8);
    quint16 tt = 0x0001 << t;
    out << quint16(0x24) << quint8(0x05) << quint8(0x01) << quint8(0x02) << quint8(station)
        << quint8(tt/256) << quint8(tt%256);
    emit SendCommand(ADDR, CMD_CAN, msg);
}

void PageImp::SendCanCmdStart(quint8 pos)
{
    station = pos;
    QByteArray msg;
    QDataStream out(&msg,  QIODevice::ReadWrite);
    out.setVersion(QDataStream::Qt_4_8);
    quint16 tt = 0;
    for (int row=0; row < Enable.size(); row++) {
        if (Enable.at(row)->text()  ==  "Y")
            tt += 0x0001 << row;
    }
    out << quint16(0x24) << quint8(0x05) << quint8(0x01) << quint8(0x00) << quint8(station)
        << quint8(tt/256) << quint8(tt%256);
    emit SendCommand(ADDR, CMD_CAN, msg);
}

void PageImp::SendCanCmdStop()
{
    QByteArray msg;
    QDataStream out(&msg,  QIODevice::ReadWrite);
    out.setVersion(QDataStream::Qt_4_8);
    out << quint16(0x24) << quint8(0x01) << quint8(0x02);
    emit SendCommand(ADDR, CMD_CAN, msg);
}

void PageImp::SendCanCmdConfig()
{
    QByteArray msg;
    QDataStream out(&msg,  QIODevice::ReadWrite);
    out.setVersion(QDataStream::Qt_4_8);
    for (int row=0; row < Enable.size(); row++) {
        int v = 0;
        switch (ImpMode) {
        case IMP_FREE:
        case IMP_INIT:
        case IMP_TEST:
        case IMP_SAMPLE_ADD:
        case IMP_SAMPLE_OTHER:
            v = VoltTest.at(row);
            break;
        case IMP_SAMPLE:
            v = Volt.at(row)->value();
            break;
        default:
            break;
        }
        out << quint16(0x24) << quint8(0x08) << quint8(0x03) << quint8(row)
            << quint8(Terminal1.at(row)->text().toInt())
            << quint8(Terminal2.at(row)->text().toInt())
            << quint8(v/256) << quint8(v%256)
            << quint8(CalculateGear(row)) << quint8(Freq.at(row));
    }
    emit SendCommand(ADDR, CMD_CAN, msg);
}

void PageImp::SendTestJudge()
{
    QStringList s;
    s.append("匝间");
    s.append(FileInUse);
    s.append(JudgeAll);
    emit SendCommand(ADDR, CMD_JUDGE, s.join("@").toUtf8());
}

void PageImp::ReadCanCmdStatus(QByteArray msg)
{
    int s = quint8(msg.at(1));
    switch (s) {
    case 0x00:
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
    ImpMode = IMP_FREE;
}

void PageImp::CalculateResult(QByteArray )
{
    quint8 num = station - WIN_ID_OUT13;
    WaveImp.at(CurrentWave)->InitWaveByte(num);
    WaveImp.at(CurrentWave)->InitWaveTest(num);

    qint32 Area1 = 0;
    qint32 Area2 = 0;
    qint32 Area3 = 0;
    qint32 Phase1 = 0;
    qint32 Phase2 = 0;
    qint32 Flut1 = 0;
    qint32 Flut2 = 0;
    qint32 F = 0;
    qint32 A = 0;
    qint32 D = 0;
    qint32 P = 0;
    quint16 s = Block0.at(CurrentWave);
    quint16 e = Block1.at(CurrentWave);
    if (s  <  1)
        s = 1;
    if (e > 398)
        e = 398;
    for (int i=s; i < e; i++) {
        int a1 = WaveImp.at(CurrentWave)->WaveByteH.at(i);
        int a2 = WaveImp.at(CurrentWave)->WaveTestH.at(i);
        Area1 += abs(a1-0x200);
        Area2 += abs(a2-0x200);
        int b1 = WaveImp.at(CurrentWave)->WaveByteH.at(i-1);
        int b2 = WaveImp.at(CurrentWave)->WaveByteH.at(i);
        int b3 = WaveImp.at(CurrentWave)->WaveByteH.at(i+1);
        int c1 = WaveImp.at(CurrentWave)->WaveTestH.at(i-1);
        int c2 = WaveImp.at(CurrentWave)->WaveTestH.at(i);
        int c3 = WaveImp.at(CurrentWave)->WaveTestH.at(i+1);
        Area3 += abs((b1+b2*2+b3)-(c1+c2*2+c3));

        Flut1 += abs(b2-b3);
        Flut2 += abs(c2-c3);
        Phase1 += (a1-0x200)*(a1-0x200);
        Phase2 += (a1-0x200)*(a2-0x200);
    }
    F = qMin(99, abs((Flut2-Flut1)*100/Flut1));
    P = qMin(99, abs((Phase1-Phase2)*100/Phase1));
    A = qMin(99, abs((Area2-Area1)*100/Area1));
    D = qMin(99, abs(qMin(Area2, Area3/4)*100/Area1));

    QString n;
    QString judge = "OK";
    int number = CurrentWave;
    if (Flut.at(qMin(number, Flut.size()-1))->value() != 0)
        n = QString("电晕:%1,相位:%2%,面积:%3%,差积:%4%").arg(F).arg(P).arg(A).arg(D);
    else
        n = QString("相位:%1,面积:%2%,差积:%3%").arg(P).arg(A).arg(D);

    if (abs(A) >= Area.at(qMin(number, Area.size()-1))->value())
        judge = "NG";
    else if (abs(D) >= Diff.at(qMin(number, Diff.size()-1))->value())
        judge = "NG";
    else if (abs(F) >= Flut.at(qMin(number, Flut.size()-1))->value() &&
             Flut.at(qMin(number, Flut.size()-1))->value() != 0)
        judge = "NG";
    else if (abs(P) >= Phase.at(qMin(number, Phase.size()-1))->value())
        judge = "NG";
    if (judge  ==  "NG")
        JudgeAll = "NG";
    QStringList t = QString(Items.at(CurrentWave)).split("@");
    if (t.at(2)  ==  " ")
        t[2] = n;
    if (t.at(3)  ==  " ")
        t[3] = judge;
    emit SendCommand(ADDR, CMD_ITEM, t.join("@").toUtf8());
}

void PageImp::CalculateAvarageWave()
{
    QByteArray w;
    quint8 num = station - WIN_ID_OUT13;
    QByteArray byte = WaveImp.at(CurrentWave)->WaveByte;
    QByteArray test = WaveImp.at(CurrentWave)->WaveBytes[num];
    AvrCount++;
    for (int i=0; i < qMin(byte.size(), test.size()); i++)
        w.append(quint8(test.at(i))+(quint8(test.at(i))-quint8(byte.at(i)))/AvrCount);
    if (AvrCount  ==  1)
        w = WaveImp.at(CurrentWave)->WaveByte;
    WaveImp.at(CurrentWave)->WaveBytes[num] = w;
    WaveImp.at(CurrentWave)->WaveByteShow(w);
    ui->LabelWave->WaveByteShow(w);
}

void PageImp::ReadCanCmdSample(QByteArray msg)
{
    if (ImpMode != IMP_SAMPLE)
        return;
    quint8 num = quint8(msg.at(1));
    Freq[num] = quint8(msg.at(3));
    VoltTest[num] = quint16(msg.at(4)*256)+quint8(msg.at(5));
}

void PageImp::ReadCanCmdWave(QByteArray msg)
{
    switch (ImpMode) {
    case IMP_FREE:
    case IMP_INIT:
        break;
    case IMP_SAMPLE:
    case IMP_SAMPLE_ADD:
    case IMP_SAMPLE_OTHER:
        WaveImp.at(CurrentWave)->WaveByte.append(msg);
        break;
    case IMP_TEST:
        WaveImp.at(CurrentWave)->WaveTest.append(msg);
        break;
    default:
        break;
    }
}

void PageImp::ReadCanCmdWaveOk(QByteArray msg)
{
    QByteArray w;
    quint8 num = station - WIN_ID_OUT13;
    switch (ImpMode) {
    case IMP_SAMPLE:
    case IMP_SAMPLE_ADD:
    case IMP_SAMPLE_OTHER:
        w = WaveImp.at(CurrentWave)->WaveByte;
        WaveImp.at(CurrentWave)->WaveBytes[num] = w;
        WaveImp.at(CurrentWave)->WaveByteShow(w);
        ui->LabelWave->WaveByteShow(w);
        break;
    case IMP_TEST:
        w = WaveImp.at(CurrentWave)->WaveTest;
        WaveImp.at(CurrentWave)->WaveTests[num] = w;
        emit SendCommand(ADDR, CMD_WAVE_BYTE, w);
        CalculateResult(msg);
        break;
    }
}

void PageImp::ReadCanCmdWaveStart(QByteArray msg)
{
    CurrentWave = (quint8)msg.at(1);
    quint8 num = station - WIN_ID_OUT13;
    QByteArray w = WaveImp.at(CurrentWave)->WaveBytes[num];
    QByteArray i = WaveImp.at(CurrentWave)->WaveItem;
    switch (ImpMode) {
    case IMP_FREE:
    case IMP_INIT:
        break;
    case IMP_SAMPLE:
    case IMP_SAMPLE_ADD:
    case IMP_SAMPLE_OTHER:
        WaveImp.at(CurrentWave)->WaveByte.clear();
        break;
    case IMP_TEST:
        WaveImp.at(CurrentWave)->WaveTest.clear();
        emit SendCommand(ADDR, CMD_WAVE_ITEM, i);
        emit SendCommand(ADDR, CMD_WAVE_TEST, w);
        break;
    default:
        break;
    }
}

void PageImp::SendWave(QByteArray msg)
{
    quint8 num = station - WIN_ID_OUT13;
    int t = WaveNumber.size();
    for (int i=0; i < WaveNumber.size(); i++) {
        if (WaveImp.at(WaveNumber.at(i))->WaveItem  ==  msg)
            t = i;
    }
    if (t  ==  WaveNumber.size())
        return;
    for (int i=0; i < qMin(3, WaveNumber.size()-t); i++) {
        QByteArray w;
        w = WaveImp.at(WaveNumber.at(t+i))->WaveItem;
        emit SendCommand(ADDR, CMD_WAVE_ITEM, w);
        w = WaveImp.at(WaveNumber.at(t+i))->WaveBytes[num];
        emit SendCommand(ADDR, CMD_WAVE_TEST, w);
        w = WaveImp.at(WaveNumber.at(t+i))->WaveTest;
        emit SendCommand(ADDR, CMD_WAVE_BYTE, w);
    }
}

int PageImp::CalculateGear(int row)
{
    int gear = 0;
    if (Volt.at(row)->value() <= 1000)
        gear = 1;
    else if (Volt.at(row)->value() <= 2000)
        gear = 2;
    else if (Volt.at(row)->value() <= 4000)
        gear = 3;
    else if (Volt.at(row)->value() <= 5000)
        gear = 4;
    gear <<=  4;
    gear += Time.at(row)->value()+1;
    return gear;
}

bool PageImp::WaitTimeOut(quint16 t)
{
    TimeOut = 0;
    while (ImpMode != IMP_FREE) {
        if (ImpMode == IMP_TEST && TimeOut%50 == 0)
            SendCanCmdStatus();
        Delay(10);
        TimeOut++;
        if (TimeOut > t)
            return false;
    }
    return true;
}

void PageImp::Delay(int ms)
{
    QElapsedTimer t;
    t.start();
    while (t.elapsed() < ms)
        QCoreApplication::processEvents();
}

void PageImp::AutoChangeVolt()
{
    for (int i=1; i < Volt.size(); i++)
        Volt.at(i)->setValue(Volt.at(0)->value());
}

void PageImp::showEvent(QShowEvent *e)
{
    InitSettings();
    e->accept();
}

void PageImp::SendWarnning(QString s)
{
    QVariantHash hash;
    hash.insert("TxAddress", "WinHome");
    hash.insert("TxCommand", "Warnning");
    hash.insert("TxMessage", tr("匝间异常:\n%1").arg(s));
    emit SendVariant(QVariant::fromValue(hash));
}
/*********************************END OF FILE**********************************/
