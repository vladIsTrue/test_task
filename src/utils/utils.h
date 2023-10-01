#ifndef UTILS_H
#define UTILS_H

#include <QDateTime>

#define LOGI(...) (qInfo()      << "Info:\t["   \
                                << QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss.zzz ") << "] " << __VA_ARGS__)
#define LOGD(...) (qDebug()     << "Debug:\t["  \
                                << QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss.zzz ") << "] " << __VA_ARGS__)
#define LOGE(...) (qCritical()  << "Error:\t["  \
                                << QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss.zzz ") << "] " << __VA_ARGS__)

#endif // UTILS_H
