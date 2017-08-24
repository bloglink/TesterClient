/*******************************************************************************
 * Copyright [2017]   <青岛艾普智能仪器有限公司>
 * All rights reserved.
 *
 * version:     0.1
 * author:      zhaonanlin
 * brief:       232控制模块
*******************************************************************************/
#ifndef CTRLDEVICE_232_H
#define CTRLDEVICE_232_H

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

class CtrlDevice_232 : public QWidget
{
    Q_OBJECT
public:
    explicit CtrlDevice_232(QWidget *parent = 0);
    ~CtrlDevice_232();
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
    QSerialPort *com6;
    QSerialPort *com7;
    QSerialPort *com8;
    QTimer *timer;
};

#endif // CTRLDEVICE_232_H
