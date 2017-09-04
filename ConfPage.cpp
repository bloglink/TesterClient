/*******************************************************************************
 * Copyright [2017]   <青岛艾普智能仪器有限公司>
 * All rights reserved.
 *
 * version:     0.1
 * author:      zhaonanlin
 * brief:       型号管理
*******************************************************************************/
#include "ConfPage.h"

ConfPage::ConfPage(QWidget *parent) : QWidget(parent)
{
    initUI();
}

ConfPage::~ConfPage()
{
}

void ConfPage::initTypes(QString dat)
{
    QStringList names = dat.split(" ");
    mView->setRowCount(0);
    for (int i=0; i < names.size(); i++) {
        mView->appendRow(new QStandardItem(names.at(i)));
    }
}

void ConfPage::initOther(QString dat)
{
    QDomDocument docs;
    docs.setContent(dat);
    if (!docs.elementsByTagName("Conf").isEmpty()) {
        QDomNodeList list = docs.elementsByTagName("Conf").at(0).childNodes();
        for (int i=0; i < list.size(); i++) {
            QDomElement dom = list.at(i).toElement();
            QStringList temp = dom.text().split(",");
            if (dom.nodeName() == "color") {
                for (int i=0; i < temp.size(); i++) {
                    colors.at(i)->setStyleSheet(QString("background-color:%1").arg(temp.at(i)));
                }
            }
            if (dom.nodeName() == "type") {
                typeComboBox->setCurrentText(temp.at(0));
            }
        }
    }
    if (!docs.elementsByTagName("Sys").isEmpty()) {
        QDomNodeList list = docs.elementsByTagName("Sys").at(0).childNodes();
        for (int i=0; i < list.size(); i++) {
            QDomElement dom = list.at(i).toElement();
            QStringList temp = dom.text().split(",");
            if (dom.nodeName() == "Test_Item") {
                pModel->setRowCount(0);
                testItem = temp;
                for (int i=0; i < temp.size(); i++) {
                    pModel->appendRow(new QStandardItem(btnNames.at(temp.at(i).toInt()-1)));
                }
                pModel->appendRow(new QStandardItem);
            }
        }
    }
}

QStringList ConfPage::testItems()
{
    return testItem;
}

void ConfPage::readSettings()
{
    //当前使用的测试项目
    QString t = QString("./config/%1.ini").arg(CurrentSettings());
    QSettings *ini = new QSettings(t, QSettings::IniFormat);
    ini->setIniCodec("GB18030");
    ini->beginGroup("Conf");

    QStringList temp = ini->value("color").toString().split(",");
    for (int i=0; i < temp.size(); i++) {
        colors.at(i)->setStyleSheet(QString("background-color:%1").arg(temp.at(i)));
    }

    temp = ini->value("type").toString().split(",");
    typeComboBox->setCurrentText(temp.at(0));
    ini->endGroup();
    ini->beginGroup("Sys");
    temp = ini->value("Test_Item", "1").toString().split(",");
    pModel->setRowCount(0);
    testItem = temp;
    for (int i=0; i < temp.size(); i++) {
        pModel->appendRow(new QStandardItem(btnNames.at(temp.at(i).toInt()-1)));
    }
    pModel->appendRow(new QStandardItem);
    qDebug() << temp;
}

void ConfPage::initUI()
{
    this->setObjectName("ConfPage");
    QStringList headers;
    headers << tr("电机型号");
    mView = new StandardItem(0, headers.size());
    mView->setHorizontalHeaderLabels(headers);

    view = new QTableView(this);
    view->setModel(mView);
    view->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    connect(view, SIGNAL(clicked(QModelIndex)), this, SLOT(updateType()));

    typeLineEdit = new QLineEdit(this);
    typeLineEdit->setMinimumSize(97, 35);
    typeLineEdit->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);

    QPushButton *btnAppend = new QPushButton(this);
    btnAppend->setText(tr("添加型号"));
    btnAppend->setMinimumSize(97, 35);
    connect(btnAppend, SIGNAL(clicked(bool)), this, SLOT(appendType()));
    QPushButton *btnRemove = new QPushButton(this);
    btnRemove->setText(tr("删除型号"));
    btnRemove->setMinimumSize(97, 35);
    connect(btnRemove, SIGNAL(clicked(bool)), this, SLOT(deleteType()));
    QPushButton *btnExit = new QPushButton(this);
    btnExit->setText(tr("保存退出"));
    btnExit->setMinimumSize(97, 35);
    connect(btnExit, SIGNAL(clicked(bool)), this, SLOT(saveSettings()));

    QHBoxLayout *btnLayout = new QHBoxLayout;
    btnLayout->addWidget(new QLabel("型号名称", this));
    btnLayout->addWidget(typeLineEdit);
    btnLayout->addWidget(btnAppend);
    btnLayout->addWidget(btnRemove);
    btnLayout->addStretch();
    btnLayout->addWidget(btnExit);

    QVBoxLayout *mLayout = new QVBoxLayout;
    mLayout->addWidget(view);
    mLayout->addLayout(btnLayout);

    QGroupBox *mGroup = new QGroupBox(this);
    mGroup->setLayout(mLayout);

    QStringList typeNames;
    typeNames << "M1S0" << "M1S1" << "M1S2" << "M1S3"
              << "M1S1L1" << "M1S1L2" << "M1S1L3"
              << "ABC" << "ABC-N" << "ABC-O" << "ABC-Y"
              << "None";
    typeComboBox = new QComboBox(this);
    typeComboBox->addItems(typeNames);
    typeComboBox->setMinimumSize(97, 35);
    typeComboBox->setView(new QListView);
    connect(typeComboBox, SIGNAL(currentIndexChanged(QString)), this, SLOT(autoPixmap(QString)));
    typePixmap = new QLabel(this);
    typePixmap->setPixmap(QPixmap(":/source/M1S0.jpg"));

    QGridLayout *cLayout = new QGridLayout;
    for (int i=0; i < 8; i++) {
        QPushButton *btn = new QPushButton(QString::number(i+1));
        colors.append(btn);
        cLayout->addWidget(btn, i/2, i%2);
        connect(btn, SIGNAL(clicked(bool)), this, SLOT(selectColor()));
    }

    QGridLayout *tLayout = new QGridLayout;
    tLayout->addWidget(new QLabel("电机类型", this), 0, 0);
    tLayout->addWidget(typeComboBox, 0, 1);
    tLayout->addWidget(typePixmap, 1, 0, 1, 2);
    tLayout->addWidget(new QLabel("线夹颜色", this), 2, 0);
    tLayout->addLayout(cLayout, 3, 0, 1, 2);
    tLayout->setColumnStretch(1, 1);
    tLayout->setRowStretch(4, 1);

    QGroupBox *tGroup = new QGroupBox(this);
    tGroup->setLayout(tLayout);

    QStringList pHeaders;
    pHeaders << tr("测试项目");
    pModel = new StandardItem(0, pHeaders.size());
    pModel->setHorizontalHeaderLabels(pHeaders);
    pModel->appendRow(new QStandardItem(""));

    pView = new QTableView(this);
    pView->setModel(pModel);
    pView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    btnHide = new QToolButton(this);
    btnHide->setIcon(QIcon(":/icons/left.png"));
    btnHide->setIconSize(QSize(30, 30));
    btnHide->setFocusPolicy(Qt::NoFocus);
    btnHide->setText(tr("测\n试\n管\n理"));
    btnHide->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
    connect(btnHide, SIGNAL(clicked(bool)), this, SLOT(showButtons()));

    QVBoxLayout *hideLayout = new QVBoxLayout;
    hideLayout->addWidget(btnHide);
    hideLayout->addStretch();

    QVBoxLayout *btnsLayout = new QVBoxLayout;

    btnNames << "电阻" << "反嵌" << "绝缘" << "交耐" << "直耐"
             << "匝间" << "电参" << "电感" << "堵转" << "低启"
             << "霍尔" << "负载" << "空载" << "BEMF";
    QStringList objNames;
    objNames << "ConfResistance" << "ConfigMAG" << "ConfInsulation" << "ConfCurrent_AC" << "x"
             << "ConfigIMP" << "x" << "ConfInductance" << "x" << "x" << "ConfHall"
             << "ConfLoadTesting" << "ConfNoLoadTest"  << "ConfBackForce";
    for (int i=0; i < btnNames.size(); i++) {
        buttons.append(new QPushButton(btnNames.at(i), this));
        btnsLayout->addWidget(buttons.at(i));
        buttons.at(i)->setMinimumSize(97, 44);
        buttons.at(i)->setObjectName(objNames.at(i));
        buttons.at(i)->setCheckable(true);
        connect(buttons.at(i), SIGNAL(clicked(bool)), this, SLOT(clickButton()));
    }
    buttons.at(1)->hide();
    buttons.at(4)->hide();
    buttons.at(5)->hide();
    buttons.at(6)->hide();
    buttons.at(8)->hide();
    buttons.at(9)->hide();

    QPushButton *btnDelete = new QPushButton("删除项目", this);
    btnDelete->setMinimumSize(97, 35);
    connect(btnDelete, SIGNAL(clicked(bool)), this, SLOT(deleteItem()));
    btnsLayout->addWidget(btnDelete);
    QPushButton *btnConfig = new QPushButton("配置项目", this);
    btnConfig->setMinimumSize(97, 35);
    connect(btnConfig, SIGNAL(clicked(bool)), this, SLOT(windowChange()));
    btnsLayout->addWidget(btnConfig);
    btnsLayout->addStretch();
    btnsLayout->setMargin(0);

    btnFrame = new QFrame(this);
    btnFrame->setLayout(btnsLayout);
    btnFrame->hide();

    QHBoxLayout *pLayout = new QHBoxLayout;
    pLayout->addWidget(pView);
    pLayout->addLayout(hideLayout);
    pLayout->addWidget(btnFrame);

    QGroupBox *pGroup = new QGroupBox(this);
    pGroup->setLayout(pLayout);

    QHBoxLayout *layout = new QHBoxLayout;
    layout->addStretch();
    layout->addWidget(tGroup);
    layout->addWidget(pGroup);
    layout->addWidget(mGroup);
    layout->addStretch();
    this->setLayout(layout);
}

void ConfPage::saveSettings()
{
    emit sendNetMsg("6004 Conf");
    //当前使用的测试项目
    QString t = QString("./config/%1.ini").arg(CurrentSettings());
    QSettings *ini = new QSettings(t, QSettings::IniFormat);
    ini->setIniCodec("GB18030");
    ini->beginGroup("Conf");

    QDomText text;
    QDomDocument doc;
    QDomElement root = doc.createElement("Conf");
    doc.appendChild(root);

    QStringList temp;
    for (int i=0; i < colors.size(); i++) {
        QPalette palette = colors.at(i)->palette();
        temp.append(palette.color(QPalette::Background).name());
    }
    QDomElement color = doc.createElement("color");
    root.appendChild(color);
    text = doc.createTextNode(temp.join(","));
    color.appendChild(text);
    ini->setValue("color", temp.join(","));

    temp.clear();
    temp.append(typeComboBox->currentText());
    QDomElement type = doc.createElement("type");
    root.appendChild(type);
    text = doc.createTextNode(temp.join(","));
    type.appendChild(text);
    ini->setValue("type", temp.join(","));

    emit sendNetMsg((doc.toByteArray()).insert(0, "6002 "));
    emit buttonClicked(NULL);
}

void ConfPage::saveSys()
{
    emit sendNetMsg("6004 Sys");
    //当前使用的测试项目
    QString t = QString("./config/%1.ini").arg(CurrentSettings());
    QSettings *ini = new QSettings(t, QSettings::IniFormat);
    ini->setIniCodec("GB18030");
    ini->beginGroup("Sys");

    QDomText text;
    QDomDocument doc;
    QDomElement root = doc.createElement("Sys");
    doc.appendChild(root);

    QStringList temp;
    for (int i=0; i < pModel->rowCount(); i++) {
        for (int t=0; t < btnNames.size(); t++) {
            if (btnNames.at(t) == pModel->item(i, 0)->text())
                temp.append(QString::number(t+1));
        }
    }
    QDomElement item = doc.createElement("Test_Item");
    root.appendChild(item);
    text = doc.createTextNode(temp.join(","));
    item.appendChild(text);
    ini->setValue("Test_Item", temp.join(","));

    emit sendNetMsg((doc.toByteArray()).insert(0, "6002 "));
}

void ConfPage::clickButton()
{
    QPushButton *btn = qobject_cast<QPushButton *>(sender());
    for (int i=0; i < buttons.size(); i++) {
        buttons.at(i)->setChecked(false);
    }
    btn->setChecked(true);
    for (int i=0; i < pModel->rowCount(); i++) {
        if (pModel->item(i, 0)->text() == btn->text())
            return;
    }
    if (pView->currentIndex().row() < 0 || (pView->currentIndex().row()+1 == pModel->rowCount())) {
        pModel->appendRow(new QStandardItem(""));
        if (pModel->rowCount() > 1)
            pModel->item(pModel->rowCount()-2, 0)->setText(btn->text());
    } else {
        pModel->item(pView->currentIndex().row(), 0)->setText(btn->text());
    }
    saveSys();
}

void ConfPage::showButtons()
{
    if (btnFrame->isHidden()) {
        btnFrame->show();
        btnHide->setIcon(QIcon(":/icons/right.png"));
    } else {
        btnFrame->hide();
        btnHide->setIcon(QIcon(":/icons/left.png"));
    }
}

void ConfPage::windowChange()
{
    for (int i=0; i < buttons.size(); i++) {
        if (buttons.at(i)->isChecked())
            emit buttonClicked(buttons.at(i)->objectName().toUtf8());
    }
}

void ConfPage::selectColor()
{
    QPushButton *btn = qobject_cast<QPushButton *>(sender());
    QColor color = QColorDialog::getColor(Qt::white, this);
    if (color.isValid())
        btn->setStyleSheet(QString("background-color:%1").arg(color.name()));
}

void ConfPage::deleteItem()
{
    int row = pView->currentIndex().row();
    if (row < 0)
        return;
    if (row+1 == pModel->rowCount())
        return;
    pModel->removeRow(row);
    saveSys();
}

void ConfPage::autoPixmap(QString name)
{
    QString pixmap = QString(":/source/%1.jpg").arg(name);
    typePixmap->setPixmap(QPixmap(pixmap));
}

void ConfPage::recvAppShow(QString win)
{
    if (win != this->objectName())
        return;
    queryType();
    readSettings();

}

void ConfPage::appendType()
{
    QString name = typeLineEdit->text();
    if (name.isEmpty())
        return;
    QString c = mView->item(0,0)->text();
    QString Source = QString("./config/%1.ini").arg(c);
    QString Target = QString("./config/%1.ini").arg(name);
    QFile *s = new QFile(Source);
    s->copy(Target);
    s->close();
    QSettings *ini = new QSettings("./nandflash/global.ini", QSettings::IniFormat);
    ini->setIniCodec("GB18030");
    ini->beginGroup("GLOBAL");
    ini->setValue("FileInUse", name);

    queryType();
}

void ConfPage::deleteType()
{
    QString name = mView->item(0, 0)->text();

    QString Target = QString("./config/%1.ini").arg(name);
    QFile::remove(Target);

    queryType();
}

void ConfPage::updateType()
{
    int row = view->currentIndex().row();
    if (row < 1)
        return;
    QString name = mView->item(row, 0)->text();

    QSettings *ini = new QSettings("./nandflash/global.ini", QSettings::IniFormat);
    ini->setIniCodec("GB18030");
    ini->beginGroup("GLOBAL");
    ini->setValue("FileInUse", name);

    queryType();
}

void ConfPage::queryType()
{
    QDir dir("./config");
    dir.setFilter(QDir::Files | QDir::Hidden | QDir::NoSymLinks);
    dir.setSorting(QDir::Size | QDir::Reversed);

    FileNames.clear();
    QFileInfoList list = dir.entryInfoList();
    for (int i=0; i < list.size(); i++)
        FileNames.append(list.at(i).fileName());

    if (FileNames.isEmpty())
        FileNames.append("Bash_File.ini");

    mView->setRowCount(0);
    mView->appendRow(new QStandardItem(CurrentSettings()));
    for (int i=0; i < FileNames.size(); i++) {
        QString type = FileNames.at(i);
        type = type.remove(".ini");
        if (type == CurrentSettings())
            continue;
        mView->appendRow(new QStandardItem(type.remove(".ini")));
    }
}

QString ConfPage::CurrentSettings()
{
    QSettings *ini = new QSettings("./nandflash/global.ini", QSettings::IniFormat);
    QString n = ini->value("/GLOBAL/FileInUse", "Base_File").toString();
    return n.remove(".ini");
}
