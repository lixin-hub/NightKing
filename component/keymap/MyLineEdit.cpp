//
// Created by LIXIN on 2023/5/7.
//

#include <QKeyEvent>
#include <QLineEdit>
#include "MyLineEdit.h"
#include "QDebug"

MyLineEdit::MyLineEdit(QWidget *parent) : QLineEdit(parent) {}

void MyLineEdit::keyPressEvent(QKeyEvent *event) {

    key.clear();
    if (event->modifiers() & Qt::ControlModifier) {
        key.append("Ctlr+");
    }
    if (event->modifiers() & Qt::ShiftModifier) {
        key.append("Shift+");
        // Shift is pressed
    }
    if (event->modifiers() & Qt::AltModifier) {
        key.append("Alt+");
    }

    key.append(event->key());
    setText(key);

}

void MyLineEdit::keyReleaseEvent(QKeyEvent *event) {
    if (event->modifiers() == Qt::NoModifier)
//        setText(key);
        QWidget::keyReleaseEvent(event);
}

