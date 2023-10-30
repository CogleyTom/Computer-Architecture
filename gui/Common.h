/**
 * @file Common.h
 * Contains common functions.
 */

#ifndef COMMON_H
#define COMMON_H

#include <QString>
#include <QCoreApplication>
#include <QFile>
#include <QDir>

namespace Common {
    QString applicationDataPath();
    QString pathInTemp(QString path);
}

#endif // COMMON_H
