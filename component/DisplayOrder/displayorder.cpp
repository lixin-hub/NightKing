#include <QAbstractButton>
#include "displayorder.h"
#include "ui_displayorder.h"
#include "QDebug"
#include "QMouseEvent"
#include "QDrag"
#include "QMimeData"

DisplayOrder::DisplayOrder(QWidget *parent, const QString& name,int index) : QWidget(parent), ui(new Ui::DisplayOrder) {
    ui->setupUi(this);
    this->name = name;
    this->index=index;
    ui->display_name->setText(name);
}

DisplayOrder::~DisplayOrder() {
    delete ui;
}

void DisplayOrder::mousePressEvent(QMouseEvent *event) {
    if (event->button() == Qt::LeftButton) {
        auto *drag = new QDrag(this);
        auto *data = new QMimeData;
        data->setProperty("index",this->index);
        data->setProperty("name",this->name);
        drag->setMimeData(data);
        drag->exec(Qt::MoveAction);
    }
}