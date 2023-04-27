//
// Created by LIXIN on 2023/4/21.
//

#include "DDC_CI.h"
#include "common/monitor/mymonitors.h"
bool DDC_CI::getMonitorBrightness(HMONITOR hMonitor, MonitorBrightness &brightness) {
    DWORD minimumBrightness;
    DWORD maximumBrightness;
    DWORD currentBrightness;
    QList<Monitor> monitors=MyMonitors::getMonitors();
    for (auto & monitor : monitors){
        if (hMonitor==monitor.hmonitor) {
            if (!GetMonitorBrightness(monitor.physicalHandles[0], &minimumBrightness, &currentBrightness, &maximumBrightness)) {
                return false;
            }
        }
    }
    brightness = {static_cast<unsigned long>(maximumBrightness), static_cast<unsigned long>(currentBrightness)};
    return true;
}

[[maybe_unused]] bool DDC_CI::getMonitorContrast(HANDLE hMonitor, MonitorContrast &contrast) {
    DWORD minimumContrast;
    DWORD maximumContrast;
    DWORD currentContrast;
    if (!GetMonitorContrast(hMonitor, &minimumContrast, &currentContrast, &maximumContrast)) {
        //TODO:换成弹窗
        return false;
    }


    contrast = {static_cast<unsigned long>(maximumContrast), static_cast<unsigned long>(currentContrast)};

    return true;
}

bool DDC_CI::setBrightness(HMONITOR hMonitor, DWORD dw) {
    LPDWORD pdwCurrentBrightness;
    LPDWORD pdwMinimumBrightness;
    LPDWORD pdwMaximumBrightness;
    GetMonitorBrightness(hMonitor, pdwMinimumBrightness, pdwCurrentBrightness, pdwMaximumBrightness);
    if (pdwCurrentBrightness == nullptr) {
        return false;
    }
    bool isSuccess = false;
    DWORD cPhysicalMonitors;
    LPPHYSICAL_MONITOR pPhysicalMonitors;
    if (GetNumberOfPhysicalMonitorsFromHMONITOR(hMonitor, &cPhysicalMonitors)) {
        pPhysicalMonitors = (LPPHYSICAL_MONITOR) malloc(cPhysicalMonitors * sizeof(PHYSICAL_MONITOR));
        if (GetPhysicalMonitorsFromHMONITOR(hMonitor, cPhysicalMonitors, pPhysicalMonitors)) {
            isSuccess = SetMonitorBrightness(pPhysicalMonitors[0].hPhysicalMonitor, dw);
        }
        free(pPhysicalMonitors);
    } else {
        return false;
    }
    return isSuccess;
}
