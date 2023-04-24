#include "common/plans/Gamma/CGammaRamp.h"
#include "adjustlightui.h"
#include "ui_adjustlightui.h"
#include "page/component/myprogressbar/myprogressbar.h"
#include "qdebug.h"
#include <common/monitor/mymonitors.h>
#include "common/monitor/MonitorController.h"

AdjustLightUi::AdjustLightUi(QWidget *parent) : QWidget(parent), ui(new Ui::AdjustLightUi) {
    ui->setupUi(this);
    QList<HMONITOR> screen_list = MyMonitors::getHMonitors();
    for (int i = 0; i < screen_list.count(); i++) {
        auto *p1 = new MyProgressBar(this);
        int light;
        bool isSuccess = MonitorController::getBrightness(screen_list.at(i), light);
        if (isSuccess) {
            p1->setValue(light);
            qDebug() << "显示器亮度：" << light;
        } else
            qDebug() << "获取显示器 " << i << " 亮度失败" << light;
        p1->setName(QString("显示器%1").arg(i + 1));
        connect(p1->getProgressbar(), &QSlider::valueChanged, this, [=](int value)mutable {
            if (MonitorController::setBrightness(screen_list, i, value, true)) {
                qDebug() << "亮度调节成功";
            } else {
                qDebug() << "亮度调节失败";
            }
        });
        ui->content->addWidget(p1);
    }
}


AdjustLightUi::~AdjustLightUi() {
    delete ui;
}

