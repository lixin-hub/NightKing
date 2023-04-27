#ifndef MYMONITORS_H
#define MYMONITORS_H

#include "highlevelmonitorconfigurationapi.h"
#include "physicalmonitorenumerationapi.h"
#include "QList"
#include "QScreen"

struct MonitorBrightness {
    unsigned long maximumBrightness;
    unsigned long currentBrightness;
};
struct MonitorContrast {
    unsigned long maximumContrast;
    unsigned long currentContrast;
};
struct Monitor {
    HMONITOR hmonitor;
    QList<HANDLE> physicalHandles;
    DISPLAY_DEVICE displayDevice;
    QString id;
};
//所有信息
struct MonitorInfoA {
    DISPLAY_DEVICE displayDevice;
    MonitorBrightness monitorBrightness;
    MonitorContrast monitorContrast;
};

class MyMonitors {
public:
    MyMonitors();
    //获取HMonitor->HANDEL
    static QList<Monitor> getMonitors();
    //和上面一样只是改成Map的形式更方便
    static QHash<HMONITOR, struct Monitor> getMonitorMap();
    //获取所有HMONITOR
    static QList<HMONITOR> getHMonitors();
    //获取对应显示器的全部信息
    static MonitorInfoA getMonitorsInfoA(HMONITOR hmonitor);
    static QMap<QString, QScreen *> getScreenNameMap();
};

#endif // MYMONITORS_H
