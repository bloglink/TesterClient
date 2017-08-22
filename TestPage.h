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
#include <QPushButton>
#include <QToolButton>
#include <QColorDialog>
#include <QPalette>
#include <QColor>

#include "delegate.h"

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
private:
    QTableView *view;
    ItemModel *mView;

    QTableView *test;
    ItemModel *mTest;

    QTableView *pView;
    ItemModel *pModel;

    QLineEdit *typeLineEdit;
    QComboBox *typeComboBox;
    QLabel *typePixmap;

    QList<QPushButton *> buttons;
    QList<QLabel *> colors;

    QFrame *btnFrame;
    QToolButton *btnHide;

    QCustomPlot *wave;
    QCustomPlot *histogram;
};

#endif // TESTPAGE_H
