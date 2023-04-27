#ifndef MONITORINFO_H
#define MONITORINFO_H

#include <QWidget>
#include "common/monitor/mymonitors.h"

namespace Ui {
    class MonitorInfo;
}

class MonitorInfo : public QWidget {
Q_OBJECT

public:
    MonitorInfoA monitorInfo{};
    QString adjustMethod;//调节方式
    QString name;
    bool isMask{};//是否开启屏幕遮罩
    explicit MonitorInfo(QWidget *parent = nullptr, QString name = "display", QString adjustMethod = "*const ",MonitorInfoA monitorInfoA={});

    ~MonitorInfo() override;

private:
    Ui::MonitorInfo *ui;
};

#endif // MONITORINFO_H
