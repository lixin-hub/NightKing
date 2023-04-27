//
// Created by LIXIN on 2023/4/26.
//

#ifndef NIGHTKINGSETTING_JSONUTIL_H
#define NIGHTKINGSETTING_JSONUTIL_H
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

class JsonUtil {
public:
    static QString getStringProperty(QString json,QString prefix);
    static int getIntProperty(QString json,QString prefix);
    static bool getBoolProperty(QString json,QString prefix);

};


#endif //NIGHTKINGSETTING_JSONUTIL_H
