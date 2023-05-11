//
// Created by LIXIN on 2023/4/29.
//

#ifndef NIGHTKING_TIMEUTIL_H
#define NIGHTKING_TIMEUTIL_H

#include <QTimer>
#include <QDateTime>
#include "QMap"
#include "common/monitor/MonitorController.h"
#include "common/Util/FileUitl/FileUtil.h"


class TimeUtil {
public:


    uint currentTimeMills();

//输入目标时间点的小时和分钟

    static void startTimeCount();

    static void closeTimer();

    QTimer *createMyTimer(int h1, int m1, int h2, int m2);
};

//1 2 3 4 5 6 7 8 9 10 11 12
//13 14 15 16 17 18 19 20 21 22 23 24
#endif //NIGHTKING_TIMEUTIL_H
//13:10->14:11 23小时-1分钟+10
//13:10->14:9 23小时+1分钟+10
//37小时+11分钟

//                  《蝇》
//        陋室孤灯似无魂，冷陶区区伴为蝇。
//        百屋晴虹尤未灭，俟时虚左待故人。
//        何苦停箸不能食，举杯且邀镜中人。
//        忧思自有逍遥酿，黄金叶燎祛伤悲。
//        若君拂看盏边蝇，飘渺橫翔亦可停。