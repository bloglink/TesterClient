#ifndef IOBRD_H
#define IOBRD_H

#include <QTimer>
#include <QObject>
#include <QDataStream>
#include <QtSerialPort/QSerialPort>

class IOBrd : public QObject
{
    Q_OBJECT
public:
    explicit IOBrd(QObject *parent = 0);

signals:

public slots:
    bool initPort(QString portName);
    quint16 readPort();
    bool sendPort(quint16 hex);
private slots:
    bool readThread();
private:
    QSerialPort *com;
    quint16 status;
};

#endif // IOBRD_H
