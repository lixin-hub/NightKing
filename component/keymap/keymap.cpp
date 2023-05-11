//
// Created by LIXIN on 2023/5/7.
//

// You may need to build the project (run Qt uic code generator) to get "ui_KeyMap.h" resolved

#include "keymap.h"
#include "ui_KeyMap.h"
#include "common/Util/KeyMapUtil/MyGlobalShortCut.h"
#include "QDebug"
#include "QKeyEvent"
#include "common/monitor/MonitorController.h"

KeyMap::~KeyMap() {
    delete ui;
}

KeyMap::KeyMap(QWidget *parent, QString name, int index) : QWidget(parent), ui(new Ui::KeyMap) {
    ui->setupUi(this);
    this->name = name;
    this->index = index;
    ui->displayName->setText(name);
    auto shortcut = new MyGlobalShortCut("Ctrl+W", this);
    QObject::connect(shortcut, &MyGlobalShortCut::activated, [=]() {
        MonitorController::addBrightness(MyMonitors::getHMonitors(), index, 5);
    });  auto subshortcut = new MyGlobalShortCut("Ctrl+Q", this);
    QObject::connect(subshortcut, &MyGlobalShortCut::activated, [=]() {
        MonitorController::subBrightness(MyMonitors::getHMonitors(), index, 5);
    });

    connect(ui->addKey, &QLineEdit::textChanged, [=](QString keySequence) {
        qDebug() << keySequence;
        auto shortcut = new MyGlobalShortCut(keySequence, this);
        QObject::connect(shortcut, &MyGlobalShortCut::activated, [=]() {
            qDebug() << "增加亮点";
        });
    });
}

