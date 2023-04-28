#include "settingwindow.h"
#include "ui_settingwindow.h"
#include "QIcon"
#include "QMargins"
#include <QAction>
#include <QMenu>
#include <QSystemTrayIcon>
#include<QDebug>
#include <QPushButton>
#include <qsystemtrayicon.h>

SettingWindow::SettingWindow(QWidget *parent) : QWidget(parent), ui(new Ui::SettingWindow) {
    ui->setupUi(this);
    initMenu();
}
//初始化菜单和图标
void SettingWindow::initMenu() {
    ui->bt_generate->setIcon(QIcon(":/img/shezhi.svg"));
    ui->bt_display_setting->setIcon(QIcon(":/img/xianshiqi.svg"));
    ui->bt_timer->setIcon(QIcon(":/img/timer.svg"));
    ui->bt_key_map->setIcon(QIcon(":/img/kuaijiejian.svg"));
    ui->bt_update->setIcon(QIcon(":/img/gengxin.svg"));
    setWindowTitle("设置");
    connect(ui->bt_generate, &QToolButton::clicked, [=] {
        this->ui->myTab->setCurrentIndex(0);
    });
    connect(ui->bt_display_setting, &QToolButton::clicked, [=] { this->ui->myTab->setCurrentIndex(1); });
    connect(ui->bt_key_map, &QToolButton::clicked, [=] { this->ui->myTab->setCurrentIndex(2); });
    connect(ui->bt_timer, &QToolButton::clicked, [=] { this->ui->myTab->setCurrentIndex(3); });
    connect(ui->bt_update, &QToolButton::clicked, [=] { this->ui->myTab->setCurrentIndex(4); });
}

SettingWindow::~SettingWindow() {
    delete ui;
}
//监听窗口变化
void SettingWindow::resizeEvent(QResizeEvent *event) {
    resizeBlock();
}

/**
 * 调整tab标签内容以适应窗口变化
 */
void SettingWindow::resizeBlock() {
    ui->genarate_block->resize(ui->tab_genarate->size());
    ui->genarate_block->move(ui->tab_genarate->pos());
    ui->display_block->resize(ui->tab_display->size());
    ui->display_block->move(ui->tab_display->pos());
}
