#ifndef IOBRD_H
#define IOBRD_H

#include <QDebug>
#include <QTimer>
#include <QObject>
#include <QDataStream>
#include <QMessageBox>
#include <QElapsedTimer>
#include <QCoreApplication>
#include <QtSerialPort/QSerialPort>

//#define X01_TARGET 0x0100
//#define X01_ORIGIN 0x0200
//#define X02_TARGET 0x0400
//#define X02_ORIGIN 0x0800
//#define X03_TARGET 0x0800
//#define X03_ORIGIN 0x0400
//#define X04_TARGET 0x4000
//#define X04_ORIGIN 0x8000

#define X01_TARGET 0x0100
#define X01_ORIGIN 0x0200
#define X02_TARGET 0x0400
#define X02_ORIGIN 0x0800
#define X03_TARGET 0x2000
#define X03_ORIGIN 0x1000
#define X04_TARGET 0x4000
#define X04_ORIGIN 0x8000

//#define X10 0x0040  // 启动
//#define X11 0x0002  // 启动
//#define X12 0x0080  // 停止
//#define X13 0x0010  // 停止

#define X10 0x0001  // 启动
#define X11 0x0002  // 启动
#define X12 0x0004  // 停止
#define X13 0x0008  // 停止
#define X14 0x0010  // 复位

#define Y00 0x0001  // 台下起降气缸1
#define Y01 0x0002  // 台下夹紧气缸2
#define Y02 0x0004  // 台上压紧气缸3
#define Y03 0x0008  // 台上耐压气缸4

//#define Y08 0x0100  // 绿灯
//#define Y09 0x0200  // 蜂鸣器
//#define Y10 0x0400  // 黄灯
//#define Y11 0x0800  // 红灯

#define Y08 0x0100  // 蜂鸣器
#define Y09 0x0200  // 红灯
#define Y10 0x0400  // 黄灯
#define Y11 0x0800  // 绿灯

#define LED_B Y08
#define LED_R Y09
#define LED_Y Y10
#define LED_G Y11

class IOBrd : public QObject
{
    Q_OBJECT
public:
    explicit IOBrd(QObject *parent = 0);

signals:
    void sendStart(bool s);
    void iobrdReset();
public slots:
    bool initPort(QString portName);
    quint16 readPort();
    bool sendPort(quint16 hex);
    bool waitPort(quint16 hex);
    void quitPort(bool s);
    bool open(QString name);
    bool close();
    bool write(quint16 hex);
    quint16 get_target_status(quint16 hex);
    bool thread();
private slots:
    bool readThread();
    bool wait(int t);
private:
    QSerialPort *com;
    quint16 status;
    bool isQuit;
    quint16 target_status;
    quint16 time_out;
    quint16 io_hex;
};

#endif // IOBRD_H
