//
// Created by LIXIN on 2023/4/26.
//

#ifndef NIGHTKINGSETTING_FILEUTIL_H
#define NIGHTKINGSETTING_FILEUTIL_H
#include "QString"
#include "QVariant"

class FileUtil {
public:

    static void save(QString path, QString text);
    static QVariant getValue(QString group, QString key, QVariant defaultValue);
    static QString read(QString path);
    static void setValue(QString group, QString key, QVariant value);
    static QVariant getItem(QString group, QString key, int index, const QVariant& defaultValue);
    static void setItem(QString group, QString key, int index, const QVariant& value);
    };

#endif //NIGHTKINGSETTING_FILEUTIL_H
