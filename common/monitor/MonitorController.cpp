//
// Created by LIXIN on 2023/4/21.
//

#include "MonitorController.h"
#include "QApplication"
#include "common/plans/WMI/WMI.h"
#include "common/plans/DDC_CI/DDC_CI.h"
#include "common/plans/Gamma/CGammaRamp.h"
#include "common/plans/IOCTL/Ioctl.h"

bool MonitorController::getBrightness(HMONITOR hmonitor, int &value) {
    auto *monitorBrightness = (MonitorBrightness *) malloc(sizeof(MonitorBrightness));
    bool isSuccess = DDC_CI::getMonitorBrightness(hmonitor, monitorBrightness);
    if (isSuccess) {
        value =(int) monitorBrightness->currentBrightness;
        return true;
    }
    return WMI::getBrightness(&value);
}

bool MonitorController::setBrightness(const QList<HMONITOR>& hMonitors, int index, int value, bool useGama) {
    if (DDC_CI::setBrightness(hMonitors.at(index), value)) {
        qDebug() << "使用DDC/CI";
        return true;
    } else if (WMI::setBrightness(value)) {
        qDebug() << "使用WMI";
        return true;
    } else if (Ioctl::setBrightness(value)) {
        qDebug() << "使用Ioctl";
        return true;
    }
    if (useGama) {
        CGammaRamp cGammaRamp;
        if (cGammaRamp.SetBrightness(nullptr, value)) {
            qDebug() << "使用CGamaRamp";
            return true;
        }
    }
    return false;
}
