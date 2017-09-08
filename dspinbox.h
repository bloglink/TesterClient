#ifndef DSPINBOX_H
#define DSPINBOX_H

#include <QObject>
#include <QSpinBox>
#include <QModelIndex>
#include <QItemDelegate>
#include <QStyledItemDelegate>

class DSpinBox : public QItemDelegate
{
    Q_OBJECT

public:
    explicit DSpinBox(QObject *parent = 0);

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
    void setMaxinum(int max);
private:
    int max;
};

#endif // DSPINBOX_H
