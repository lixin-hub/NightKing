#include <QSettings>
#include <QDir>
#include "displaypage.h"
#include "ui_displaypage.h"
#include "QDebug"
#include "component/MonitorInfo/monitorinfo.h"
#include "common/monitor/MonitorController.h"
#include "QCheckBox"
#include "QVBoxLayout"
#include "QPushButton"
#include "component/DisplayOrder/displayorder.h"
#include "component/MarkDisplay/markdisplay.h"
#include "QScreen"
#include "common/Util/FileUitl/FileUtil.h"
#include "common/Util/JsonUtil/JsonUtil.h"
#include "QSlider"
#include "QComboBox"

DisplayPage::DisplayPage(QWidget *parent) : QWidget(parent), ui(new Ui::DisplayPage) {
    ui->setupUi(this);
    ui->scrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);//隐藏横向滚动条
    ui->scrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);//隐藏竖向滚动条
    initMonitorInfoList();
    initConnect();
}

DisplayPage::~DisplayPage() {
    delete ui;
}

void DisplayPage::initMonitorInfoList() {

    int i = 0;
    auto *isHiddenLayout = new QVBoxLayout(this);
    auto *showOrderLayout = new QVBoxLayout(this);
    isHiddenLayout->setMargin(0);
    isHiddenLayout->setSpacing(10);
    for (auto hm: MyMonitors::getHMonitors()) {
        i++;
        QString title_name = "显示器" + QString::number(i);
        MonitorInfoA monitorInfoA = MyMonitors::getMonitorsInfoA(hm);
        auto *info = new MonitorInfo(ui->display_info_list, title_name, "*", monitorInfoA);
        ui->monitor_info->addWidget(info);
        //隐藏显示器
        auto *isHidden = new QCheckBox(this);
        isHidden->setChecked(FileUtil::getItem("display", "isHidden", i - 1, false).toBool());
        connect(isHidden, &QCheckBox::stateChanged, this, [=](int state) {
            MonitorInfoA monitorInfo = monitorInfoA;
            int index = i - 1;
            FileUtil::setItem("display", "isHidden", index, isHidden->isChecked());
        });
        isHidden->setText(title_name);
        isHiddenLayout->addWidget(isHidden);
        //标记显示器按钮
        connect(ui->mark_display, &QPushButton::clicked, [=] {
            QMap<QString, QScreen *> scm = MyMonitors::getScreenNameMap();
            QString name = QString(monitorInfoA.displayDevice.DeviceName);
            name = name.left(name.lastIndexOf("\\"));
            QScreen *sc = scm.value(name);
            auto *m = new MarkDisplay(nullptr, title_name, sc);
            m->show();
        });
        //调整显示器顺序
        auto *order = new DisplayOrder(ui->hiden_display, title_name);
        showOrderLayout->addWidget(order);
    }
    ui->hiden_display->setLayout(isHiddenLayout);
    ui->dispaly_order->setLayout(showOrderLayout);

}

void DisplayPage::initConnect() {
    //亮度范围
    ui->min_bar->setValue(FileUtil::getValue("display", "min_brightness", -50).toInt());
    ui->max_bar->setValue(FileUtil::getValue("display", "max_brightness", 100).toInt());
    connect(ui->max_bar, &QSlider::valueChanged, [](int value) {
        FileUtil::setValue("display", "max_brightness", value);
    });
    connect(ui->min_bar, &QSlider::valueChanged, [](int value) {
        FileUtil::setValue("display", "min_brightness", value);
    });
    //显示器亮度更新速度
    ui->brightness_dealy->setCurrentIndex(FileUtil::getValue("display", "brightness_change_time_index", 3).toInt());
//    void (QComboBox::*current)(int index)= &QComboBox::currentIndexChanged;
    connect(ui->brightness_dealy, qOverload<int>(&QComboBox::currentIndexChanged), [](int index) {
        FileUtil::setValue("display", "brightness_change_time_index", index);
    });
}

