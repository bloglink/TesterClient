/*******************************************************************************
 * Copyright [2016]   <青岛艾普智能仪器有限公司>
 * All rights reserved.
 *
 * version:     2.1.0.170424
 * author:      zhaonanlin
 * brief:       数据管理模块
*******************************************************************************/
#include "WinData.h"
#include "ui_WinData.h"

WinData::WinData(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::WinData)
{
    ui->setupUi(this);
    this->setObjectName("DataPage");
    InitWindows();
    InitButtons();
    InitSqlite();
}

WinData::~WinData()
{
    delete ui;
}

void WinData::InitWindows()
{
#if (QT_VERSION <= QT_VERSION_CHECK(5, 0, 0))
    ui->TabTestData->horizontalHeader()->setResizeMode(QHeaderView::Stretch);
    ui->TabDetail->horizontalHeader()->setResizeMode(QHeaderView::Stretch);
    ui->TabDetail->verticalHeader()->setResizeMode(QHeaderView::Stretch);
#else
    ui->TabTestData->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->TabDetail->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->TabDetail->verticalHeader()->setSectionResizeMode(QHeaderView::Stretch);
#endif
}

void WinData::InitButtons()
{
    QButtonGroup *BtnGroup = new QButtonGroup;
    BtnGroup->addButton(ui->BtnExit, Qt::Key_0);
    BtnGroup->addButton(ui->BtnShow, Qt::Key_1);
    BtnGroup->addButton(ui->BtnCheck, Qt::Key_2);
    BtnGroup->addButton(ui->BtnClear, Qt::Key_3);
    connect(BtnGroup, SIGNAL(buttonClicked(int)), this, SLOT(ReadButtons(int)));
}

void WinData::ReadButtons(int id)
{
    switch (id) {
    case Qt::Key_0:
        emit buttonClicked(NULL);
        emit SendCommand(ADDR, CMD_JUMP, NULL);
        break;
    case Qt::Key_1:
        ExportToUsb();
        break;
    case Qt::Key_2:
        ReadSqliteAll();
        break;
    case Qt::Key_3:
        ClearSqlite();
        break;
    default:
        break;
    }
}

void WinData::InitSqlite()
{
    QFile file(SQL_PATH);
    if (!file.exists()) {
        file.open(QIODevice::ReadWrite);
        file.close();
    }
    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName(SQL_PATH);
    if (!db.open()) {
        qDebug() << QTime::currentTime().toString() << "Sql Error";
        return;
    }

    QSqlQuery query(db);
    query.prepare("select count(*) from sqlite_master where type='table' and name='TestData'");
    query.exec();
    if (query.next() && query.value(0).toInt() == 0) {
        QString cmd = "create table TestData (id int primary key, item text, para text,";
        cmd += " result text, judge text, b1 text, b2 text, b3 text)";
        query.prepare(cmd);
        query.exec();
    }
//    model = new QSqlTableModel(this);
//    model->setTable("TestData");
//    model->setEditStrategy(QSqlTableModel::OnManualSubmit);
//    ui->TabTestData->setModel(model);
}

void WinData::ReadSqliteAll()
{
    ui->TabDetail->setRowCount(0);
    ReadSqlite(tr("总数"));
    ReadSqlite(tr("电阻"));
    ReadSqlite(tr("反嵌"));
    ReadSqlite(tr("磁旋"));
    ReadSqlite(tr("绝缘"));
    ReadSqlite(tr("交耐"));
    ReadSqlite(tr("匝间"));
    ReadSqlite(tr("电感"));
    ReadSqlite(tr("功率"));
    ReadSqlite(tr("PG"));
    ReadSqlite(tr("低启"));
    ReadSqlite(tr("堵转"));
    QStringList n;
    for (int i=0; i < ui->TabDetail->rowCount(); i++)
        n.append(ui->TabDetail->item(i, 0)->text());
    QStringList v;
    for (int i=0; i < ui->TabDetail->rowCount(); i++)
        v.append(ui->TabDetail->item(i, 4)->text());
    ui->TextHistogram->setNames(n);
    ui->TextHistogram->setValues(v);
    ui->TextHistogram->update();

    qDebug() << QTime::currentTime().toString() << "WinData read OK";
}

void WinData::ReadSqlite(QString item)
{
    QSqlQuery query(db);
    if (ui->EditType->text().isEmpty()) {
        query.prepare("select judge from TestData where para>=:d1 and para<=:d2 and item=:item");
        query.bindValue(":d1", ui->DateEdit1->date());
        query.bindValue(":d2", ui->DateEdit2->date());
        query.bindValue(":item", item);
    } else {
        QString cmd = "select judge from TestData where para>=:d1 and para<=:d2";
        cmd += " and item=:item and result=:type";
        query.prepare(cmd);
        query.bindValue(":d1", ui->DateEdit1->date());
        query.bindValue(":d2", ui->DateEdit2->date());
        query.bindValue(":item", item);
        query.bindValue(":type", ui->EditType->text());
    }
    query.exec();
    int t1 = 0;
    int t2 = 0;
    while (query.next()) {
        if (query.value(0).toString() == "OK")
            t1++;
        if (query.value(0).toString() == "NG")
            t2++;
    }
    if (t1 == 0 && t2 == 0)
        return;
    int row = ui->TabDetail->rowCount();
    ui->TabDetail->setRowCount(row+1);
    for (int i=0; i < 5; i++) {
        ui->TabDetail->setItem(row, i, new QTableWidgetItem);
        ui->TabDetail->item(row, i)->setTextAlignment(Qt::AlignCenter);
    }
    ui->TabDetail->item(row, 0)->setText(item);
    ui->TabDetail->item(row, 1)->setText(QString::number(t1+t2));
    ui->TabDetail->item(row, 2)->setText(QString::number(t1));
    ui->TabDetail->item(row, 3)->setText(QString::number(t2));
    ui->TabDetail->item(row, 4)->setText(QString::number(t2*100/(t1+t2)));
    ui->TabDetail->item(row, 4)->setText(QString("%1%").arg(t1*100/(t1+t2)));
}

void WinData::ClearSqlite()
{
    QSqlQuery query(db);
    query.prepare("delete from TestData");
    query.exec();
    ReadSqliteAll();
}

void WinData::ExportToUsb()
{
    QString path = ReadUsb();
    if (path.isEmpty()) {
        QMessageBox::warning(this,  "",   "请插入U盘",  QMessageBox::Ok);
        return;
    }
    QFile file(QString("%1/%2.csv").arg(path).arg(QDate::currentDate().toString("yy-MM-dd")));
    if (!file.open(QFile::WriteOnly)) {
        QMessageBox::warning(this,  "",   "创建失败",  QMessageBox::Ok);
        return;
    }
    QSqlQuery query(db);
    query.prepare("select item, para, result, judge from TestData");
    query.exec();
    int i = 0;
    while (query.next()) {
        file.write(ToGbk(query.value(0).toString().replace(",", " ")));
        file.write(",");
        file.write(ToGbk(query.value(1).toString().replace(",", " ")));
        file.write(",");
        file.write(ToGbk(query.value(2).toString().replace(",", " ")));
        file.write(",");
        file.write(ToGbk(query.value(3).toString().replace(",", " ")));
        file.write("\n");
        i++;
        if (i%10 == 1)
            ui->label_2->setText(QString::number(i));
    }
    file.close();
    QMessageBox::information(this,  "",   "导出成功",  QMessageBox::Ok);
}

QString WinData::ReadUsb()
{
    QDir *dir = new QDir;
    dir->setPath("/mnt/usb1");
    if (dir->count() > 2)
        return "/mnt/usb1";
    dir->setPath("/mnt/usb2");
    if (dir->count() > 2)
        return "/mnt/usb2";
    return NULL;
}

QByteArray WinData::ToGbk(const QString &inStr)
{
    QTextCodec *gbk = QTextCodec::codecForName("GB18030");
    return gbk->fromUnicode(inStr);
}

void WinData::showEvent(QShowEvent *e)
{
    ui->DateEdit1->setDate(QDate::currentDate());
    ui->DateEdit2->setDate(QDate::currentDate());
    ReadSqliteAll();
    e->accept();
}
/*********************************END OF FILE**********************************/
