#ifndef CONFHALL_H
#define CONFHALL_H

#include <QWidget>
#include <QTableView>
#include <QHeaderView>
#include <QLayout>
#include <QCheckBox>
#include <QLineEdit>
#include <QComboBox>
#include <QPushButton>
#include <QDebug>
#include <QSpinBox>
#include <QLabel>
#include <QJsonObject>

#include "conf_delegation.h"

class ConfHall : public QWidget
{
    Q_OBJECT
public:
    explicit ConfHall(QWidget *parent = 0);
    ~ConfHall();

signals:
    void buttonClicked(QByteArray win);
    void sendAppCmd(QJsonObject obj);
public slots:
    void initSettings(QJsonObject obj);
    void readSettings();
    int readCount();
private slots:
    void initUI();
    void back();
private:
    QTableView *view;
    StandardItem *mView;

    QCheckBox *turnCheckBox;
    QComboBox *turnComboBox;

    QStringList itemNames;
};

#endif // CONFHALL_H
