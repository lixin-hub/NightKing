//
// Created by LIXIN on 2023/4/21.
//

#include "MonitorController.h"
#include "QApplication"
#include "common/plans/WMI/WMI.h"
#include "common/plans/DDC_CI/DDC_CI.h"
#include "common/plans/Gamma/CGammaRamp.h"
#include "common/plans/IOCTL/Ioctl.h"

static QHash<HMONITOR, TransparentWindow *> transWindowMap;

bool MonitorController::getBrightness(HMONITOR hmonitor, MonitorBrightness &monitorBrightness) {
    auto *brightness = (MonitorBrightness *) malloc(sizeof(MonitorBrightness));
    bool isSuccess = DDC_CI::getMonitorBrightness(hmonitor, *brightness);
    if (isSuccess) {
        monitorBrightness = *brightness;
        return true;
    }
    return WMI::getBrightness(monitorBrightness);
}

bool MonitorController::setBrightness(const QList<HMONITOR> &hMonitors, int index, int value, bool useGama) {
    if (value >= 0) {
        if (transWindowMap.contains(hMonitors[index])) {
            transWindowMap.value(hMonitors[index])->hide();
//            transWindowMap.remove(hMonitors[index]);
        }
        if (DDC_CI::setBrightness(hMonitors.at(index), value)) {
            qDebug() << "使用DDC/CI：" << value;
            return true;
        } else if (WMI::setBrightness(value)) {
            qDebug() << "使用WMI";
            return true;
        } else if (Ioctl::setBrightness(value)) {
            qDebug() << "使用Ioctl";
            return true;
        }
    } else {
        if (useGama) {
            CGammaRamp cGammaRamp;
            if (cGammaRamp.SetBrightness(nullptr, 125)) {
                qDebug() << "使用CGamaRamp";
                return true;
            }
        }
        TransparentWindow *transparentWindow;
        qDebug() << "使用Mask";
        if (transWindowMap.contains(hMonitors[index])) {
            transparentWindow = transWindowMap.value(hMonitors[index]);

        } else {
            transparentWindow = new TransparentWindow(nullptr, hMonitors[index],index);
            transWindowMap.insert(hMonitors[index], transparentWindow);
        }

        if (!transparentWindow->isActiveWindow()) {
            transparentWindow->show();
        }
        transparentWindow->setBrightness(value);
        return true;
    }
    return false;
}

