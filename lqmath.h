#ifndef LQMATH_H
#define LQMATH_H

#include <QObject>
#include <qmath.h>

class LQMath : public QObject
{
    Q_OBJECT
public:
    explicit LQMath(QObject *parent = 0);

signals:

public slots:
    QStringList angleOrder(QStringList s);
    QStringList readRotation(QStringList s, int speed, int count);
    QStringList angleFilter(QStringList s, double std, double min, double max);
    QStringList angleOffset(QStringList s, double offset);
    QString angleShow(QStringList s);
    QString powerShow(QStringList s1, QStringList s2);
    double readSquare(QStringList s);
    double readWorst(double std, QStringList s);
    double readMax(QStringList s);
    double readMin(QStringList s);
    double readAvr(QStringList s);
    double readPhase(QStringList s1, QStringList s2);
    double readBalance(QStringList s);
};

#endif // LQMATH_H
