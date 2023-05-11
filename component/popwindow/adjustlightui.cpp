#include "adjustlightui.h"
#include "ui_adjustlightui.h"
#include "component/myprogressbar/myprogressbar.h"
#include "qdebug.h"
#include "common/monitor/mymonitors.h"
#include "common/monitor/MonitorController.h"
#include "common/Util/FileUitl/FileUtil.h"
#include "settingwindow/settingwindow.h"
#include "common/Util/TimeUtil/TimeUtil.h"

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
        int screenIndex = FileUtil::getItem("display", "displayOrder", i, i).toInt();
        bool isHidden = FileUtil::getItem("display", "isHidden", screenIndex, false).toBool();
        if (isHidden) {
            MonitorBrightness light{};
            //这里是一个bug，不论是否显示进度条都要调用一下，不然打不开设置界面，暂时不知道原因。
            MonitorController::getBrightness(screen_list.at(screenIndex), light);
            continue;
        }
        auto *p1 = new MyProgressBar(this);
        p1->getProgressbar()->setMaximum(FileUtil::getValue("display", "max_brightness", 100).toInt());
        p1->getProgressbar()->setMinimum(FileUtil::getValue("display", "min_brightness", -50).toInt());
//        p1->getGamaProgressBar()->setValue(
//                FileUtil::getValue("display", QString("currentGama").append(QString::number(screenIndex)), 122).toInt());
        MonitorBrightness light{};
//        MonitorController::setGama(screen_list, screenIndex,
//                                   FileUtil::getValue("display", QString("currentGama").append(QString::number(screenIndex)),
//                                                      122).toInt());
        bool isSuccess = MonitorController::getBrightness(screen_list.at(screenIndex), light);
        int curr = FileUtil::getValue("display", QString("currentBrightness").append(QString::number(screenIndex)),
                                      (int) light.currentBrightness).toInt();
        if (isSuccess) {
            MonitorController::setBrightness(screen_list, screenIndex, light.currentBrightness);
//            MonitorController::setBrightness(screen_list, screenIndex, curr);

            qDebug() << "显示器亮度：" << light.currentBrightness;
        } else {
            qDebug() << "获取显示器 " << screenIndex << " 亮度失败" << light.currentBrightness;
        }
        p1->setValue(curr);
        p1->setName(QString("显示器%1").arg(screenIndex + 1));
        //监听亮度进度条变化
        connect(p1->getProgressbar(), &QSlider::valueChanged, this, [=](int value)mutable {
            if (MonitorController::setBrightness(screen_list, screenIndex, value)) {
                FileUtil::setValue("display", QString("currentBrightness").append(QString::number(screenIndex)), value);
                qDebug() << "亮度调节成功";
            } else {
                qDebug() << "亮度调节失败";
            }
        });
//        //监听Gama进度条变化
//        connect(p1->getGamaProgressBar(), &QSlider::valueChanged, this, [=](int value) {
//            if (MonitorController::setGama(screen_list, screenIndex, value)) {
//                FileUtil::setValue("display", QString("currentGama").append(QString::number(screenIndex)), value);
//                qDebug() << "Gama调节成功";
//            } else {
//                qDebug() << "Gama调节失败";
//            }
//        });
        ui->content->addWidget(p1);
    }
}


QPushButton *AdjustLightUi::getCloeButton() {
    return ui->close;
}

