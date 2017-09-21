#include "lqmath.h"

LQMath::LQMath(QObject *parent) : QObject(parent)
{

}

QStringList LQMath::angleOrder(QStringList s)
{
    QList<int> squn;
    QStringList strs;
    squn << 0 << 1 << 2
         << 15 << 16 << 17
         << 30 << 31 << 32
         << 3 << 4 << 5 << 6 << 7 << 8
         << 18 << 19 << 20 << 21 << 22 << 23
         << 33 << 34 << 35 << 36 << 37 << 38
         << 9 << 10 << 11
         << 24 << 25 << 26
         << 39 << 40 << 41
         << 12 << 13 << 14
         << 27 << 28 << 29
         << 42 << 43 << 44;
    for (int i=0; i < qMin(squn.size(), s.size()); i++)
        strs.append(s.at(squn.at(i)));
    return strs;
}

QStringList LQMath::readRotation(QStringList s, int speed, int count)
{
    QStringList str;
    int r = 60*100000/(speed*count);
    for (int i=0; i < s.size(); i++)
        str.append(QString::number(s.at(i).toDouble()*360/r));
    return str;
}

QStringList LQMath::angleFilter(QStringList s, double std, double min, double max)
{
    QStringList angles;
    for (int i=0; i < s.size(); i++) {
        double r = s.at(i).toDouble();
        if (abs(r-std)/std > min/100 && abs(r-std)/std < max/100) {
            if (r > std)
                angles.append(QString::number(r-std/min));
            else
                angles.append(QString::number(r+std/min));
        } else
            angles.append(s.at(i));
    }
    return angles;
}

QStringList LQMath::angleOffset(QStringList s, double offset)
{
    QStringList str;
    for (int i=0; i < s.size(); i++) {
        double r = s.at(i).toDouble();
        str.append(QString::number(r+offset));
    }
    return str;
}

QString LQMath::angleShow(QStringList s)
{
    QString tmp;
    QStringList names;
    names << "U1:" << "U2:" << "U3:"
          << "V1:" << "V2:" << "V3:"
          << "W1:" << "W2:" << "W3:"

          << "UH1:" << "UH2:" << "UH3:"
          << "UL1:" << "UL2:" << "UL3:"
          << "VH1:" << "VH2:" << "VH3:"
          << "VL1:" << "VL2:" << "VL3:"
          << "WH1:" << "WH2:" << "WH2:"
          << "WL1:" << "WL3:" << "WL2:"

          << "UW1:" << "UW2:" << "UW3:"
          << "VU1:" << "VU2:" << "VU3:"
          << "WV1:" << "WV2:" << "WV3:"

          << "Hu-UV:" << "Hv-VW:" << "Hw-WU:"
          << "Hv-VW1:" << "Hv-VW2:" << "Hv-VW3:"
          << "Hw-WU1:" << "Hw-WU2:" << "Hw-WU3:";
    for (int i=0; i < qMin(names.size(), s.size()); i++) {
        double angle = s.at(i).toDouble();
        QString t = QString("%1%2").arg(names.at(i)).arg(QString::number(angle, 'f', 1));
        tmp.append(t);
        if (i%3 == 2) {
            tmp.append("\n");
        } else {
            tmp.append("\t\t");
        }
        if (i%9 == 8 && i != 17)
            tmp.append("\n");
    }
    return tmp;
}

QString LQMath::powerShow(QStringList s1, QStringList s2)
{
    QString tmp;
    tmp.append("\n");
    QStringList item;
    item << "高电平:" << "低电平:" << "频率:";
    int tt = 0;
    for (int i=110; i < s1.size(); i++) {
        if (i >= 170)
            break;
        int t = (i-50)%20;
        if (t == 0) {
            tmp.append(item.at(0));
            tmp.append(QString::number(s1.at(i).toDouble()*15.28/4095, 'f', 2));
            tmp.append("V");
            tmp.append("\t\t");
        } else if (t == 1) {
            tmp.append(item.at(1));
            tmp.append(QString::number(s1.at(i).toDouble()*15.28/4095, 'f', 2));
            tmp.append("V");
            tmp.append("\t\t");
        } else if (t == 7) {
            tmp.append(item.at(2));
            tmp.append(QString::number(s2.at(tt).toDouble(), 'f', 1));
            tmp.append("Hz");
            tmp.append("\n");
            tt++;
        }
    }
    return tmp;
}


double LQMath::readSquare(QStringList s)
{
    int w = 0;
    for (int i=0; i < s.size(); i++) {
        int t = s.at(i).toInt()-128;
        w += t*t;
    }
    return sqrt(w/s.size());
}

double LQMath::readWorst(double std, QStringList s)
{
    QList<double> w;
    for (int i=0; i < s.size(); i++) {
        w.append(abs(std-s.at(i).toDouble()));
    }
    double max = w.at(0);
    int t = 0;
    for (int i=0; i < w.size(); i++) {
        if (max < w.at(i)) {
            max = w.at(i);
            t = i;
        }
    }
    return s.at(t).toDouble();
}

double LQMath::readMax(QStringList s)
{
    QList<double> w;
    for (int i=0; i < s.size(); i++) {
        w.append(s.at(i).toDouble());
    }
    double max = w.at(0);
    for (int i=0; i < w.size(); i++) {
        max = qMax(max, w.at(i));
    }
    return max;
}

double LQMath::readMin(QStringList s)
{
    QList<double> w;
    for (int i=0; i < s.size(); i++) {
        w.append(s.at(i).toDouble());
    }
    double min = w.at(0);
    for (int i=0; i < w.size(); i++) {
        min = qMin(min, w.at(i));
    }
    return min;
}

double LQMath::readAvr(QStringList s)
{
    if (s.isEmpty())
        return 0;
    double w = 0;
    for (int i=0; i < s.size(); i++) {
        w += s.at(i).toDouble();
    }
    return w/s.size();
}

double LQMath::readPhase(QStringList s1, QStringList s2)
{
    int t1 = 0;
    int t2 = 0;
    for (int i=0; i < s1.size()-10; i++) {
        double a1 = 0;
        double a2 = 0;
        for (int j=0; j < 5; j++) {
            a1 += s1.at(i+j).toInt();
            a2 += s1.at(i+j+1).toInt();
        }
        a1 /=5;
        a2 /=5;
        if ((a1 <= 127) && (a2 >= 127)) {
            t1 = i+2;
            break;
        }
    }
    int max = 0;
    for (int i=0; i < s2.size(); i++) {
        if (max < s2.at(i).toInt())
            max = s2.at(i).toInt();
    }
    for (int i=t1; i < s2.size()-10; i++) {
        double a1 = 0;
        double a2 = 0;
        for (int j=0; j < 5; j++) {
            a1 += s2.at(i+j).toInt();
            a2 += s2.at(i+j+1).toInt();
        }
        a1 /=5;
        a2 /=5;
        if ((a1 >= max/2) && (a2 <= max/2)) {
            t2 = i+2;
            break;
        }
    }
    return t2-t1;
}

double LQMath::readBalance(QStringList s)
{
    if (s.isEmpty())
        return -1;
    double max = s.at(0).toDouble();
    double min = s.at(0).toDouble();
    double avr = 0;
    for (int i=0; i < s.size(); i++) {
        avr += s.at(i).toDouble();
        max = qMax(max, s.at(i).toDouble());
        min = qMin(min, s.at(i).toDouble());
    }
    avr /= s.size();
    if (avr == 0)
        return -1;
    return ((max-min) * 100 / avr);
}

//void MainPage::testDebug()
//{
//    qDebug() << "test readBance";
//    QStringList s1;
//    s1 << "2425" << "2426" << "2427";
//    qDebug() << readBalance(s1);

//    srand(QDateTime::currentDateTime().toMSecsSinceEpoch());
//    QStringList s2;
//    for (int i=0; i < 5; i++) {
//        for (int t=0; t < 3; t++)
//            s2 << QString::number((360+rand()%2)/0.18);
//        for (int t=0; t < 3; t++)
//            s2 << QString::number((180+rand()%1)/0.18);
//        for (int t=0; t < 3; t++)
//            s2 << QString::number((180+rand()%1)/0.18);
//        for (int t=0; t < 3; t++)
//            s2 << QString::number((120+rand()%1)/0.18);
//        for (int t=0; t < 3; t++)
//            s2 << QString::number((50+rand()%1)/0.18);
//    }
//    for (int i=s2.size(); i < 110; i++) {
//        s2.append("0");
//    }
//    QStringList s3;
//    s3 << "3325" << "52" << "3300" << "3325" << "0" << "0" << "0" << "50" << "50" << "0";
//    s2 << s3 << s3 << s3 << s3 << s3 << s3 << s3 << s3 << s3;
//    qDebug() << "test s";
//    qDebug() << s2;
//    qDebug() << "test order";
//    qDebug() << math.angleOrder(s2);
//    qDebug() << "test rotation";
//    qDebug() << math.readRotation(math.angleOrder(s2), 1000, 3);
//    qDebug() << "test filter";
//    QStringList full = angleFilter(math.readRotation(math.angleOrder(s2), 1000, 3).mid(0,9), 360, 10, 20);
//    qDebug() << full;
//    QStringList half = angleFilter(math.readRotation(math.angleOrder(s2), 1000, 3).mid(9,18), 180, 10, 20);
//    qDebug() << half;
//    QStringList with = angleFilter(math.readRotation(math.angleOrder(s2), 1000, 3).mid(27,9), 120, 10, 20);
//    qDebug() << with;
//    QStringList hall = angleFilter(math.readRotation(math.angleOrder(s2), 1000, 3).mid(36,9), 32.5, 10, 20);
//    qDebug() << hall;
//    qDebug() << "test offset";
//    QStringList offset = readOffset();
//    qDebug() << offset;
//    qDebug() << "angle offset";
//    full = angleOffset(full, offset.at(0).toDouble());
//    half = angleOffset(half, offset.at(1).toDouble());
//    with = angleOffset(with, offset.at(2).toDouble());
//    hall = angleOffset(hall, offset.at(3).toDouble());
//    qDebug() << "test worst";
//    double wFull = readWorst(360, full);
//    double wHalf = readWorst(180, half);
//    double wWith = readWorst(120, with);
//    double wHall = readWorst(32.5, hall);
//    qDebug() << wFull << wHalf << wWith << wHall;
//    QString vv;
//    vv.append(QString("%1°,").arg(QString::number(wFull, 'f', 1)));
//    vv.append(QString("%1°,").arg(QString::number(wHalf, 'f', 1)));
//    vv.append(QString("%1°,").arg(QString::number(wWith, 'f', 1)));
//    vv.append(QString("%1° ").arg(QString::number(wHall, 'f', 1)));
//    qDebug() << vv;
//    qDebug() << "test hall";
//    //    qDebug() << powerShow(s2);
//    qDebug() << "test show";
//    QStringList angle;
//    angle << full << half << with << hall;
//    QString ss = angleShow(angle);
//    //    ss.append(powerShow(s2));
//    testBox = new PopupBox(this, "", "配置中，请稍后", QMessageBox::Ok);
//    testBox->setWindowFlags(Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint | Qt::Popup);
//    testBox->setStyleSheet("QDialog{border:2px solid cyan;}");
//    testBox->resize(QSize(1024, 768));
//    testBox->setText(ss);
//    testBox->show();

//    QStringList square;
//    QStringList hallwave;
//    for (int i=0; i < 800; i++) {
//        square << QString::number(128*sin(i*6.28/360))+128;
//        if (((i+120)%360) > 180)
//            hallwave << "254";
//        else
//            hallwave << "14";
//    }

//    qDebug() << "test square";
//    qDebug() << square;
//    qDebug() << readSquare(square);
//    qDebug() << "test phase";
//    qDebug() << readPhase(square, hallwave);

//    qDebug() << "test scale";
//    qDebug() << readScale();
//}
