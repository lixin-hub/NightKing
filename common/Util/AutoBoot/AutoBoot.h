//
// Created by LIXIN on 2023/4/29.
//

#ifndef NIGHTKING_AUTOBOOT_H
#define NIGHTKING_AUTOBOOT_H

#include <QDir>
#include <QSettings>
#include <QCoreApplication>

//设置程序自启动 appPath程序路径
void autoBoot() {
    QString appName = QCoreApplication::applicationName();//程序名称
    QString appPath = QCoreApplication::applicationFilePath();//程序路径
    appPath = appPath.replace("/", "\\");
    auto *reg = new QSettings(R"(HKEY_LOCAL_MACHINE\SOFTWARE\Microsoft\Windows\CurrentVersion\Run)",
                              QSettings::NativeFormat);

    QString val = reg->value(appName).toString();//如果此键不存在，则返回的是空字符串
    if (val != appPath) {
        reg->setValue(appName, appPath);
    }
    reg->deleteLater();
}

void closeAutoBoot() {
    QString appName = QCoreApplication::applicationName();//程序名称
    auto *reg = new QSettings(R"(HKEY_LOCAL_MACHINE\SOFTWARE\Microsoft\Windows\CurrentVersion\Run)",
                              QSettings::NativeFormat);
    if (reg->contains(appName))
        reg->remove(appName);
    reg->deleteLater();
}

#endif //NIGHTKING_AUTOBOOT_H
