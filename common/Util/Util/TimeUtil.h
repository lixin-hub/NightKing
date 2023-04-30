//
// Created by LIXIN on 2023/4/29.
//

#ifndef NIGHTKING_TIMEUTIL_H
#define NIGHTKING_TIMEUTIL_H

#include <QDateTime>

int currentTimeMills() {
    QDateTime timeDate = QDateTime::currentDateTime();  // 获取当前时间
    return timeDate.toTime_t();                // 将当前时间转为时间戳
}

#endif //NIGHTKING_TIMEUTIL_H
