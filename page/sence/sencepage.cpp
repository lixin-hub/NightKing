//
// Created by LIXIN on 2023/5/6.
//

// You may need to build the project (run Qt uic code generator) to get "ui_SencePage.h" resolved

#include "sencepage.h"
#include "ui_SencePage.h"


SencePage::SencePage(QWidget *parent) :
        QWidget(parent), ui(new Ui::SencePage) {
    ui->setupUi(this);
}

SencePage::~SencePage() {
    delete ui;
}
