/*******************************************************************************
 * Copyright [2017]   <青岛艾普智能仪器有限公司>
 * All rights reserved.
 *
 * version:     0.1
 * author:      zhaonanlin
 * brief:       数据管理
*******************************************************************************/
#include "page_sqlite.h"

PageSqlite::PageSqlite(QWidget *parent) : QWidget(parent)
{
    initUI();
    initSql();
    initSqlTableModel();
}

PageSqlite::~PageSqlite()
{
}

void PageSqlite::initUI()
{
    this->setObjectName("DataPage");
    view = new QTableView(this);
    view->setSelectionMode(QAbstractItemView::SingleSelection);
    view->setSelectionBehavior(QAbstractItemView::SelectRows);
    view->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    connect(view, SIGNAL(clicked(QModelIndex)), this, SLOT(readViews(QModelIndex)));

    views = new QTableView(this);
    views->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    QHBoxLayout *viewLayout = new QHBoxLayout;
    viewLayout->addWidget(view);
    viewLayout->addWidget(views);

    date = new QDateEdit(this);
    date->setDisplayFormat("yyyy-MM-dd");
    date->setDate(QDate::currentDate());

    QPushButton *btnQuery = new QPushButton(this);
    btnQuery->setText(tr("查询"));
    btnQuery->setMinimumSize(97, 35);
    connect(btnQuery, SIGNAL(clicked(bool)), this, SLOT(querySql()));

    QPushButton *btnExport = new QPushButton(this);
    btnExport->setText(tr("导出"));
    btnExport->setMinimumSize(97, 35);
    connect(btnExport, SIGNAL(clicked(bool)), this, SLOT(exportSql()));

    QPushButton *btnClear = new QPushButton(this);
    btnClear->setText(tr("清空"));
    btnClear->setMinimumSize(97, 35);
    connect(btnClear, SIGNAL(clicked(bool)), this, SLOT(clearSql()));

    QPushButton *btnExit = new QPushButton(this);
    btnExit->setText(tr("退出"));
    btnExit->setMinimumSize(97, 35);
    connect(btnExit, SIGNAL(clicked(bool)), this, SLOT(saveData()));

    QHBoxLayout *btnLayout = new QHBoxLayout;
    btnLayout->addWidget(new QLabel("测试日期", this));
    btnLayout->addWidget(date);
    btnLayout->addWidget(btnQuery);
    btnLayout->addWidget(btnExport);
    btnLayout->addWidget(btnClear);
    btnLayout->addStretch();
    btnLayout->addWidget(btnExit);

    QVBoxLayout *layout = new QVBoxLayout;
    layout->addLayout(viewLayout);
    layout->addLayout(btnLayout);
    this->setLayout(layout);
}

void PageSqlite::initSql()
{
    QFile file("./nandflash/aip.db");
    if (!file.exists()) {
        file.open(QIODevice::ReadWrite);
        file.close();
    }

    db = QSqlDatabase::addDatabase("QSQLITE","SQL");
    db.setDatabaseName("./nandflash/aip.db");
    if (!db.open())
        qDebug() << QTime::currentTime().toString() << "open sql Error";
    initTable();
}

void PageSqlite::initTable()
{
    QSqlQuery query(db);
    QString s = "create table if not exists TestData";
    s += " (id integer primary key, date text, time text, type text, code text, user text, judge text)";
    if (!query.exec(s))
        qDebug() << QTime::currentTime().toString() << "create table TestData Error";

    s = "create table if not exists TestDatas";
    s += " (id integer primary key, parent integer, item text, para text, result text, judge text)";
    if (!query.exec(s))
        qDebug() << QTime::currentTime().toString() << "create table TestDatas Error";
}

void PageSqlite::initSqlTableModel()
{
    mView = new SqlTableModel(this, db);
    mView->setTable("TestData");
    mView->select();
    QStringList header;
    header << tr("ID") << tr("测试日期") << tr("测试时间")
           << tr("测试型号") << tr("产品编码") << tr("测试人") << tr("测试判定");
    for (int i=0; i < header.size(); i++)
        mView->setHeaderData(i, Qt::Horizontal, header.at(i));

    view->setModel(mView);
    view->hideColumn(0);

    mViews = new SqlTableModel(this, db);
    mViews->setTable("TestDatas");
    mViews->select();
    QStringList headers;
    headers << tr("ID") << tr("父ID") << tr("项目") << tr("参数") << tr("结果") << tr("判定");
    for (int i=0; i < headers.size(); i++)
        mViews->setHeaderData(i, Qt::Horizontal, headers.at(i));

    views->setModel(mViews);
    views->hideColumn(0);
    views->hideColumn(1);
}

void PageSqlite::saveSql(QJsonObject obj)
{
    QSqlQuery query(db);
    quint64 uuid = snow.getId();
    QStringList title = obj.value("title").toString().split("@");
    if (title.size() >= 3) {
        query.prepare("insert into TestData values(?, ?, ?, ?, ?, ?, ?)");
        query.bindValue(0, uuid);
        query.bindValue(1, QDate::currentDate().toString("yyyy-MM-dd"));
        query.bindValue(2, QTime::currentTime().toString("hh:mm:ss"));
        query.bindValue(3, title.at(0));
        query.bindValue(4, title.at(1));
        query.bindValue(5, title.at(2));
        query.bindValue(6, title.at(3));
        query.exec();
    }
    QStringList content = obj.value("content").toString().split("\n");
    for (int i=0; i < content.size(); i++) {
        QStringList temp = QString(content.at(i)).split("@");
        if (temp.size() >= 4) {
            query.prepare("insert into TestDatas values(?, ?, ?, ?, ?, ?)");
            query.bindValue(0, snow.getId());
            query.bindValue(1, uuid);
            query.bindValue(2, temp.at(0));
            query.bindValue(3, temp.at(1));
            query.bindValue(4, temp.at(2));
            query.bindValue(5, temp.at(3));
            query.exec();
        }
    }
}

void PageSqlite::readViews(QModelIndex index)
{
    int row = index.row();
    quint64 id = mView->index(row, 0).data().toDouble();
    mViews->setFilter(QObject::tr("parent=%1").arg(id));
    mViews->select();
}

void PageSqlite::saveData()
{
    emit buttonClicked(NULL);
}

void PageSqlite::querySql()
{
    mView->setFilter(QObject::tr("date='%1'").arg(date->date().toString("yyyy-MM-dd")));
    mView->select();
}

void PageSqlite::clearSql()
{
    db.close();
    db.open();
    QSqlQuery query(db);
    if (!query.exec("drop table TestData"))
        qDebug() << query.lastError();
    if (!query.exec("drop table TestDatas"))
        qDebug() << query.lastError();
    initTable();
    initSqlTableModel();
}

void PageSqlite::exportSql()
{
    QString name = getSaveFileName();
    QFile file(QString("%1.csv").arg(name));
    if (!file.open(QFile::WriteOnly)) {
        QMessageBox::warning(this,  "",   "创建失败",  QMessageBox::Ok);
        return;
    }
    QSqlQuery query(db);
    QString cmd;
    cmd = "select TestData.id, TestData.date, ";
    cmd += "TestData.time, TestData.type, TestData.code, ";
    cmd += "TestData.user, TestData.judge, ";
    cmd += "TestDatas.item, TestDatas.para, ";
    cmd += "TestDatas.result, TestDatas.judge ";
    cmd += "from TestData ";
    cmd += "INNER JOIN TestDatas ON TestData.id = TestDatas.parent";
    query.exec(cmd);
    QStringList header;
    header << tr("测试日期") << tr("测试时间") << tr("测试型号")
           << tr("产品编码") << tr("测试人") << tr("测试判定");
    for (int i=0; i < header.size(); i++) {
        file.write(ToGbk(header.at(i)));
        file.write(",");
    }
    QStringList headers;
    headers << tr("测试项目") << tr("测试参数") << tr("测试结果") << tr("测试判定");
    for (int i=0; i < 5; i++) {
        for (int j=0; j < headers.size(); j++) {
            file.write(ToGbk(headers.at(i%4)));
            file.write(",");
        }
    }

    while (query.next()) {
        double id = query.value(0).toDouble();
        if (current_id != id) {
            file.write("\n");
            file.write(ToGbk(query.value(1).toString().replace(",", " ")));
            file.write(",");
            file.write(ToGbk(query.value(2).toString().replace(",", " ")));
            file.write(",");
            file.write(ToGbk(query.value(3).toString().replace(",", " ")));
            file.write(",");
            file.write(ToGbk(query.value(4).toString().replace(",", " ")));
            file.write(",");
            file.write(ToGbk(query.value(5).toString().replace(",", " ")));
            file.write(",");
            file.write(ToGbk(query.value(6).toString().replace(",", " ")));
            file.write(",");
        } else {
            // nothing
        }
        file.write(ToGbk(query.value(7).toString().replace(",", " ")));
        file.write(",");
        file.write(ToGbk(query.value(8).toString().replace(",", " ")));
        file.write(",");
        file.write(ToGbk(query.value(9).toString().replace(",", " ")));
        file.write(",");
        file.write(ToGbk(query.value(10).toString().replace(",", " ")));
        file.write(",");

        current_id = id;
    }
    file.close();
    QMessageBox::information(this,  "",   "导出成功",  QMessageBox::Ok);
}

QByteArray PageSqlite::ToGbk(const QString &inStr)
{
    QTextCodec *gbk = QTextCodec::codecForName("GB18030");
    return gbk->fromUnicode(inStr);
}

QString PageSqlite::getSaveFileName()
{
    QString fileName = QFileDialog::getSaveFileName(this,
                                                    tr("Open Config"),
                                                    QDate::currentDate().toString("yy-MM-dd"),
                                                    tr("Config Files (*.csv)"));
    return fileName;
}
