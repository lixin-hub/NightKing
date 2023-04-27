//
// Created by LIXIN on 2023/4/26.
//

#include "JsonUtil.h"

QString JsonUtil::getStringProperty(QString json, QString prefix) {
    QJsonParseError err_rpt{};
    QJsonDocument root_Doc = QJsonDocument::fromJson(json.toUtf8(), &err_rpt); // 字符串格式化为JSON
    if (err_rpt.error != QJsonParseError::NoError) {
        qDebug() << "JSON格式错误";
        return "";
    } else    // JSON格式正确
    {
        QJsonObject root = root_Doc.object();
        return root.value(prefix).toString();
    }
}

int JsonUtil::getIntProperty(QString json, QString prefix) {
    return 0;
}

bool JsonUtil::getBoolProperty(QString json, QString prefix) {
    return false;
}
