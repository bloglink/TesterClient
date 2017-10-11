#ifndef MAGNETIC_H
#define MAGNETIC_H

#include <QDebug>
#include <QLabel>
#include <QWidget>
#include <QLayout>
#include <QSpinBox>
#include <QCheckBox>
#include <QSettings>
#include <QTableView>
#include <QHeaderView>
#include <QPushButton>
#include <QMessageBox>
#include <QJsonObject>

#include "conf_delegation.h"
#include "dcombobox.h"
#include "dspinbox.h"
#include "ddoublespinbox.h"

#include "qcustomplot.h"

class Magnetic : public QWidget
{
    Q_OBJECT
public:
    explicit Magnetic(QWidget *parent = 0);

signals:
    void buttonClicked(QByteArray win);
    void sendAppCmd(QJsonObject obj);
    void sendNetMsg(QByteArray msg);
public slots:
    void initSettings(QJsonObject obj);
    void readSettings();
private slots:
    void initUI();
    void waveL();
    void waveR();
    void back();
    void recvAppShow(QString win);
private:
    QTableView *view;
    StandardItem *mView;

    QCheckBox *turnCheckBox;
    QComboBox *turnComboBox;

    QStringList itemNames;

    QList<QCustomPlot*> waves;
};

#endif // MAGNETIC_H
