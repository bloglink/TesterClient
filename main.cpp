/*******************************************************************************
 * Copyright [2016]   <  青岛艾普智能仪器有限公司>
 * All rights reserved.
 *
 * version:     2.1.0.170427
 * author:      zhaonanlin
 * brief:       电机综合测试仪
*******************************************************************************/

#include "MainPage.h"
#include <QApplication>
#include <QTextCodec>

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);
    MainPage w;
    int ret = w.login();
    if (ret == QDialog::Rejected) {
        return 0;
    }
    if (ret == QDialog::Accepted) {
        w.readSettings();
        w.showMaximized();
    }

    return a.exec();
}
/*********************************END OF FILE**********************************/
