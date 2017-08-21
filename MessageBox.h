/*******************************************************************************
 * Copyright [2017]   <青岛艾普智能仪器有限公司>
 * All rights reserved.
 *
 * version:     0.0.0.1
 * author:      zhaonanlin
 * brief:       警告弹出窗口
*******************************************************************************/
#ifndef MESSAGEBOX_H
#define MESSAGEBOX_H

#include <QVariant>
#include <QGridLayout>
#include <QMessageBox>
#include <QDialogButtonBox>

class PopupBox : public QDialog
{
    Q_OBJECT
public:
    explicit PopupBox(QWidget  *parent = 0, const QString &title = tr("Tip"),
                            const QString &text = "",
                            QMessageBox::StandardButtons buttons = QMessageBox::Ok,
                            QMessageBox::StandardButton defaultButton = QMessageBox::Ok);

    ~PopupBox();
protected:
    void changeEvent(QEvent *event);
private slots:
    void onButtonClicked(QAbstractButton *button);
    void TranslateButtonText(void);
    void ReadVariant(QVariant s);
private:
    QDialogButtonBox *ButtonBox;
};

#endif // MESSAGEBOX_H
