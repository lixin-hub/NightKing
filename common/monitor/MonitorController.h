//
// Created by LIXIN on 2023/4/21.
//

#ifndef NIGHTKING_MONITORCONTROLLER_H
#define NIGHTKING_MONITORCONTROLLER_H

#include <QList>
#include "highlevelmonitorconfigurationapi.h"

class MonitorController {
public:

    //设置亮度值
    static bool setBrightness(const QList<HMONITOR>& hMonitors, int index, int value, bool useGama);

    static bool getBrightness(HMONITOR hmonitor, int &value);
};


#endif //NIGHTKING_MONITORCONTROLLER_H
