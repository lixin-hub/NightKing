//
// Created by LIXIN on 2023/5/4.
//
#include "TimeUtil.h"
#include "QDebug"

static QHash<HMONITOR, QTimer *> hashStartTimer;
static QHash<HMONITOR, QTimer *> hashStopTimer;

uint TimeUtil::currentTimeMills() {
    QDateTime timeDate = QDateTime::currentDateTime();  // 获取当前时间
    return timeDate.toTime_t();                // 将当前时间转为时间戳
}

//输入开始时间返回定时器
QTimer *creatStartTimer(int h1, int m1, int h2, int m2) {
    QTime currTime = QDateTime::currentDateTime().time();
    //当前时间在指定时间之前
    QTime start(h1, m1);
    QDateTime sd(QDate::currentDate(), start);//开始时间
    QDateTime cd(QDate::currentDate(), currTime);//当前时间
    QDateTime tw = sd.addDays(1);//明天开始时间
    qDebug() << "今天" << sd;
    qDebug() << "明天" << tw;
    auto timer = new QTimer();
    qint64 diff;
    if (start > currTime) {//curr--start---
        diff = qAbs(cd.msecsTo(sd));
    } else {//--end--curr
        diff = qAbs(cd.msecsTo(tw));
    }
    qDebug() << "创建开始定时任务，定时时间至：" << cd.addMSecs(diff);
    timer->start(diff);
    return timer;
}

//输入结束时间返回定时器，或者
QTimer *creatEndTimer(int h1, int m1) {
    QTime currTime = QDateTime::currentDateTime().time();
    //当前时间在指定时间之前
    QTime end(h1, m1);
    QDateTime ed(QDate::currentDate(), end);
    QDateTime cd(QDate::currentDate(), currTime);
    QDateTime tw = ed.addDays(1);
    auto timer = new QTimer();
    qint64 diff;
    if (currTime < end) {//  -----curr------end-
//        int diff = (h1 - h) * 60 * 60 * 100 + (m1 - m) * 60 * 100;
        diff = qAbs(cd.msecsTo(ed));
    } else {//-end --- curr
        diff = qAbs(cd.msecsTo(tw));
    }
    qDebug() << "创建结束定时任务，定时时间至：" << cd.addMSecs(diff);
    timer->start((int) diff);
    return timer;
}

void TimeUtil::startTimeCount() {
    QList<HMONITOR> monitorList = MyMonitors::getHMonitors();
    closeTimer();
    //拿到所以显示器
    int monitorSize = monitorList.size();
    for (int i = 0; i < monitorSize; ++i) {
        HMONITOR hmonitor = monitorList[i];
        /*//获取每个显示器是否开启了黑夜模式
        bool isEnableDarkModeIndex = FileUtil::getItem("time", "isEnableDarkModeIndex", i, false).toBool();
        if (!isEnableDarkModeIndex) {//如果没有开启夜间模式,要看看是不是之前创建过定时器，吧他清除掉
            if (hashStartTimer.contains(hmonitor)) {
                QTimer *timer = hashStartTimer.value(hmonitor);
                timer->stop();
                timer->deleteLater();
                hashStartTimer.remove(hmonitor);
            }
            if (hashStopTimer.contains(hmonitor)) {
                QTimer *timer = hashStopTimer.value(hmonitor);
                timer->stop();
                timer->deleteLater();
                hashStopTimer.remove(hmonitor);
            }
            continue;
        }*/
        //获取开启和关闭黑夜模式的时间点
        int startTimeH = FileUtil::getItem("time", "darkModeStartTimeHour", i, -1).toInt();
        int startTimeM = FileUtil::getItem("time", "darkModeStartTimeMin", i, -1).toInt();
        int endTimeH = FileUtil::getItem("time", "darkModeEndTimeHour", i, -1).toInt();
        int endTimeM = FileUtil::getItem("time", "darkModeEndTimeMin", i, -1).toInt();
        bool isEnableDarkModeIndex = FileUtil::getItem("time", "isEnableDarkModeIndex", i, false).toBool();
        //开始时间的定时器
        if (startTimeH != -1 && startTimeM != -1 && isEnableDarkModeIndex) {
            QTimer *startTimer = creatStartTimer(startTimeH, startTimeM, endTimeH, endTimeM);
            hashStartTimer.insert(hmonitor, startTimer);
            QTime currTime = QDateTime::currentDateTime().time();
            //当前时间在指定时间之前
            QTime start(startTimeH, startTimeM);
            QTime end(endTimeH, endTimeM);
            if (currTime > start && currTime < end) {//--start---curr--end,这个时候需要执行一次
                bool isEnable = FileUtil::getItem("time", "isEnableDarkModeIndex", i, false).toBool();
                if (isEnable) {
                    MonitorController::enableDarkMode(monitorList, i);
                }
            }
            QObject::connect(startTimer, &QTimer::timeout, [=]() mutable {
                qDebug() << "开启夜间模式";
                monitorList = MyMonitors::getHMonitors();
                MonitorBrightness monitorBrightness{};
                MonitorController::getBrightness(hmonitor, monitorBrightness);
                FileUtil::setItem("time", "normalBrightness", i, (int) monitorBrightness.currentBrightness);
                bool isEnableDarkModeIndex = FileUtil::getItem("time", "isEnableDarkModeIndex", i, false).toBool();
                if (isEnableDarkModeIndex) {
                    qDebug() << "显示器：" << i;
                    MonitorController::enableDarkMode(monitorList, i);
                }
                //完成了黑夜模式的启动，现在开始新的任务
                startTimeCount();
            });
        }

        if (endTimeH != -1 && endTimeM != -1 && isEnableDarkModeIndex) {
            //结束时间的定时器
            QTimer *endTimer = creatEndTimer(endTimeH, endTimeM);
            hashStopTimer.insert(hmonitor, endTimer);
            QObject::connect(endTimer, &QTimer::timeout, [=]() mutable {
                qDebug() << "关闭夜间模式";
                bool forAll = FileUtil::getValue("time", "forAllDisplay", true).toBool();
                monitorList = MyMonitors::getHMonitors();
                if (forAll) {
                    for (int j = 0; j < monitorList.size(); j++) {
                        int value = FileUtil::getItem("time", "normalBrightness", j, 50).toInt();
                        MonitorController::disableDarkMode(monitorList, j, value);
                    }
                } else {
                    int value = FileUtil::getItem("time", "normalBrightness", i, 50).toInt();
                    MonitorController::disableDarkMode(monitorList, i, value);
                }
                //完成了黑夜模式的关闭，现在开始新的任务
                startTimeCount();
            });
        }
    }

}

void TimeUtil::closeTimer() {
    if (!hashStartTimer.isEmpty()) {
        for (const auto &item: hashStartTimer) {
            item->stop();
            item->deleteLater();
        }
        hashStartTimer.clear();
    }
    if (!hashStopTimer.isEmpty()) {
        for (const auto &item: hashStopTimer) {
            item->stop();
            item->deleteLater();
        }
        hashStopTimer.clear();
    }
}
