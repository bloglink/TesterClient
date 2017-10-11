/*******************************************************************************
 * Copyright [2016]   <  青岛艾普智能仪器有限公司>
 * All rights reserved.
 *
 * version:     2.1.0.170427
 * author:      zhaonanlin
 * brief:       电机综合测试仪
*******************************************************************************/
#include "logtotext.h"
#include "MainPage.h"
#include <QApplication>
#include <QTextCodec>
#include <QTranslator>
#include <QLibraryInfo>

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);

//    qInstallMessageHandler(debugMessageHandler);

    QTranslator qt_zh;
    qt_zh.load("qt_zh_CN.qm", ":/translations");
    a.installTranslator(&qt_zh);  // 加载Qt标准对话框的中文翻译文件

    MainPage w;
    if (!w.check())
        return 0;
    int ret = w.login();
    if (ret == QDialog::Rejected) {
        return 0;
    }
    if (ret == QDialog::Accepted) {
        w.readSettings();
        w.showMaximized();
        w.showWarnning();
    }

    return a.exec();
}
/*********************************END OF FILE**********************************/
