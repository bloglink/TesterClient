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

void ConfPage::initUI()
{
    this->setObjectName("ConfPage");
    QStringList headers;
    headers << tr("电机型号");
    mView = new ItemModel(0, headers.size());
    mView->setHorizontalHeaderLabels(headers);

    view = new QTableView(this);
    view->setModel(mView);
    view->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    typeLineEdit = new QLineEdit(this);
    typeLineEdit->setMinimumSize(97, 35);
    typeLineEdit->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);

    QPushButton *btnAppend = new QPushButton(this);
    btnAppend->setText(tr("添加型号"));
    btnAppend->setMinimumSize(97, 35);
    connect(btnAppend,SIGNAL(clicked(bool)),this,SLOT(appendType()));
    QPushButton *btnRemove = new QPushButton(this);
    btnRemove->setText(tr("删除型号"));
    btnRemove->setMinimumSize(97, 35);
    connect(btnRemove,SIGNAL(clicked(bool)),this,SLOT(deleteType()));
    QPushButton *btnExit = new QPushButton(this);
    btnExit->setText(tr("保存退出"));
    btnExit->setMinimumSize(97, 35);
    connect(btnExit, SIGNAL(clicked(bool)), this, SLOT(saveData()));

    QHBoxLayout *btnLayout = new QHBoxLayout;
    btnLayout->addWidget(new QLabel("型号名称",this));
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
    connect(typeComboBox,SIGNAL(currentIndexChanged(QString)),this,SLOT(autoPixmap(QString)));
    typePixmap = new QLabel(this);
    typePixmap->setPixmap(QPixmap(":/source/M1S0.jpg"));

    QGridLayout *cLayout = new QGridLayout;
    for (int i=0; i < 8; i++) {

        QPushButton *btn = new QPushButton(QString::number(i+1));
        colors.append(btn);
        cLayout->addWidget(btn,i/2,i%2);
        connect(btn,SIGNAL(clicked(bool)),this,SLOT(selectColor()));
    }

    QGridLayout *tLayout = new QGridLayout;
    tLayout->addWidget(new QLabel("电机类型",this),0,0);
    tLayout->addWidget(typeComboBox,0,1);
    tLayout->addWidget(typePixmap,1,0,1,2);
    tLayout->addWidget(new QLabel("线夹颜色",this),2,0);
    tLayout->addLayout(cLayout,3,0,1,2);
    tLayout->setColumnStretch(1,1);
    tLayout->setRowStretch(4,1);

    QGroupBox *tGroup = new QGroupBox(this);
    tGroup->setLayout(tLayout);

    QStringList pHeaders;
    pHeaders << tr("测试项目");
    pModel = new ItemModel(0, pHeaders.size());
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

    QStringList btnNames;
    btnNames << "电阻" << "反嵌" << "绝缘" << "交耐" << "匝间"
             << "电感" << "空载" << "加载" << "FG";
    QStringList objNames;
    objNames << "ConfigDCR" << "ConfigMAG" << "ConfigIR" << "ConfigACW" << "ConfigIMP"
             << "ConfigIND" << "ConfigPWR" << "ConfigLoad" << "ConfigFG";
    for (int i=0; i < btnNames.size(); i++) {
        buttons.append(new QPushButton(btnNames.at(i),this));
        btnsLayout->addWidget(buttons.at(i));
        buttons.at(i)->setMinimumSize(97, 44);
        buttons.at(i)->setObjectName(objNames.at(i));
        buttons.at(i)->setCheckable(true);
        connect(buttons.at(i),SIGNAL(clicked(bool)),this,SLOT(clickButton()));
    }

    QPushButton *btnDelete = new QPushButton("删除项目",this);
    btnDelete->setMinimumSize(97, 35);
    connect(btnDelete,SIGNAL(clicked(bool)),this,SLOT(deleteItem()));
    btnsLayout->addWidget(btnDelete);
    QPushButton *btnConfig = new QPushButton("配置项目",this);
    btnConfig->setMinimumSize(97, 35);
    connect(btnConfig,SIGNAL(clicked(bool)),this,SLOT(windowChange()));
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

void ConfPage::saveData()
{
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

    temp.clear();
    temp.append(typeComboBox->currentText());
    QDomElement type = doc.createElement("type");
    root.appendChild(type);
    text = doc.createTextNode(temp.join(","));
    type.appendChild(text);

    emit sendNetMsg(doc.toByteArray());
    emit buttonClicked(NULL);
}

void ConfPage::clickButton()
{
    QPushButton *btn = qobject_cast<QPushButton *>(sender());
    for (int i=0; i < buttons.size(); i++) {
        buttons.at(i)->setChecked(false);
    }
    btn->setChecked(true);
    if (pView->currentIndex().row() < 0 || (pView->currentIndex().row()+1 == pModel->rowCount())) {
        pModel->appendRow(new QStandardItem(""));
        if (pModel->rowCount() > 1)
            pModel->item(pModel->rowCount()-2,0)->setText(btn->text());
    }
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
    if(color.isValid()) {
        btn->setStyleSheet(QString("background-color:%1").arg(color.name()));
    }
}

void ConfPage::deleteItem()
{
    int row = pView->currentIndex().row();
    if (row < 0)
        return;
    pModel->removeRow(row);
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
    emit sendNetMsg("3004");
    emit sendNetMsg("6004 sys");
    emit sendNetMsg("6004 conf");
}

void ConfPage::appendType()
{
    emit sendNetMsg("3000");
}

void ConfPage::deleteType()
{
    emit sendNetMsg("3000");
}
