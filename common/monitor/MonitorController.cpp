//
// Created by LIXIN on 2023/4/21.
//

#include <QDateTime>
#include "MonitorController.h"
#include "QApplication"
#include "common/plans/WMI/WMI.h"
#include "common/plans/DDC_CI/DDC_CI.h"
#include "common/plans/Gamma/CGammaRamp.h"
#include "common/plans/IOCTL/Ioctl.h"
#include "QTimer"
#include "common/Util/FileUitl/FileUtil.h"

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

bool MonitorController::setBrightness(const QList<HMONITOR> &hMonitors, int index, int value) {
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
//        //将亮度调整为负数的时候首先要将屏幕亮度先调整为0，然后再调整为负数
//        MonitorBrightness monitorBrightness{};
//        getBrightness(hMonitors[index], monitorBrightness);
//        if (monitorBrightness.currentBrightness > 0) {
//            setBrightness(hMonitors, index, 0);
//        }
        TransparentWindow *transparentWindow;
        qDebug() << "使用Mask";
        if (transWindowMap.contains(hMonitors[index])) {
            transparentWindow = transWindowMap.value(hMonitors[index]);

        } else {
            transparentWindow = new TransparentWindow(nullptr, hMonitors[index], index);
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

bool MonitorController::setGama(QList<HMONITOR> list, int i, int value) {
    CGammaRamp cGammaRamp;
    if (cGammaRamp.SetBrightness(nullptr, value)) {
        qDebug() << "使用CGamaRamp";
        return true;
    }
    return false;
}

void MonitorController::enableDarkMode(QList<HMONITOR> &monitors, int index, int value) {
//    将亮度调整为负数的时候首先要将屏幕亮度先调整为0，然后再调整为负数
    MonitorBrightness monitorBrightness{};
    getBrightness(monitors[index], monitorBrightness);
    if (monitorBrightness.currentBrightness > 0) {
        setBrightness(monitors, index, 0);
    }
    int darkValue = FileUtil::getItem("time", "darkModeValue", index, value).toInt();
    MonitorController::setBrightness(monitors, index, value);
    FileUtil::setValue("display", QString("currentBrightness").append(QString::number(index)), darkValue);
}

void MonitorController::disableDarkMode(QList<HMONITOR> &monitors, int index, int value) {
    MonitorBrightness monitorBrightness{};
    getBrightness(monitors[index], monitorBrightness);
    if (monitorBrightness.currentBrightness > 0) {
        setBrightness(monitors, index, 0);
    }
    int darkValue = FileUtil::getItem("time", "darkModeValue", index, value).toInt();
    MonitorController::setBrightness(monitors, index, darkValue);
    FileUtil::setValue("display", QString("currentBrightness").append(QString::number(index)), darkValue);
}

bool MonitorController::addBrightness(const QList<HMONITOR> &monitors, int index, int value) {
    MonitorBrightness monitorBrightness{};
    getBrightness(monitors[index], monitorBrightness);
    int darkValue = FileUtil::getItem("display", "currentBrightness", index,
                                      (int) monitorBrightness.currentBrightness).toInt() + value;
    MonitorController::setBrightness(monitors, index, darkValue);
    FileUtil::setValue("display", QString("currentBrightness").append(QString::number(index)), darkValue);
    return true;
}

bool MonitorController::subBrightness(const QList<HMONITOR> &hMonitors, int index, int value) {
    return addBrightness(hMonitors, index, 0 - value);
}


