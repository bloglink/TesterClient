#ifndef CONFINDUCTANCE_H
#define CONFINDUCTANCE_H

#include <QDebug>
#include <QLabel>
#include <QWidget>
#include <QLayout>
#include <QLineEdit>
#include <QCheckBox>
#include <QTableView>
#include <QHeaderView>
#include <QPushButton>
#include <QtXml/QDomDocument>

#include "conf_delegation.h"

class ConfInductance : public QWidget
{
    Q_OBJECT
public:
    explicit ConfInductance(QWidget *parent = 0);
    ~ConfInductance();

signals:
    void sendNetMsg(QByteArray dat);
    void buttonClicked(QByteArray win);
public slots:
    void initData(QString dat);
private slots:
    void initUI();
    void saveData();
    void autoInput();
    void appendXmlData(int column, QString name);
    void recvAppShow(QString win);
private:
    QTableView *view;
    StandardItem *model;

    QLineEdit *nounLineEdit;
    QSpinBox *avrSpinBox;
    QComboBox *freqComboBox;
    QComboBox *connComboBox;
    QComboBox *modeComboBox;

    QDomDocument doc;
    QDomElement root;
    QStringList itemNames;
};

#endif // CONFINDUCTANCE_H
