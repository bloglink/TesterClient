#ifndef CONF_QRENCODE_H
#define CONF_QRENCODE_H

#include <QDebug>
#include <QLabel>
#include <QWidget>
#include <QLayout>
#include <QSpinBox>
#include <QCheckBox>
#include <QComboBox>
#include <QTableView>
#include <QMessageBox>
#include <QHeaderView>
#include <QPushButton>
#include <QJsonObject>
#include <QTextBrowser>
#include <QTextEdit>
#include <QLineEdit>
#include <QImage>
#include <QFileDialog>
#include <QListView>
#include <QPrinter>
#include <QGroupBox>
#include <QSerialPort>

class Conf_QRencode : public QWidget
{
    Q_OBJECT
public:
    explicit Conf_QRencode(QWidget *parent = 0);
signals:
    void buttonClicked(QByteArray win);
    void sendAppCmd(QJsonObject obj);
public slots:
    void initSettings(QJsonObject msg);
    void printer();
private slots:
    void initUI();
    void saveSettings();
    void openFile();
    void openCom();
    void back();
private:
    QTextEdit *text;
    QLabel *logo;
    QLabel *size;
    QLineEdit *edit1;
    QLineEdit *edit2;
    QLineEdit *edit3;
    QLineEdit *edit4;
    QLineEdit *edit5;
    QLineEdit *edit6;
    QTextBrowser *soft;
    QComboBox *codeComboBox;
    QComboBox *portComboBox;
    QSerialPort *com;
};

#endif // CONF_QRENCODE_H
