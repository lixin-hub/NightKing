#ifndef MYMONITORS_H
#define MYMONITORS_H

#include "highlevelmonitorconfigurationapi.h"
#include "physicalmonitorenumerationapi.h"
#include "QList"

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

    //获取所有HMONITOR
    static QList<HMONITOR> getHMonitors();
    static QList<MonitorInfoA> getMonitorsInfoA();

};

#endif // MYMONITORS_H
