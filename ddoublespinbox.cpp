#include "ddoublespinbox.h"

DDoubleSpinBox::DDoubleSpinBox(QObject *parent):QItemDelegate(parent)
{
    max = 9999.99;
    prec = 2;
}

QWidget *DDoubleSpinBox::createEditor(QWidget *parent,
                                const QStyleOptionViewItem &,
                                const QModelIndex &) const
{
    QDoubleSpinBox *editor = new QDoubleSpinBox(parent);
    editor->setMaximum(max);
    editor->setDecimals(prec);
    editor->setButtonSymbols(QAbstractSpinBox::NoButtons);
    return editor;
}

void DDoubleSpinBox::setEditorData(QWidget *editor,const QModelIndex &index) const
{
    int value = index.model()->data(index, Qt::EditRole).toInt();
    QDoubleSpinBox *spinBox = static_cast<QDoubleSpinBox*>(editor);
    spinBox->setValue(value);
}

void DDoubleSpinBox::setModelData(QWidget *editor, QAbstractItemModel *model,
                            const QModelIndex &index) const
{
    QDoubleSpinBox *spinBox = static_cast<QDoubleSpinBox*>(editor);
    spinBox->interpretText();
    int value = spinBox->value();
    model->setData(index, value, Qt::EditRole);
}

void DDoubleSpinBox::updateEditorGeometry(QWidget *editor,
                                    const QStyleOptionViewItem &option, const QModelIndex &) const
{
    editor->setGeometry(option.rect);
}

void DDoubleSpinBox::setMaxinum(double m)
{
    max = m;
}

void DDoubleSpinBox::setDecimals(int p)
{
    prec = p;
}


