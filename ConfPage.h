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

#include "delegate.h"

class ConfPage : public QWidget
{
    Q_OBJECT
public:
    explicit ConfPage(QWidget *parent = 0);
    ~ConfPage();

signals:
    void saveConfig(QByteArray dat);
    void buttonClicked(QByteArray win);
private slots:
    void initUI();
    void saveData();
    void clickButton();
    void showButtons();
    void windowChange();
    void selectColor();
    void deleteItem();
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
    QList<QPushButton *> colors;

    QFrame *btnFrame;
    QToolButton *btnHide;
};

#endif // CONFPAGE_H
