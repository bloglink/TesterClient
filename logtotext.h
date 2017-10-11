#ifndef LOGTOTEXT
#define LOGTOTEXT
#include <QTextStream>
#include <QFile>
#include <QTime>

void debugMessageHandler(QtMsgType type, const QMessageLogContext &context, const QString &msg);

#endif // LOGTOTEXT

