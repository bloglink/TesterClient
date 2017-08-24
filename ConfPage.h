#ifndef CONFPAGE_H
#define CONFPAGE_H

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

#include "conf_delegation.h"

class ConfPage : public QWidget
{
    Q_OBJECT
public:
    explicit ConfPage(QWidget *parent = 0);
    ~ConfPage();

signals:
    void sendNetMsg(QByteArray dat);
    void buttonClicked(QByteArray win);
public:
    void initTypes(QString dat);
    void initOther(QString dat);
private slots:
    void initUI();
    void saveData();
    void saveSys();
    void clickButton();
    void showButtons();
    void windowChange();
    void selectColor();
    void deleteItem();
    void autoPixmap(QString name);
    void recvAppShow(QString win);
    void appendType();
    void deleteType();
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
    QList<QPushButton *> colors;

    QFrame *btnFrame;
    QToolButton *btnHide;

    QStringList btnNames;


};

#endif // CONFPAGE_H
