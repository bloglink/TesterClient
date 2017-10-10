#ifndef BACKPAGE_H
#define BACKPAGE_H

#include <QDebug>
#include <QWidget>
#include <QLayout>
#include <QSettings>
#include <QTableView>
#include <QHeaderView>
#include <QPushButton>
#include <QJsonObject>

#include "conf_delegation.h"

class BackPage : public QWidget
{
    Q_OBJECT
public:
    explicit BackPage(QWidget *parent = 0);
signals:
    void buttonClicked(QByteArray win);
    void sendAppCmd(QJsonObject obj);
public slots:
    void initSettings(QJsonObject obj);
    void readSettings();
    void saveSettings();
private slots:
    void initUI();
    void back();
    void recvAppShow(QString win);
private:
    QTableView *view;
    StandardItem *mView;
    QStringList itemNames;
};

#endif // BACKPAGE_H
