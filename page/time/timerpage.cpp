#include "timerpage.h"
#include "ui_timerpage.h"
#include "QPushButton"
#include "component/TimerPoint/timepoint.h"
#include "common/monitor/mymonitors.h"
#include "QDebug"
#include "common/Util/FileUitl/FileUtil.h"
#include "common/Util/TimeUtil/TimeUtil.h"

TimerPage::TimerPage(QWidget *parent) : QWidget(parent), ui(new Ui::TimerPage) {
    ui->setupUi(this);
    ui->displaySelector->setCurrentIndex(0);
    initState(0);
    //-------------------------------------
    QList<HMONITOR> monitorList = MyMonitors::getHMonitors();
    for (int i = 0; i < monitorList.size(); ++i) {
        ui->displaySelector->addItem(QString("显示器").append(QString::number(i + 1)));
    }
    //切换显示器
    connect(ui->displaySelector, qOverload<int>(&QComboBox::currentIndexChanged), [=](int index) {
        bool forAll = ui->forAllDisplay->isChecked();
        if (forAll) {
            ui->displaySelector->setEnabled(false);
        }
        initState(index);
    });

    //应用到全部显示器
    connect(ui->forAllDisplay, &QCheckBox::toggled, [=]() {
        ui->displaySelector->setEnabled(!ui->forAllDisplay->isChecked());
        FileUtil::setValue("time", "forAllDisplay", ui->forAllDisplay->isChecked());
    });
    //启用对应显示器的夜间模式
    connect(ui->enableDarkMode, &QCheckBox::toggled, [=]() {

    });
    //开始时间
    connect(ui->startTime, &QTimeEdit::timeChanged, [=](QTime time) {

    });
    //结束时间
    connect(ui->endTime, &QTimeEdit::timeChanged, [=](QTime time) {

    });

    //应用设置
    connect(ui->apply, &QPushButton::clicked, [=]() {
        TimeUtil::closeTimer();
        bool forAllDisplay = ui->forAllDisplay->isChecked();
        bool isEnable = ui->enableDarkMode->isChecked();
        if (forAllDisplay) {
            //应用到了全部显示器,那么对所有显示器启用或者不启用
            QList<HMONITOR> monitorList = MyMonitors::getHMonitors();
            for (int i = 0; i < monitorList.size(); ++i) {
                QTime st = ui->startTime->time();
                QTime et = ui->endTime->time();
                FileUtil::setItem("time", "isEnableDarkModeIndex", i, isEnable);
                FileUtil::setItem("time", "darkModeValue", i, ui->darkModeValue->value());
                FileUtil::setItem("time", "darkModeStartTimeHour", i, st.hour());
                FileUtil::setItem("time", "darkModeStartTimeMin", i, st.minute());
                FileUtil::setItem("time", "darkModeEndTimeHour", i, et.hour());
                FileUtil::setItem("time", "darkModeEndTimeMin", i, et.minute());
            }
        } else {
            int index = ui->displaySelector->currentIndex();
            QTime st = ui->startTime->time();
            QTime et = ui->endTime->time();
            FileUtil::setItem("time", "isEnableDarkModeIndex", index, isEnable);
            FileUtil::setItem("time", "darkModeValue", index, ui->darkModeValue->value());
            FileUtil::setItem("time", "darkModeStartTimeHour", index, st.hour());
            FileUtil::setItem("time", "darkModeStartTimeMin", index, st.minute());
            FileUtil::setItem("time", "darkModeEndTimeHour", index, et.hour());
            FileUtil::setItem("time", "darkModeEndTimeMin", index, et.minute());
        }
        TimeUtil::startTimeCount();
    });
    auto *timePointLayout = new QVBoxLayout();
    timePointLayout->setMargin(0);
    ui->timeLayoutWidget->setLayout(timePointLayout);
    connect(ui->addTimePoint, &QPushButton::clicked, [=]() {
        auto *timePoint = new TimePoint(this);
        timePointLayout->addWidget(timePoint);
    });

}

void TimerPage::initState(int index) {
    QTime st(FileUtil::getItem("time", "darkModeStartTimeHour", index, 19).toInt(),
             FileUtil::getItem("time", "darkModeStartTimeMin", index, 0).toInt());
    ui->startTime->setTime(st);
    QTime et(FileUtil::getItem("time", "darkModeEndTimeHour", index, 8).toInt(),
             FileUtil::getItem("time", "darkModeEndTimeMin", index, 0).toInt());
    ui->endTime->setTime(et);
    bool enableIndex = FileUtil::getItem("time", "isEnableDarkModeIndex", index, false).toBool();
    ui->enableDarkMode->setChecked(enableIndex);
    bool forAll = FileUtil::getValue("time", "forAllDisplay", true).toBool();
    ui->displaySelector->setEnabled(!forAll);
    int darkValue = FileUtil::getItem("time", "darkModeValue", index, -20).toInt();
    ui->darkvalue->setText(QString::number(darkValue));
    ui->darkModeValue->setValue(darkValue);
    ui->forAllDisplay->setChecked(forAll);
    ui->darkModeValue->setMinimum(FileUtil::getValue("display", "min_brightness", -50).toInt());
    ui->darkModeValue->setMaximum(FileUtil::getValue("display", "max_brightness", 100).toInt());
}

TimerPage::~TimerPage() {
    delete ui;
}
