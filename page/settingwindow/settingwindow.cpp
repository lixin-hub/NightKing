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
#include "page/component/popwindow/MessageTitle.h"

SettingWindow::SettingWindow(QWidget *parent) : QWidget(parent), ui(new Ui::SettingWindow) {
    ui->setupUi(this);
    ui->bt_generate->setIcon(QIcon(":/img/shezhi.svg"));
    ui->bt_display_setting->setIcon(QIcon(":/img/xianshiqi.svg"));
    ui->bt_timer->setIcon(QIcon(":/img/timer.svg"));
    ui->bt_key_map->setIcon(QIcon(":/img/kuaijiejian.svg"));
    ui->bt_update->setIcon(QIcon(":/img/shezhi.svg"));
    QSystemTrayIcon *icon = new QSystemTrayIcon(this);
    icon->setIcon(QIcon(":/img/shezhi.svg"));
    icon->setContextMenu(new QMenu(this));

    // 创建一个 "Quit" 菜单项
    QAction *quitAction = new QAction("Quit", icon->contextMenu());
    quitAction->setIcon(QIcon(":/images/exit.png"));
    quitAction->setToolTip("Quit the App");
    // 将 "Quit" 菜单项添加到菜单中
    icon->contextMenu()->addAction(quitAction);
    MessageTitle *m ;
    connect(icon, &QSystemTrayIcon::activated, this, [=](QSystemTrayIcon::ActivationReason resaon) mutable {
        if (resaon == QSystemTrayIcon::Trigger) {
            qDebug()<<"点击托盘图标";
            if(!m)
               m=new MessageTitle(this);
            m->showAsQQ();
        }
    });
    // 将菜单显示在托盘中
    icon->show();
}

SettingWindow::~SettingWindow() {
    delete ui;
}
