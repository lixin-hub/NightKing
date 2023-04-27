#include "displayorder.h"
#include "ui_displayorder.h"
#include "QDebug"
DisplayOrder::DisplayOrder(QWidget *parent, QString name) : QWidget(parent), ui(new Ui::DisplayOrder) {
    ui->setupUi(this);
    ui->display_name->setText(name);

    connect(ui->up, &QToolButton::clicked, [=]() mutable {
    });
}

DisplayOrder::~DisplayOrder() {
    delete ui;
}


