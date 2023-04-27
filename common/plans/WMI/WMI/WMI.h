//
// Created by LIXIN on 2023/4/20.
//

#ifndef NIGHTKING_WMI_H
#define NIGHTKING_WMI_H
#include <windows.h>
#include <physicalmonitorenumerationapi.h>
#include <string>
#include <highlevelmonitorconfigurationapi.h>
#include <QDebug>
#include <comdef.h>
#include "combaseapi.h"
#include "objbase.h"
#include "common/monitor/mymonitors.h"
#define _WIN32_DCOM
class WMI {
public:
static bool setBrightness(int bValue);
static bool getBrightness(MonitorBrightness &monitorBrightness);

~WMI();
};

#endif //NIGHTKING_WMI_H
