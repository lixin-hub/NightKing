#include <QCheckBox>
#include "dragtarget.h"
#include "ui_dragtarget.h"
#include "QDragEnterEvent"
#include "QVBoxLayout"
#include "displayorder.h"
#include "QMimeData"
#include "common/monitor/mymonitors.h"
#include "QDebug"
#include "common/Util/FileUitl/FileUtil.h"

DragTarget::DragTarget(QWidget *parent) : QWidget(parent), ui(new Ui::DragTarget) {
    ui->setupUi(this);
    nums = 0;
}

void DragTarget::dragEnterEvent(QDragEnterEvent *event) {
    if (event->source() == this)
        event->ignore();
    else {
        event->accept();
    }

}


DragTarget::~DragTarget() {
    delete ui;
}

void DragTarget::dropEvent(QDropEvent *event) {

    if (event->source() == this) {
        event->ignore();
        return;
    }
    int len = MyMonitors::getHMonitors().length();
    const QMimeData *data = event->mimeData();
    int index = data->property("index").toInt();
    auto *t = dynamic_cast<QVBoxLayout *>(this->layout());
    if (len > t->children().length()) {
        qDebug() << data->text();
        if (!indexList.contains(index)) {
            indexList.push_back(index);
            t->addWidget(new DisplayOrder(this, data->property("name").toString(), index));
            nums++;
        }
        if (len == nums) {
            for (int i = 0; i < indexList.size(); i++) {
                qDebug() << "drag" << indexList.at(i);
                FileUtil::setItem("display", "displayOrder", i, indexList.at(i));
            }
        }
    }
}

void DragTarget::clear() {
    deleteItem(this->layout());
    nums = 0;
    indexList.clear();
}

void DragTarget::deleteItem(QLayout *layout) {
    QLayoutItem *child;
    while ((child = layout->takeAt(0)) != nullptr) {
        //setParent为NULL，防止删除之后界面不消失
        if (child->widget()) {
            child->widget()->setParent(nullptr);
            delete child->widget();
        } else if (child->layout()) {
            deleteItem(child->layout());
            child->layout()->deleteLater();
        }
        delete child;
    }
}
