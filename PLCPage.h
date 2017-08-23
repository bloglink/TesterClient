/*******************************************************************************
 * Copyright [2017]   <青岛艾普智能仪器有限公司>
 * All rights reserved.
 *
 * version:     0.0.0.1
 * author:      zhaonanlin
 * brief:       PLC控制模块
*******************************************************************************/
#ifndef PLCPAGE_H
#define PLCPAGE_H

#include <QTimer>
#include <QObject>
#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>
#include <QTextBrowser>
#include <QDataStream>
#include <QDebug>
#include <QElapsedTimer>
#include <QCoreApplication>
#include <QMessageBox>
#include <QFile>

class PLCPage : public QWidget
{
    Q_OBJECT
public:
    explicit PLCPage(QWidget *parent = 0);

signals:

public slots:
    void sendIO(quint16 hex);
    quint16 readIO();
    void sendPlc(QString cmd);
    void readPlc();
private slots:
    void initCom();
    void wait(int ms);
private:
    QSerialPort *com3;
    QSerialPort *com4;
    QSerialPort *com5;
    QTimer *timer;
};

#endif // PLCPAGE_H
