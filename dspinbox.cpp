#include "dspinbox.h"

DSpinBox::DSpinBox(QObject *parent):QItemDelegate(parent)
{
    max = 99;
}

QWidget *DSpinBox::createEditor(QWidget *parent,
                                const QStyleOptionViewItem &,
                                const QModelIndex &) const
{
    QSpinBox *editor = new QSpinBox(parent);
    editor->setMaximum(max);
    editor->setButtonSymbols(QAbstractSpinBox::NoButtons);
    return editor;
}

void DSpinBox::setEditorData(QWidget *editor,const QModelIndex &index) const
{
    int value = index.model()->data(index, Qt::EditRole).toInt();
    QSpinBox *spinBox = static_cast<QSpinBox*>(editor);
    spinBox->setValue(value);
}

void DSpinBox::setModelData(QWidget *editor, QAbstractItemModel *model,
                            const QModelIndex &index) const
{
    QSpinBox *spinBox = static_cast<QSpinBox*>(editor);
    spinBox->interpretText();
    int value = spinBox->value();
    model->setData(index, value, Qt::EditRole);
}

void DSpinBox::updateEditorGeometry(QWidget *editor,
                                    const QStyleOptionViewItem &option, const QModelIndex &) const
{
    editor->setGeometry(option.rect);
}

void DSpinBox::setMaxinum(int m) {
    max = m;
}
