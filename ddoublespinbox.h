#ifndef DDOUBLESPINBOX_H
#define DDOUBLESPINBOX_H

#include <QObject>
#include <QModelIndex>
#include <QDoubleSpinBox>
#include <QItemDelegate>
#include <QStyledItemDelegate>

class DDoubleSpinBox : public QItemDelegate
{
    Q_OBJECT

public:
    explicit DDoubleSpinBox(QObject *parent = 0);

    QWidget *createEditor(QWidget *parent,
                          const QStyleOptionViewItem &option,
                          const QModelIndex &index) const;

    void setEditorData(QWidget *editor, const QModelIndex &index) const;
    void setModelData(QWidget *editor,
                      QAbstractItemModel *model,
                      const QModelIndex &index) const;

    void updateEditorGeometry(QWidget *editor,
                              const QStyleOptionViewItem &option,
                              const QModelIndex &index) const;
public slots:
    void setMaxinum(double max);
    void setDecimals(int prec);
private:
    double max;
    int prec;
};

#endif // DDOUBLESPINBOX_H
