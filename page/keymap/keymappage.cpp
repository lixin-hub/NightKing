//
// Created by LIXIN on 2023/5/7.
//

// You may need to build the project (run Qt uic code generator) to get "ui_KeyMapPage.h" resolved

#include <QShortcut>
#include <QMessageBox>
#include "keymappage.h"
#include "ui_KeyMapPage.h"
#include "component/keymap/keymap.h"
#include "common/monitor/mymonitors.h"
#include "QDebug"
#include "common/Util/KeyMapUtil/MyGlobalShortCut.h"

KeyMapPage::KeyMapPage(QWidget *parent) : QWidget(parent), ui(new Ui::KeyMapPage) {
    ui->setupUi(this);
    auto allDisplay = new KeyMap(this, "所有显示器", -1);
    ui->keymapLayout->addWidget(allDisplay);
    QList<HMONITOR> monitors = MyMonitors::getHMonitors();
    for (int i = 0; i < monitors.size(); ++i) {
        auto display = new KeyMap(this, QString("显示器").append(QString::number(i)), i);
        ui->keymapLayout->addWidget(display);
    }

}

KeyMapPage::~KeyMapPage() {
    delete ui;
}


