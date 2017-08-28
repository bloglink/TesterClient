/*******************************************************************************
 * Copyright [2017]   <青岛艾普智能仪器有限公司>
 * All rights reserved.
 *
 * version:     0.0.0.1
 * author:      zhaonanlin
 * brief:       测试界面
*******************************************************************************/
#ifndef TESTPAGE_H
#define TESTPAGE_H

#include <QWidget>
#include <QTableView>
#include <QHeaderView>
#include <QLayout>
#include <QCheckBox>
#include <QLineEdit>
#include <QComboBox>
#include <QPushButton>
#include <QDebug>
#include <QtXml/QDomDocument>
#include <QXmlStreamWriter>
#include <QXmlStreamReader>
#include <QLabel>
#include <QGroupBox>
#include <QToolButton>
#include <QColorDialog>
#include <QPalette>
#include <QColor>

#include "conf_delegation.h"
#include "conf_qrencode.h"

#include "qcustomplot.h"

#define PI 3.1415936

class TestPage : public QWidget
{
    Q_OBJECT
public:
    explicit TestPage(QWidget *parent = 0);
    ~TestPage();

signals:
    void sendNetMsg(QByteArray dat);
    void buttonClicked(QByteArray win);
    void buttonTest();
    void buttonTest1();
    void buttonTest2();
    void buttonTest3();
    void buttonTest4();
    void buttonTest5();
    void buttonTest6();
public slots:
    void initItems();
    void updateItems(QString items);
private slots:
    void initUI();
    void saveData();
    void clickButton();
    void showButtons();
    void windowChange();
    void recvAppShow(QString win);
    void selectColor();
    void deleteItem();
    void DrawHistogram();
    void DrawWave();
    void Printer();
    void printPreview(QPrinter *printer);
private:
    QTableView *view;
    StandardItem *mView;

    QTableView *test;
    StandardItem *mTest;

    QTableView *pView;
    StandardItem *pModel;

    QLineEdit *typeLineEdit;
    QComboBox *typeComboBox;
    QLabel *typePixmap;

    QList<QPushButton *> buttons;
    QList<QLabel *> colors;

    QFrame *btnFrame;
    QToolButton *btnHide;

    QCustomPlot *wave;
    QCustomPlot *histogram;

    ConfQrenCode *qrencode;
};

#endif // TESTPAGE_H
