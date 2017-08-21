/*******************************************************************************
 * Copyright [2016]   <  青岛艾普智能仪器有限公司>
 * All rights reserved.
 *
 * version:     2.1.0.170417
 * author:      zhaonanlin
 * brief:       型号管理模块
*******************************************************************************/
#include "WinType.h"
#include "ui_WinType.h"

WinType::WinType(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::WinType)
{
    ui->setupUi(this);
    InitWindows();
    InitButtons();
    InitSettings();
}

WinType::~WinType()
{
    delete ui;
}

void WinType::InitWindows()
{
    ItemNames << "清除" << "电阻" << "反嵌" << "绝缘" << "交耐" << "直耐" << "匝间" << "电感"
              << "功率" << "低启" << "堵转" << "转速" << "泄漏" << "PG" << "EMF" << "DCBL";
#if (QT_VERSION  <= QT_VERSION_CHECK(5, 0, 0))
    ui->TabColor->horizontalHeader()->setResizeMode(QHeaderView::Stretch);
    ui->TabColor->verticalHeader()->setResizeMode(QHeaderView::Stretch);
    ui->TabWire->horizontalHeader()->setResizeMode(QHeaderView::Stretch);
    ui->TabWire->verticalHeader()->setResizeMode(QHeaderView::Stretch);
    ui->TabFile->horizontalHeader()->setResizeMode(QHeaderView::Stretch);
    ui->TabTest->horizontalHeader()->setResizeMode(QHeaderView::Stretch);
#else
    ui->TabColor->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->TabColor->verticalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->TabWire->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->TabWire->verticalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->TabFile->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->TabTest->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->BoxTestNG->setEnabled(false);
#endif
    for (int i=0; i < ui->TabColor->rowCount(); i++) {
        for (int j=0; j < ui->TabColor->columnCount(); j++) {
            ui->TabColor->setItem(i, j, new QTableWidgetItem);
        }
    }
    ui->TabColor->item(0, 0)->setBackgroundColor(QColor(255, 255, 255, 255));
    ui->TabColor->item(1, 0)->setBackgroundColor(QColor(92, 217, 254, 255));
    ui->TabColor->item(2, 0)->setBackgroundColor(QColor(128, 181, 154, 255));
    ui->TabColor->item(3, 0)->setBackgroundColor(QColor(185, 0, 254, 255));
    ui->TabColor->item(4, 0)->setBackgroundColor(QColor(228, 0, 254, 255));
    ui->TabColor->item(5, 0)->setBackgroundColor(QColor(240, 78, 153, 255));
    ui->TabColor->item(6, 0)->setBackgroundColor(QColor(254, 121, 115, 255));
    ui->TabColor->item(7, 0)->setBackgroundColor(QColor(250, 204, 94, 255));
    ui->TabColor->item(8, 0)->setBackgroundColor(QColor(244, 254, 124, 255));
    ui->TabColor->item(9, 0)->setBackgroundColor(QColor(149, 237, 124, 255));

    ui->TabColor->item(0, 1)->setBackgroundColor(QColor(167, 167, 167, 255));
    ui->TabColor->item(1, 1)->setBackgroundColor(QColor(29, 203, 254, 255));
    ui->TabColor->item(2, 1)->setBackgroundColor(QColor(87, 151, 154, 255));
    ui->TabColor->item(3, 1)->setBackgroundColor(QColor(157, 0, 254, 255));
    ui->TabColor->item(4, 1)->setBackgroundColor(QColor(208, 0, 254, 255));
    ui->TabColor->item(5, 1)->setBackgroundColor(QColor(234, 0, 114, 255));
    ui->TabColor->item(6, 1)->setBackgroundColor(QColor(254, 69, 52, 255));
    ui->TabColor->item(7, 1)->setBackgroundColor(QColor(250, 174, 0, 255));
    ui->TabColor->item(8, 1)->setBackgroundColor(QColor(242, 254, 55, 255));
    ui->TabColor->item(9, 1)->setBackgroundColor(QColor(106, 231, 71, 255));

    ui->TabColor->item(0, 2)->setBackgroundColor(QColor(120, 120, 120, 255));
    ui->TabColor->item(1, 2)->setBackgroundColor(QColor(25, 172, 223, 255));
    ui->TabColor->item(2, 2)->setBackgroundColor(QColor(52, 100, 237, 255));
    ui->TabColor->item(3, 2)->setBackgroundColor(QColor(117, 0, 215, 255));
    ui->TabColor->item(4, 2)->setBackgroundColor(QColor(165, 0, 195, 255));
    ui->TabColor->item(5, 2)->setBackgroundColor(QColor(187, 0, 87, 255));
    ui->TabColor->item(6, 2)->setBackgroundColor(QColor(254, 0, 0, 255));
    ui->TabColor->item(7, 2)->setBackgroundColor(QColor(249, 121, 0, 255));
    ui->TabColor->item(8, 2)->setBackgroundColor(QColor(246, 254, 0, 255));
    ui->TabColor->item(9, 2)->setBackgroundColor(QColor(76, 205, 28, 255));

    ui->TabColor->item(0, 3)->setBackgroundColor(QColor(69, 69, 69, 255));
    ui->TabColor->item(1, 3)->setBackgroundColor(QColor(25, 143, 184, 255));
    ui->TabColor->item(2, 3)->setBackgroundColor(QColor(44, 78, 200, 255));
    ui->TabColor->item(3, 3)->setBackgroundColor(QColor(78, 0, 153, 255));
    ui->TabColor->item(4, 3)->setBackgroundColor(QColor(133, 0, 163, 255));
    ui->TabColor->item(5, 3)->setBackgroundColor(QColor(155, 0, 75, 255));
    ui->TabColor->item(6, 3)->setBackgroundColor(QColor(226, 0, 0, 255));
    ui->TabColor->item(7, 3)->setBackgroundColor(QColor(221, 86, 0, 255));
    ui->TabColor->item(8, 3)->setBackgroundColor(QColor(197, 196, 0, 255));
    ui->TabColor->item(9, 3)->setBackgroundColor(QColor(64, 172, 27, 255));

    ui->TabColor->item(0, 4)->setBackgroundColor(QColor(35, 35, 35, 255));
    ui->TabColor->item(1, 4)->setBackgroundColor(QColor(19, 112, 144, 255));
    ui->TabColor->item(2, 4)->setBackgroundColor(QColor(27, 50, 134, 255));
    ui->TabColor->item(3, 4)->setBackgroundColor(QColor(64, 0, 124, 255));
    ui->TabColor->item(4, 4)->setBackgroundColor(QColor(105, 0, 128, 255));
    ui->TabColor->item(5, 4)->setBackgroundColor(QColor(122, 0, 60, 255));
    ui->TabColor->item(6, 4)->setBackgroundColor(QColor(172, 0, 0, 255));
    ui->TabColor->item(7, 4)->setBackgroundColor(QColor(166, 68, 0, 255));
    ui->TabColor->item(8, 4)->setBackgroundColor(QColor(146, 149, 0, 255));
    ui->TabColor->item(9, 4)->setBackgroundColor(QColor(49, 134, 17, 255));

    ui->TabColor->item(0, 5)->setBackgroundColor(QColor(0, 0, 0, 255));
    ui->TabColor->item(1, 5)->setBackgroundColor(QColor(15, 82, 105, 255));
    ui->TabColor->item(2, 5)->setBackgroundColor(QColor(19, 34, 86, 255));
    ui->TabColor->item(3, 5)->setBackgroundColor(QColor(53, 3, 100, 255));
    ui->TabColor->item(4, 5)->setBackgroundColor(QColor(89, 2, 108, 255));
    ui->TabColor->item(5, 5)->setBackgroundColor(QColor(106, 6, 55, 255));
    ui->TabColor->item(6, 5)->setBackgroundColor(QColor(152, 2, 2, 255));
    ui->TabColor->item(7, 5)->setBackgroundColor(QColor(144, 61, 3, 255));
    ui->TabColor->item(8, 5)->setBackgroundColor(QColor(116, 118, 2, 255));
    ui->TabColor->item(9, 5)->setBackgroundColor(QColor(0, 255, 0, 255));

    ui->BoxType->setView(new QListView);
    ui->BoxTestNG->setView(new QListView(this));
    ui->Type->setCurrentIndex(0);
    ui->Other->setCurrentIndex(0);

    QRegExp rx1;
    rx1.setPattern("^[-|0-9|A-Z|a-z|^\s]{1,16}$"); // 限制接受1至16个字符,减号、数字和英文字母
    QValidator *validator_16c = new QRegExpValidator(rx1, this);
    ui->EditTypeName->setValidator(validator_16c);
}

void WinType::InitButtons()
{
    btnGroup = new QButtonGroup;
    btnGroup->addButton(ui->BtnInsert, Qt::Key_0);
    btnGroup->addButton(ui->BtnDelete, Qt::Key_1);
    btnGroup->addButton(ui->BtnCheck, Qt::Key_3);
    btnGroup->addButton(ui->BtnConfExit, Qt::Key_4);
    connect(btnGroup, SIGNAL(buttonClicked(int)), this, SLOT(ReadButtons(int)));
    connect(ui->BoxType, SIGNAL(currentIndexChanged(int)), this, SLOT(ChangeMotorTypes()));
    connect(ui->TabWire, SIGNAL(cellClicked(int, int)), this, SLOT(ShowWireColorWindow()));
    connect(ui->TabColor, SIGNAL(cellClicked(int, int)), this, SLOT(SelectWireColor(int, int)));
    connect(ui->TabTest, SIGNAL(cellClicked(int, int)), this, SLOT(ShowAvailableItem(int, int)));

    itemButtons = new QButtonGroup;
    itemButtons->addButton(ui->BtnItemSet, 0x00);
    itemButtons->addButton(ui->BtnItemDcr, 0x01);
    itemButtons->addButton(ui->BtnItemMag, 0x02);
    itemButtons->addButton(ui->BtnItemInr, 0x03);
    itemButtons->addButton(ui->BtnItemAcw, 0x04);
    itemButtons->addButton(ui->BtnItemDcw, 0x05);
    itemButtons->addButton(ui->BtnItemImp, 0x06);
    itemButtons->addButton(ui->BtnItemInd, 0x07);
    itemButtons->addButton(ui->BtnItemPwr, 0x08);
    itemButtons->addButton(ui->BtnItemLvs, 0x09);
    itemButtons->addButton(ui->BtnItemLck, 0x0A);
    itemButtons->addButton(ui->BtnItemDelete, Qt::Key_0);
    connect(itemButtons, SIGNAL(buttonClicked(int)), this, SLOT(ReadItemButtons(int)));
    for (int i=1; i < itemButtons->buttons().size()-1; i++) {
        itemButtons->buttons().at(i)->hide();
    }
}

void WinType::ReadButtons(int id)
{
    switch (id) {
    case Qt::Key_0:
        if (CurrentUser() == 0) {
            SendWarnning(tr("用户权限不足"));
            return;
        }
        CopySettings(ui->EditTypeName->text());
        ReadAllSettings();
        ReadAvailableItems();
        InitSettings();
        break;
    case Qt::Key_1:
        if (CurrentUser() == 0) {
            SendWarnning(tr("用户权限不足"));
            return;
        }
        RemoveSettings();
        ReadAllSettings();
        break;
    case Qt::Key_3:
        QuerySettings();
        break;
    case Qt::Key_4:
        if (CurrentUser() == 1)
            SaveSettings();
        emit SendCommand(ADDR, CMD_JUMP, NULL);
        break;
    default:
        break;
    }
}

void WinType::ReadItemButtons(int id)
{
    switch (id) {
    case Qt::Key_0:
        if (ui->TabTest->currentRow() == ui->TabTest->rowCount()-1) {
            break;
        }
        ui->TabTest->removeRow(ui->TabTest->currentRow());
        break;
    case 0x00:
        JumptoSetWindows();
        break;
    default:
        AddTestItem(id);
        break;
    }
    ui->Other->setCurrentIndex(0);
}

void WinType::AddTestItem(int id)
{
    if (ui->TabTest->currentRow() == ui->TabTest->rowCount()-1) {
        ui->TabTest->setItem(ui->TabTest->rowCount()-1, 0, new QTableWidgetItem);
        ui->TabTest->item(ui->TabTest->rowCount()-1, 0)->setTextAlignment(Qt::AlignCenter);
        ui->TabTest->setRowCount(ui->TabTest->rowCount()+1);
    }
    ui->TabTest->currentItem()->setText(itemButtons->buttons().at(id)->text());
}

void WinType::ChangeMotorTypes()
{
    QString jpg = QString(":/source/%1.jpg").arg(ui->BoxType->currentText());
    ui->labelType->setPixmap(QPixmap(jpg));
}

void WinType::ReadAllSettings()
{
    QDir dir("./config");
    dir.setFilter(QDir::Files | QDir::Hidden | QDir::NoSymLinks);
    dir.setSorting(QDir::Size | QDir::Reversed);

    FileNames.clear();
    QFileInfoList list = dir.entryInfoList();
    for (int i=0; i < list.size(); i++)
        FileNames.append(list.at(i).fileName());

    if (FileNames.isEmpty())
        FileNames.append(INI_DEFAULT);

    ui->TabFile->setRowCount(FileNames.size());
    for (int i=0; i < FileNames.size(); i++) {
        ui->TabFile->setItem(i, 0, new QTableWidgetItem);
        ui->TabFile->item(i, 0)->setTextAlignment(Qt::AlignCenter);
        ui->TabFile->item(i, 0)->setText(QString(FileNames.at(i)).remove(".ini"));
    }
}



void WinType::ReadAvailableItems()
{
    //可使用的测试项目
    QStringList temp = EnableItems();
    for (int i=0; i < temp.size(); i++) {
        int t = temp.at(i).toInt();
        if (t  <  itemButtons->buttons().size())
            itemButtons->buttons().at(t)->show();
    }
}

void WinType::InitSettings()
{
    ui->TextTypeShow->setText(CurrentSettings());

    QStringList temp;
    //当前使用的测试项目
    QString n = QString("./config/%1.ini").arg(CurrentSettings());
    QSettings *ini = new QSettings(n, QSettings::IniFormat);
    ini->setIniCodec("GB18030");
    ini->beginGroup("GLOBAL");

    temp = CurrentItems();
    ui->TabTest->setRowCount(temp.size()+1);
    for (int i=0; i < temp.size()+1; i++) {
        ui->TabTest->setItem(i, 0, new QTableWidgetItem);
        ui->TabTest->item(i, 0)->setTextAlignment(Qt::AlignCenter);
    }
    for (int i=0; i < temp.size(); i++) {
        if (temp.at(i).toInt() == 0)
            continue;
        ui->TabTest->item(i, 0)->setText(ItemNames.at(temp.at(i).toInt()));
    }

    temp = ini->value("WireColor").toString().split(" ");
    for (int i=0; i < qMin(temp.size(), ui->TabWire->rowCount()*2); i++) {
        ui->TabWire->item(i/2, i%2)->setBackgroundColor(QColor(temp.at(i)));
    }

    ui->BoxTestNG->setCurrentIndex(ini->value("TestNG", "1").toInt());
    ui->BoxType->setCurrentIndex(ini->value("WinType", "0").toInt());
    qDebug() << QTime::currentTime().toString() << "WinType read OK";
}

void WinType::SaveSettings()
{
    QStringList temp;
    //当前使用的测试项目
    QString n = QString("./config/%1.ini").arg(CurrentSettings());
    QSettings *ini = new QSettings(n, QSettings::IniFormat);
    ini->setIniCodec("GB18030");
    ini->beginGroup("GLOBAL");

    for (int i=0; i < ui->TabTest->rowCount()-1; i++) {
        if (!ui->TabTest->item(i, 0)->text().isEmpty()) {
            for (int j=0; j < ItemNames.size(); j++) {
                if (ui->TabTest->item(i, 0)->text() == ItemNames.at(j))
                    temp.append(QString::number(j));
            }
        }
    }
    ini->setValue("ProjToTest", temp.join(" "));

    temp.clear();
    for (int i=0; i < ui->TabWire->rowCount(); i++) {
        temp.append(ui->TabWire->item(i, 0)->backgroundColor().name());
        temp.append(ui->TabWire->item(i, 1)->backgroundColor().name());
    }
    ini->setValue("WireColor", temp.join(" "));

    ini->setValue("TestNG", QString::number(ui->BoxTestNG->currentIndex()));
    ini->setValue("WinType", QString::number(ui->BoxType->currentIndex()));
    ini->sync();


    qDebug() << QTime::currentTime().toString() << "WinType save OK";
}

void WinType::SelectWireColor(int row, int column)
{
    if (CurrentUser() == 0) {
        SendWarnning(tr("用户权限不足"));
        return;
    }
    ui->Type->setCurrentIndex(0);
    QColor color = ui->TabColor->item(row, column)->backgroundColor();
    if (ui->TabWire->currentRow()  <  0)
        return;
    ui->TabWire->currentItem()->setBackgroundColor(color);
    ui->TabWire->currentItem()->setSelected(false);
    ui->TabColor->currentItem()->setSelected(false);
}

void WinType::ShowWireColorWindow()
{
    if (CurrentUser() == 0) {
        SendWarnning(tr("用户权限不足"));
        return;
    }
    ui->Type->setCurrentIndex(1);
}

void WinType::ShowAvailableItem(int, int)
{
    if (CurrentUser() == 0) {
        SendWarnning(tr("用户权限不足"));
        return;
    }
    ui->Other->setCurrentIndex(1);
}

void WinType::JumptoSetWindows()
{
    if (ui->TabTest->currentRow()  <  0)
        return;
    if (CurrentUser() == 0) {
        SendWarnning(tr("用户权限不足"));
        return;
    }
    SaveSettings();
    if (ui->TabTest->currentItem()->text() == tr("电阻"))
        emit SendCommand(ADDR, CMD_JUMP, "PageDcr");
    if (ui->TabTest->currentItem()->text() == tr("反嵌"))
        emit SendCommand(ADDR, CMD_JUMP, "PageMag");
    if (ui->TabTest->currentItem()->text() == tr("绝缘"))
        emit SendCommand(ADDR, CMD_JUMP, "PageInr");
    if (ui->TabTest->currentItem()->text() == tr("交耐"))
        emit SendCommand(ADDR, CMD_JUMP, "PageAcw");
    if (ui->TabTest->currentItem()->text() == tr("直耐"))
        emit SendCommand(ADDR, CMD_JUMP, "PageDcw");
    if (ui->TabTest->currentItem()->text() == tr("匝间"))
        emit SendCommand(ADDR, CMD_JUMP, "PageImp");
    if (ui->TabTest->currentItem()->text() == tr("电感"))
        emit SendCommand(ADDR, CMD_JUMP, "PageInd");
    if (ui->TabTest->currentItem()->text() == tr("功率"))
        emit SendCommand(ADDR, CMD_JUMP, "PagePwr");
    if (ui->TabTest->currentItem()->text() == tr("低启"))
        emit SendCommand(ADDR, CMD_JUMP, "PageLvs");
    if (ui->TabTest->currentItem()->text() == tr("堵转"))
        emit SendCommand(ADDR, CMD_JUMP, "PageLck");
}

void WinType::CopySettings(QString name)
{
    if (name.isEmpty())
        return;
    QString c = ui->TextTypeShow->text();
    if (name.isEmpty())
        return;
    for (int i=0; i < ui->TabFile->rowCount(); i++) {
        if (ui->TabFile->item(i, 0)->text() == name)
            return;
    }
    QString Source = QString("./config/%1.ini").arg(c);
    QString Target = QString("./config/%1.ini").arg(name);
    QFile *s = new QFile(Source);
    s->copy(Target);
    name.append(".ini");
    QSettings *ini = new QSettings(INI_PATH, QSettings::IniFormat);
    ini->setIniCodec("GB18030");
    ini->beginGroup("GLOBAL");
    ini->setValue("FileInUse", name);
}

void WinType::RemoveSettings()
{
    if (ui->TabFile->currentRow()  <  0)
        return;
    QString t = ui->TabFile->currentItem()->text();
    if (t == "aip9918")
        return;
    if (t == ui->TextTypeShow->text()) {
        ui->TextTypeShow->setText("aip9918");
        ChangeSettings();
        InitSettings();
    }
    QString Target = QString("./config/%1.ini").arg(t);
    QFile::remove(Target);
}

void WinType::ChangeSettings()
{
    if (ui->TabFile->currentRow()  <  0) {
        qDebug() << QTime::currentTime().toString() << "调入电机型号Error";
        return;
    }
    QString t = ui->TextTypeShow->text();
    t.append(".ini");
    QSettings *ini = new QSettings(INI_PATH, QSettings::IniFormat);
    ini->setIniCodec("GB18030");
    ini->beginGroup("GLOBAL");
    ini->setValue("FileInUse", t);
}

void WinType::QuerySettings()
{
    QString t = ui->EditTypeName->text();
    if (t.isEmpty())
        return;
    for (int i=0; i < FileNames.size(); i++) {
        if (FileNames.at(i).contains(t))
            FileNames.move(i, 0);
    }
    for (int i=0; i < qMin(FileNames.size(), ui->TabFile->rowCount()); i++) {
        ui->TabFile->item(i, 0)->setText(QString(FileNames.at(i)).remove(".ini"));
    }
    ui->TabFile->verticalScrollBar()->setValue(0);
}

QString WinType::CurrentSettings()
{
    QSettings *ini = new QSettings(INI_PATH, QSettings::IniFormat);
    QString n = ini->value("/GLOBAL/FileInUse", INI_DEFAULT).toString();
    return n.remove(".ini");
}

QStringList WinType::CurrentItems()
{
    QString n = QString("./config/%1.ini").arg(CurrentSettings());
    QSettings *ini = new QSettings(n, QSettings::IniFormat);
    QString s = ini->value("/GLOBAL/ProjToTest", "1").toString();
    return s.split(" ");
}

QStringList WinType::EnableItems()
{
    QSettings *ini = new QSettings(INI_PATH, QSettings::IniFormat);
    QString n = ini->value("/GLOBAL/ItemEnable", INI_DEFAULT).toString();
    return n.split(" ");
}

int WinType::CurrentUser()
{
    QSettings *ini = new QSettings(INI_PATH, QSettings::IniFormat);
    return ini->value("/GLOBAL/User", "0").toInt();
}

void WinType::showEvent(QShowEvent *e)
{
    ui->Other->setCurrentIndex(0);
    ui->Type->setCurrentIndex(0);
    ReadAllSettings();
    ReadAvailableItems();
    InitSettings();
    e->accept();
}

void WinType::on_TabFile_cellClicked(int , int )
{
    ui->TextTypeShow->setText(ui->TabFile->currentItem()->text());
    ChangeSettings();
    InitSettings();
}

void WinType::SendWarnning(QString s)
{
    QVariantHash hash;
    hash.insert("TxAddress", "WinHome");
    hash.insert("TxCommand", "Warnning");
    hash.insert("TxMessage", tr("操作异常:\n%1").arg(s));
    emit SendVariant(QVariant::fromValue(hash));
}
/*********************************END OF FILE**********************************/


