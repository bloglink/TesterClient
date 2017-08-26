#ifndef CONFIGFG_H
#define CONFIGFG_H

#include <QWidget>
#include <QTableView>
#include <QHeaderView>
#include <QLayout>
#include <QCheckBox>
#include <QLineEdit>
#include <QComboBox>
#include <QPushButton>
#include <QDebug>
#include <QtXml/QDomDocument>
#include <QXmlStreamWriter>
#include <QXmlStreamReader>
#include <QSpinBox>
#include <QLabel>

#include "conf_delegation.h"

class ConfigFG : public QWidget
{
    Q_OBJECT
public:
    explicit ConfigFG(QWidget *parent = 0);
    ~ConfigFG();

signals:
    void sendNetMsg(QByteArray dat);
    void buttonClicked(QByteArray win);
public:
    void initData(QString dat);
private slots:
    void initUI();
    void saveData();
    void appendXmlData(int column, QString name);
    void recvAppShow(QString win);
private:
    QTableView *view;
    StandardItem *model;

    QCheckBox *turnCheckBox;
    QComboBox *turnComboBox;

    QDomDocument doc;
    QDomElement root;
    QStringList itemNames;
};

#endif // CONFIGFG_H
