#include "conf_qrencode.h"

Conf_QRencode::Conf_QRencode(QWidget *parent) : QWidget(parent)
{
    initUI();
}

void Conf_QRencode::initSettings(QJsonObject msg)
{
    QStringList items;
    items << "edit1" << "edit2" << "edit3" << "edit4" << "edit5" << "edit6"
          << "text" << "code" << "port";
    for (int i=0; i < items.size(); i++) {
        QString temp = msg.value(items.at(i)).toString();
        if (items.at(i) == "edit1") {
            if (temp.isEmpty())
                temp = "FZQ0060000018A";
            edit1->setText(temp);
        }
        if (items.at(i) == "edit2") {
            if (temp.isEmpty())
                temp = "000000";
            edit2->setText(temp);
        }
        if (items.at(i) == "edit3") {
            if (temp.isEmpty())
                temp = "20180129";
            edit3->setText(temp);
        }
        if (items.at(i) == "edit4") {
            if (temp.isEmpty())
                temp = "0A0A0A";
            edit4->setText(temp);
        }
        if (items.at(i) == "edit5") {
            if (temp.isEmpty())
                temp = "0001";
            edit5->setText(temp);
        }
        if (items.at(i) == "edit6") {
            if (temp.isEmpty())
                temp = "model.txt";
            edit6->setText(temp);
            QFile file(temp);
            if (file.open(QFile::ReadOnly)) {
                soft->insertPlainText(file.readAll());
            }
        }
        if (items.at(i) == "text") {
            if (temp.isEmpty())
                temp = "http://s.i-oid.org/q?m=i.1.370181.16.25.20.15003.999000000000122.0132.000001.20180315.0000000001";
            text->setText(temp);
        }
        if (items.at(i) == "code") {
            if (temp.isEmpty())
                temp = "0";
            codeComboBox->setCurrentIndex(temp.toInt());
        }
        if (items.at(i) == "port") {
            if (temp.isEmpty())
                temp = "0";
            portComboBox->setCurrentIndex(temp.toInt());
        }
    }
}

void Conf_QRencode::printer()
{
    if (codeComboBox->currentIndex() == 0)
        return;
    QString strs;
    if (codeComboBox->currentIndex() == 1){
        int t = edit5->text().toInt()+1;
        edit5->setText(QString("%1").arg(t, 4, 10, QChar('0')));
        strs.append(edit1->text());
        strs.append("\n");
        strs.append(edit2->text());
        strs.append("\n");
        strs.append(edit3->text());
        strs.append("\n");
        strs.append(edit4->text());
        strs.append("\n");
        strs.append(edit5->text());
    }
    if (codeComboBox->currentIndex() == 2) {
        QStringList tmp = text->toPlainText().split(".");
        int t = tmp[tmp.size()-1].toInt()+1;
        tmp[tmp.size()-1] = QString("%1").arg(t, 10, 10, QChar('0'));
        text->setPlainText(tmp.join("."));
        strs.append(text->toPlainText());
    }
    QString p = soft->toPlainText();
    QStringList pp = p.split("\n");
    for (int i=0; i < pp.size(); i++) {
        if (pp.at(i).contains("^FDMA")) {
            pp[i] = "^FH\\\^FDMA," + strs;
            break;
        }
    }
    p = pp.join("\n");
    if (com == NULL || !com->isOpen()) {
        com = new QSerialPort(portComboBox->currentText(), this);
        if (com->open(QIODevice::ReadWrite)) {
            com->setBaudRate(115200);
            com->setParity(QSerialPort::NoParity);
            com->setDataBits(QSerialPort::Data8);
            com->setStopBits(QSerialPort::OneStop);
            com->setFlowControl(QSerialPort::NoFlowControl);
        }
    }
    qDebug() << p;
    com->write(p.toUtf8());
    saveSettings();
}

void Conf_QRencode::initUI()
{
    com = NULL;
    this->setObjectName("qrencode");
    QGridLayout *boxLay1 = new QGridLayout;

    int row = 0;
    edit1 = new QLineEdit(this);
    boxLay1->addWidget(new QLabel(tr("产品整机号:"), this), row, 0);
    boxLay1->addWidget(edit1, row, 1, 1, 7);

    row = boxLay1->rowCount();
    edit2 = new QLineEdit(this);
    boxLay1->addWidget(new QLabel(tr("客户代码:"), this), row, 0);
    boxLay1->addWidget(edit2, row, 1, 1, 7);

    row = boxLay1->rowCount();
    edit3 = new QLineEdit(this);
    boxLay1->addWidget(new QLabel(tr("生产日期:"), this), row, 0);
    boxLay1->addWidget(edit3, row, 1, 1, 7);

    row = boxLay1->rowCount();
    edit4 = new QLineEdit(this);
    boxLay1->addWidget(new QLabel(tr("工厂代码:"), this), row, 0);
    boxLay1->addWidget(edit4, row, 1, 1, 7);

    row = boxLay1->rowCount();
    edit5 = new QLineEdit(this);
    boxLay1->addWidget(new QLabel(tr("序列号:"), this), row, 0);
    boxLay1->addWidget(edit5, row, 1, 1, 7);

    QGroupBox *box1 = new QGroupBox(tr("通用规则"), this);
    box1->setLayout(boxLay1);

    soft = new QTextBrowser(this);
    QGroupBox *box2 = new QGroupBox(tr("指令预览"), this);
    QVBoxLayout *boxLay2 = new QVBoxLayout;
    boxLay2->addWidget(soft);
    box2->setLayout(boxLay2);


    QVBoxLayout *boxLay3 = new QVBoxLayout;

    text = new QTextEdit(this);
    boxLay3->addWidget(text);

    QGroupBox *box3 = new QGroupBox(tr("海尔规则:"), this);
    box3->setLayout(boxLay3);

    QHBoxLayout *btnLayout = new QHBoxLayout;

    QStringList portNames;
    portNames << "COM3" << "COM4" << "COM5" << "COM6" << "COM7" << "COM8" << "ttyS0";
    portComboBox = new QComboBox(this);
    portComboBox->addItems(portNames);
    portComboBox->setMinimumSize(97, 35);
    portComboBox->setView(new QListView);

    btnLayout->addWidget(new QLabel(tr("打开串口"), this));
    btnLayout->addWidget(portComboBox);

    QStringList codeNames;
    codeNames << tr("不打印") << tr("通用规则") << tr("海尔规则");
    codeComboBox = new QComboBox(this);
    codeComboBox->addItems(codeNames);
    codeComboBox->setMinimumSize(97, 35);
    codeComboBox->setView(new QListView);

    btnLayout->addWidget(new QLabel(tr("条码方式"), this));
    btnLayout->addWidget(codeComboBox);

    QPushButton *btnLogo = new QPushButton(this);
    btnLogo->setText(tr("模板位置"));
    connect(btnLogo, SIGNAL(clicked(bool)), this, SLOT(openFile()));
    row = boxLay1->rowCount();
    edit6 = new QLineEdit(this);
    btnLayout->addWidget(btnLogo);
    btnLayout->addWidget(edit6);

    btnLayout->addStretch();

    QPushButton *btnPrint = new QPushButton(this);
    btnPrint->setText(tr("打印"));
    btnPrint->setMinimumSize(97, 35);
    connect(btnPrint, SIGNAL(clicked(bool)), this, SLOT(printer()));
    btnLayout->addWidget(btnPrint);

    QPushButton *btnExit = new QPushButton(this);
    btnExit->setText(tr("退出"));
    btnExit->setMinimumSize(97, 35);
    connect(btnExit, SIGNAL(clicked(bool)), this, SLOT(back()));
    btnLayout->addWidget(btnExit);

    QGridLayout *mLayout = new QGridLayout(this);
    mLayout->addWidget(box1, 0 , 0);
    mLayout->addWidget(box2, 0, 2);
    mLayout->addWidget(box3, 0, 1);
    mLayout->addLayout(btnLayout, 1, 0, 1, 3);
}

void Conf_QRencode::saveSettings()
{
    QJsonObject obj;
    obj.insert("edit1", edit1->text());
    obj.insert("edit2", edit2->text());
    obj.insert("edit3", edit3->text());
    obj.insert("edit4", edit4->text());
    obj.insert("edit5", edit5->text());
    obj.insert("edit6", edit6->text());
    obj.insert("text", text->toPlainText());
    obj.insert("code", QString::number(codeComboBox->currentIndex()));
    obj.insert("port", QString::number(portComboBox->currentIndex()));

    QJsonObject array;
    array.insert("QRCODE", obj);
    emit sendAppCmd(array);
}

void Conf_QRencode::openFile()
{
    QString fileName = QFileDialog::getOpenFileName(this,
                                                    tr("打开文件"),
                                                    "/",
                                                    tr("二进制文件(*.*)"));
    if (fileName.length() != 0) {
        edit6->setText(fileName);
    }
}

void Conf_QRencode::openCom()
{

}

void Conf_QRencode::back()
{
    saveSettings();
    emit buttonClicked(NULL);
}

