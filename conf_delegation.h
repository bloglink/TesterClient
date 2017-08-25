/*******************************************************************************
 * Copyright [2017]   <青岛艾普智能仪器有限公司>
 * All rights reserved.
 *
 * version:     0.1
 * author:      zhaonanlin
 * brief:       通用代理类
*******************************************************************************/
#ifndef DELEGATE_H
#define DELEGATE_H

#include <QItemDelegate>
#include <QStandardItemModel>
#include <QModelIndex>
#include <QObject>
#include <QSize>
#include <QSpinBox>
#include <QLineEdit>
#include <QComboBox>
#include <QApplication>
#include <QListView>

#include <QStyledItemDelegate>
#include <QDateEdit>
#include <QDoubleSpinBox>
#include <QCalendarWidget>
#include <QSqlTableModel>
#include <QSqlQueryModel>
#include <QSqlRelationalTableModel>

//编号列，只读委托
class ReadOnlyDelegate : public QItemDelegate
{
    Q_OBJECT
public:
    explicit ReadOnlyDelegate(QObject *parent = 0) : QItemDelegate(parent) { }
    QWidget *createEditor(QWidget*, const QStyleOptionViewItem &,
                          const QModelIndex &) const {
        return NULL;
    }
};

//代理类，把所有单元格中的字符居中显示
class StandardItem : public QStandardItemModel
{
    Q_OBJECT
public:
    explicit StandardItem(QObject *parent = NULL) : QStandardItemModel(parent) {}
    StandardItem(int row, int column, QObject *parent = NULL)
        : QStandardItemModel(row, column, parent) { }
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const {
        if (Qt::TextAlignmentRole == role)
            return Qt::AlignCenter;
        return QStandardItemModel::data(index, role);
    }
};

//利用 QComboBox 委托对输入进行限制
class ComboBox : public QItemDelegate
{
    Q_OBJECT
public:
    QStringList items;
    explicit ComboBox(QObject *parent = 0) : QItemDelegate(parent) {}
    QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &,
                          const QModelIndex &index) const {
        if (index.column() == 0)
            return 0;
        QComboBox *editor = new QComboBox(parent);
        editor->setItemDelegate(new QStyledItemDelegate());
        for (int i=0; i < items.size(); i++)
            editor->addItem(items.at(i));
        return editor;
    }
    void setEditorData(QWidget *editor, const QModelIndex &index) const {
        QString text = index.model()->data(index, Qt::EditRole).toString();
        QComboBox *comboBox = static_cast<QComboBox*>(editor);
        int tindex = comboBox->findText(text);
        comboBox->setCurrentIndex(tindex);
    }
    void setModelData(QWidget *editor, QAbstractItemModel *model,
                      const QModelIndex &index) const {
        QComboBox *comboBox = static_cast<QComboBox*>(editor);
        QString text = comboBox->currentText();
        model->setData(index, text, Qt::EditRole);
    }
    void updateEditorGeometry(QWidget *editor,
                              const QStyleOptionViewItem &option, const QModelIndex &) const {
        editor->setGeometry(option.rect);
        QComboBox *comboBox = static_cast<QComboBox*>(editor);
        comboBox->showPopup();
    }
    void setItemNames(QStringList names) {
        items = names;
    }
};

class SpinBox : public QItemDelegate
{
    Q_OBJECT
public:
    int high;
    explicit SpinBox(QObject *parent = 0) : QItemDelegate(parent) { }
    QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &,
                          const QModelIndex &) const {
        QSpinBox *editor = new QSpinBox(parent);
        editor->setMaximum(high);
        editor->setButtonSymbols(QAbstractSpinBox::NoButtons);
        return editor;
    }
    void setEditorData(QWidget *editor, const QModelIndex &index) const {
        int value = index.model()->data(index, Qt::EditRole).toInt();
        QSpinBox *spinBox = static_cast<QSpinBox*>(editor);
        spinBox->setValue(value);
    }
    void setModelData(QWidget *editor, QAbstractItemModel *model,
                      const QModelIndex &index) const {
        QSpinBox *spinBox = static_cast<QSpinBox*>(editor);
        spinBox->interpretText();
        int value = spinBox->value();
        model->setData(index, value, Qt::EditRole);
    }
    void updateEditorGeometry(QWidget *editor,
                              const QStyleOptionViewItem &option, const QModelIndex &) const {
        editor->setGeometry(option.rect);
    }
    void setMaxinum(int max) {
        high = max;
    }
};

class DoubleSpinBox : public QItemDelegate
{
    Q_OBJECT
public:
    double high;
    explicit DoubleSpinBox(QObject *parent = 0) : QItemDelegate(parent) { }
    QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &,
                          const QModelIndex &) const {
        QDoubleSpinBox *editor = new QDoubleSpinBox(parent);
        editor->setMaximum(high);
        editor->setButtonSymbols(QAbstractSpinBox::NoButtons);
        return editor;
    }
    void setEditorData(QWidget *editor, const QModelIndex &index) const {
        double value = index.model()->data(index, Qt::EditRole).toDouble();
        QDoubleSpinBox *spinBox = static_cast<QDoubleSpinBox*>(editor);
        spinBox->setValue(value);
    }
    void setModelData(QWidget *editor, QAbstractItemModel *model,
                      const QModelIndex &index) const {
        QDoubleSpinBox *spinBox = static_cast<QDoubleSpinBox*>(editor);
        spinBox->interpretText();
        double value = spinBox->value();
        model->setData(index, value, Qt::EditRole);
    }
    void updateEditorGeometry(QWidget *editor,
                              const QStyleOptionViewItem &option, const QModelIndex &) const {
        editor->setGeometry(option.rect);
    }
    void setMaxinum(double max) {
        high = max;
    }
};

#endif // DELEGATE_H
