//
// Created by LIXIN on 2023/4/21.
//

#include "MyThread.h"
#include <QDebug>
#include<windows.h>
#include "WMI.h"



void MyThread::StopThread() {
    is_stop_ = true;
}

void MyThread::run() {
        qInfo() << "Thread-ID: " << QThread::currentThreadId() << " Count: " << ++count_;
        WMI::setBrightness(100);
    qInfo() << "Thread-ID: " << QThread::currentThreadId() << " Exit";
}

MyThread::MyThread(QWidget* parent) {

}

MyThread::~MyThread() {

}
