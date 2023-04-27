//
// Created by LIXIN on 2023/4/26.
//

#include <QString>
#include <QFile>
#include <QSettings>
#include <QCoreApplication>
#include "FileUtil.h"
#include "QDir"
#include "QDebug"

//QDir::currentPath().append("/config")
static auto *settings = new QSettings(QCoreApplication::applicationDirPath() + "/Config.ini", QSettings::IniFormat);

QString FileUtil::read(QString path) {
    qDebug() << QDir::currentPath();
    path.append(QDir::homePath());
    if (!path.isEmpty()) {    //若文件路径不为空
        //创建QFile对象并指定路径
        QFile file(path);
        //一定要记得打开文件，并设置读写模式
        file.open(QIODevice::ReadOnly);
        //读取文件内容，一次性全部读入并保存在字节数组对象array中，默认只识别UTF-8编码的文件，其余编码格式会产生乱码
        QByteArray array;
        //若文件未达到结尾
        while (!file.atEnd()) {
            //逐行读取并拼接到字节数组array中
            array += file.readLine();
        }
        //记得关闭文件
        file.close();
        return QString::fromUtf8(array);
    }
    return "";
}

void FileUtil::save(QString path, QString text) {
    //创建一个文件对象，并关联路径
    QFile file;
    file.setFileName(path);
    //打开文件
    file.open(QIODevice::WriteOnly);
    //往文件中写入文本
    file.write(text.toUtf8());
    //记得关闭文件
    file.close();
}

QVariant FileUtil::getValue(QString group, QString key, QVariant defaultValue) {
    settings->beginGroup(group);
    QVariant variant = settings->value(key, defaultValue);
    settings->endGroup();
    return variant;
}

void FileUtil::setValue(QString group, QString key, QVariant value) {
    settings->beginGroup(group);
    settings->setValue(key, value);
    settings->endGroup();
    settings->sync();
}

void FileUtil::setItem(QString group, QString key, int index, QVariant value) {
    settings->beginWriteArray(group);
    settings->setArrayIndex(index);
    settings->setValue(key, value);
    settings->endArray();
    settings->sync();
}

QVariant FileUtil::getItem(QString group, QString key, int index, QVariant defaultValue) {
    settings->beginReadArray(group);
    settings->setArrayIndex(index);
    QVariant v = settings->value(key, defaultValue);
    settings->endArray();
    return v;
}
