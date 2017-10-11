#include "logtotext.h"

void debugMessageHandler(QtMsgType type, const QMessageLogContext &context, const QString &msg)
{
    QString message;
    QString newline = "\r\n";

    switch (type) {
    case QtDebugMsg:
        message = QString("调试信息: ");
        break;
    case QtWarningMsg:
        message = QString("警告信息: ");
        break;
    case QtCriticalMsg:
        message = QString("严重错误: ");
        break;
    case QtFatalMsg:
        message = QString("致命错误: ");
        break;
    case QtInfoMsg:
        break;
    default:
        break;
    }

    QString name = QString("%1.txt").arg(QDate::currentDate().toString("yyyy-MM-dd"));

    QFile log (name);
    log.open (QIODevice::WriteOnly | QIODevice::Append);

    QTextStream cout(&log);
    QString tstring = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss");
    message.append(QString("[%1] ").arg(tstring));
    message.append(QString("%1 ").arg(msg));
    QString lstring = QString::number(context.line, 10);
    message.append(QString("%1 (line: %2) ").arg(context.file).arg(lstring));
    message.append(QString("%1 ").arg(context.function));

    cout << message << newline;

    log.flush();
    log.close();
}
