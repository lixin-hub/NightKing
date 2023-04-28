#include "adjustlightui.h"
#include "ui_adjustlightui.h"
#include "component/myprogressbar/myprogressbar.h"
#include "qdebug.h"
#include "common/monitor/mymonitors.h"
#include "common/monitor/MonitorController.h"
#include "common/Util/FileUitl/FileUtil.h"
#include "settingwindow/settingwindow.h"

AdjustLightUi::AdjustLightUi(QWidget *parent) : QWidget(parent), ui(new Ui::AdjustLightUi) {
    ui->setupUi(this);
    init();
}

AdjustLightUi::~AdjustLightUi() {
    delete ui;
}

void AdjustLightUi::init() {
    connect(ui->settings, &QPushButton::clicked, [=]() {
        auto *w = new SettingWindow(nullptr);
        w->show();
    });
    QList<HMONITOR> screen_list = MyMonitors::getHMonitors();
    for (int i = 0; i < screen_list.count(); i++) {
        auto *p1 = new MyProgressBar(this);
        qDebug() << "min_brightness:" << FileUtil::getValue("display", "min_brightness", -50).toInt();
        p1->getProgressbar()->setMaximum(FileUtil::getValue("display", "max_brightness", 100).toInt());
        p1->getProgressbar()->setMinimum(FileUtil::getValue("display", "min_brightness", -50).toInt());
        MonitorBrightness light{};
        bool isSuccess = MonitorController::getBrightness(screen_list.at(i), light);
        int curr = FileUtil::getValue("display", QString("currentBrightness").append(i),
                                      (int) light.currentBrightness).toInt();
        if (isSuccess) {
            p1->setValue(curr);
            MonitorController::setBrightness(screen_list, i, curr, false);
            qDebug() << "显示器亮度：" << light.currentBrightness;
        } else
            qDebug() << "获取显示器 " << i << " 亮度失败" << light.currentBrightness;
        p1->setName(QString("显示器%1").arg(i + 1));
        connect(p1->getProgressbar(), &QSlider::valueChanged, this, [=](int value)mutable {
            if (MonitorController::setBrightness(screen_list, i, value, false)) {
                FileUtil::setValue("display", QString("currentBrightness").append(i), value);
                qDebug() << "亮度调节成功";
            } else {
                qDebug() << "亮度调节失败";
            }
        });
        ui->content->addWidget(p1);
    }
}


QPushButton *AdjustLightUi::getCloeButton() {
    return ui->close;
}

