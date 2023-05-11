#include "monitorinfo.h"
#include "ui_monitorinfo.h"


MonitorInfo::~MonitorInfo() {
    delete ui;
}


MonitorInfo::MonitorInfo(QWidget *parent, QString name, const QString adjustMethod, MonitorInfoA monitorInfoA)
        : QWidget(parent), ui(new Ui::MonitorInfo) {
    ui->setupUi(this);
    ui->title_name->setText(name);
    ui->display_name->setText(QString((char*)monitorInfoA.displayDevice.DeviceName));
    ui->method->setText(adjustMethod);
    ui->max_brightness->setText(QString::number(monitorInfoA.monitorBrightness.maximumBrightness));
    ui->current_brightness->setText(QString::number(monitorInfoA.monitorBrightness.currentBrightness));
    ui->enable_mask->setText("false");
}

