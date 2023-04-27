#include <QApplication>
#include <QDebug>
#include <QWidget>
#include <Windows.h>
#include <PhysicalMonitorEnumerationAPI.h>
#include "winuser.h"
#include "mymonitors.h"
#include "MonitorController.h"

//获取所有的显示器
QList<HMONITOR> MyMonitors::getHMonitors() {
    QList<HMONITOR> monitors;
    //获取所有的Hmonitor
    auto monitorEnumProc = [](HMONITOR hMonitor, HDC hdcMonitor, LPRECT lprcMonitor, LPARAM dwData) -> BOOL {
        auto monitors = reinterpret_cast<QList<HMONITOR> *>(dwData);
        monitors->push_back(hMonitor);
        return TRUE;
    };
    EnumDisplayMonitors(nullptr, nullptr, monitorEnumProc, reinterpret_cast<LPARAM>(&monitors));
    for (auto &monitor: monitors) {
        qDebug() << "发现显示器:" << monitor;
    }
    return monitors;
}

QList<Monitor> MyMonitors::getMonitors() {
    QList<Monitor> monitors;
    //获取所有的Hmonitor
    QList<HMONITOR> hs = getHMonitors();
    for (int i = 0; i < hs.size(); ++i) {
        monitors.push_back({hs[i], {}});
    }
    //获取Monitor对应的PhyliciaHandel
    for (auto &monitor: monitors) {
        DWORD numPhysicalMonitors;//接收与监视器句柄关联的物理监视器数。
        LPPHYSICAL_MONITOR physicalMonitors;//指向物理显器handel，是一个数组，使用前要分配内存
        if (!GetNumberOfPhysicalMonitorsFromHMONITOR(monitor.hmonitor, &numPhysicalMonitors)) {
            throw std::runtime_error("Failed to get physical monitor count.");
            //TODO:修改为弹窗
        }
        physicalMonitors = new PHYSICAL_MONITOR[numPhysicalMonitors];
        //单个 HMONITOR 句柄可以与多个物理监视器相关联。 此函数返回每个物理监视器的句柄
        if (!GetPhysicalMonitorsFromHMONITOR(monitor.hmonitor, numPhysicalMonitors, physicalMonitors)) {
            throw std::runtime_error("Failed to get physical monitors.");
            //TODO:修改为弹窗
        }
        //遍历相关联的数组加入到QList
        for (DWORD i = 0; i < numPhysicalMonitors; i++) {
            monitor.physicalHandles.append(physicalMonitors[i].hPhysicalMonitor);
        }
        delete[] physicalMonitors;
    }
    /**----------获取每个显示器的DISPLAY_DEVICE--------**/
    DISPLAY_DEVICE adapterDev;
    adapterDev.cb = sizeof(DISPLAY_DEVICE);
    int adapterDevIndex = 0;//表示当前DEVICE的index索引
    //第一次循环得到所有的显示器DEVICE
    while (EnumDisplayDevices(nullptr, adapterDevIndex++, &adapterDev, 0)) {
        DISPLAY_DEVICE displayDev;
        displayDev.cb = sizeof(DISPLAY_DEVICE);
        int displayDevIndex = 0;
        //第二次循环根据第一次循环获取的DeviceName进行遍历，遍历2次是因为要先拿到名字，如果不用名字那么得不到id
        while (EnumDisplayDevices(adapterDev.DeviceName, displayDevIndex++, &displayDev,
                                  EDD_GET_DEVICE_INTERFACE_NAME)) {
            // 判断是不是应该合理的显示器，否则有可能会得到非法的显示器句柄
            if (!(displayDev.StateFlags & DISPLAY_DEVICE_ATTACHED_TO_DESKTOP) ||
                displayDev.StateFlags & DISPLAY_DEVICE_MIRRORING_DRIVER) {
                continue;
            }
            //这里要吧每个显示器的设备信息放入
            //通过monitors[i].DISPLAY_DEVICE;=displayDev;但是我们并不知道这个下标i因此需要通过显示器的名称来进行匹配
            for (auto &monitor: monitors) {
                MONITORINFOEX monitorInfo;
                monitorInfo.cbSize = sizeof(MONITORINFOEX);
                //这里的monitorInfo并不是我要的，但是它包含一个显示器名称,同时displayDevice也包含一个显示器名称，
                // 这样显示器的HMONITOR和DISPLAY_DEVICE就可以对应起来了;
                GetMonitorInfo(monitor.hmonitor, &monitorInfo);
                //这里就可以对比显示器的名称然后把名称相等的关联起来，但是这里的名称还有点区别，
                /* 形如
                 *qDebug()<<monitorInfo.szDevice;
                 *qDebug()<<displayDev.DeviceName;
                 *\\.\DISPLAY1
                 * \\.\DISPLAY1\Monitor0
                 *因此不能直接比较，还要做些处理
                 */
                //这个循环是获取每个HANDEL，根据hande的下标对显示器名称进行处理
                for (int i = 0; i < monitor.physicalHandles.size(); i++) {
                    //这里获得处理的显示器名称
                    QString monitorName = QString(monitorInfo.szDevice).append("\\Monitor").append(QString::number(i));
                    QString deviceName = QString(displayDev.DeviceName);
                    // Match and store against device ID
                    if (monitorName.compare(deviceName) == 0) {
                        QString id = QString(displayDev.DeviceID);
                        monitor.id = id;
                        monitor.displayDevice = displayDev;
                        break;
                    }
                }

            }
        }
    }
    return monitors;
}

QHash<HMONITOR, Monitor> MyMonitors::getMonitorMap() {
    QList<Monitor> l = getMonitors();
    QHash<HMONITOR, Monitor> map;
    for (auto monitor: l) {
        map.insert(monitor.hmonitor, monitor);
    }
    return map;
}

MyMonitors::MyMonitors() = default;

MonitorInfoA MyMonitors::getMonitorsInfoA(HMONITOR hmonitor) {
    MonitorInfoA monitorInfos;
    MonitorBrightness monitorBrightness;
    QHash<HMONITOR, Monitor> hMap = getMonitorMap();
    MonitorController::getBrightness(hmonitor, monitorBrightness);
    Monitor monitor = hMap.value(hmonitor);
    monitorInfos.displayDevice = monitor.displayDevice;
    monitorInfos.monitorBrightness = monitorBrightness;
    qDebug() << "获取显示器信息:" << hmonitor;
    qDebug() << "显示器名称：" << monitorInfos.displayDevice.DeviceName;
    qDebug() << "显示器ID：" << monitorInfos.displayDevice.DeviceID;
    qDebug() << "显示器描述：" << monitorInfos.displayDevice.DeviceString;
    qDebug() << "显示器key：" << monitorInfos.displayDevice.DeviceKey;
    qDebug() << "显示器当前亮度：" << monitorInfos.monitorBrightness.currentBrightness;
    qDebug() << "显示器最大亮度：" << monitorInfos.monitorBrightness.maximumBrightness;
    return monitorInfos;
}

QMap<QString, QScreen*> MyMonitors:: getScreenNameMap() {
    QMap<QString, QScreen *> map;
    for (const auto item: QGuiApplication::screens()) {
        map.insert(item->name(), item);
    }
    return map;
}
