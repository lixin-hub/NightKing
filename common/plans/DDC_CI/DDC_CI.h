//
// Created by LIXIN on 2023/4/21.
//

#ifndef NIGHTKING_DDC_CI_H
#define NIGHTKING_DDC_CI_H

#include <Windows.h>
#include <PhysicalMonitorEnumerationAPI.h>
#include <HighLevelMonitorConfigurationAPI.h>
#include <QDebug>
#include "common/monitor/mymonitors.h"

using namespace std;


class DDC_CI {
public:

    static bool setBrightness(HMONITOR hMonitor, DWORD dw);

    [[maybe_unused]] static bool getMonitorContrast(HANDLE hMonitor, MonitorContrast &monitorContrast);

    static bool getMonitorBrightness(HMONITOR hMonitor, MonitorBrightness &brightness);

};

#endif //NIGHTKING_DDC_CI_H
