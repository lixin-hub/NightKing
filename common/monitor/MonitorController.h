//
// Created by LIXIN on 2023/4/21.
//

#ifndef NIGHTKING_MONITORCONTROLLER_H
#define NIGHTKING_MONITORCONTROLLER_H

#include <QList>
#include "highlevelmonitorconfigurationapi.h"
#include "mymonitors.h"
#include "common/plans/Mask/TransparentWindow.h"

class MonitorController {
public:

    //设置亮度值
    static bool setBrightness(const QList<HMONITOR> &hMonitors, int index, int value);
    static bool addBrightness(const QList<HMONITOR> &hMonitors, int index, int value);
    static bool subBrightness(const QList<HMONITOR> &hMonitors, int index, int value);

    static bool getBrightness(HMONITOR hmonitor, MonitorBrightness &monitorBrightness);

    //窗口和窗口遮罩
    static bool setGama(QList<HMONITOR> list, int i, int value);
    //启用夜间模式

    static void disableDarkMode(QList<HMONITOR> &monitors, int index, int value=30);
    //关闭夜间模式
    static  void enableDarkMode(QList<HMONITOR> &monitors, int index, int value=-50);
};


#endif //NIGHTKING_MONITORCONTROLLER_H
