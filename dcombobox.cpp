#include "dcombobox.h"

DComboBox::DComboBox(QObject *parent):QItemDelegate(parent)
{
}

QWidget *DComboBox::createEditor(QWidget *parent,
                                 const QStyleOptionViewItem &,
                                 const QModelIndex &) const
{
    QComboBox *editor = new QComboBox(parent);
    editor->setItemDelegate(new QStyledItemDelegate());
    for (int i=0; i < items.size(); i++)
        editor->addItem(items.at(i));
    return editor;
}

void DComboBox::setEditorData(QWidget *editor,const QModelIndex &index) const
{
    QString text = index.model()->data(index, Qt::EditRole).toString();
    QComboBox *comboBox = static_cast<QComboBox*>(editor);
    int tindex = comboBox->findText(text);
    comboBox->setCurrentIndex(tindex);
}

void DComboBox::setModelData(QWidget *editor, QAbstractItemModel *model,
                             const QModelIndex &index) const
{
    QComboBox *comboBox = static_cast<QComboBox*>(editor);
    QString text = comboBox->currentText();
    model->setData(index, text, Qt::EditRole);
}

void DComboBox::updateEditorGeometry(QWidget *editor,
                                     const QStyleOptionViewItem &option, const QModelIndex &) const
{
    editor->setGeometry(option.rect);
    QComboBox *comboBox = static_cast<QComboBox*>(editor);
    comboBox->showPopup();
}

void DComboBox::setItemNames(QStringList names) {
    items = names;
}

