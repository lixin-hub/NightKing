// transparentwindow.cpp
#include "common/Util//FileUitl/FileUtil.h"
#include <QBitmap>
#include <iostream>
#include "transparentwindow.h"
#include "windows.h"
#include "QMouseEvent"
#include "QDebug"
#include "QColor"
#include "QTimer"
#include "common/monitor/mymonitors.h"
#include "common/monitor/MonitorController.h"


TransparentWindow::TransparentWindow(QWidget *parent, HMONITOR hm,int index) : QWidget(parent) {
    setStyleSheet("border:none;margin:0");
    MonitorInfoA monitorInfoA = MyMonitors::getMonitorsInfoA(hm);
    QMap<QString, QScreen *> scm = MyMonitors::getScreenNameMap();
    QString name = QString(monitorInfoA.displayDevice.DeviceName);
    name = name.left(name.lastIndexOf("\\"));
    QScreen *sc = scm.value(name);
    setGeometry(sc->availableGeometry());
    this->setAttribute(Qt::WA_TranslucentBackground);//设置窗口透明属性
    setAttribute(Qt::WA_TransparentForMouseEvents, true);
    setWindowFlags(Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint |Qt::SubWindow);
    showFullScreen();
    auto *timer = new QTimer(this);
    connect(timer, &QTimer::timeout, [this]() {
        qDebug() << "更新";
        this->raise();
    });
//    timer->start(1000);
}

void TransparentWindow::paintEvent(QPaintEvent *event) {
    Q_UNUSED(event)
    QPainter painter(this);
    double b = brightness == 0 ? 0 : qAbs(brightness) / 100.0;
    qDebug() << "亮度" << b;
    painter.setOpacity(b);;
    painter.fillRect(rect(), QColor(qRgba(26, 32, 38, 0)));
}

void TransparentWindow::setBrightness(int value) {
    this->brightness = value;
    this->repaint();

}




//
//void TransparentWindow::mousePressEvent(QMouseEvent *event) {
//    if (event->button() == Qt::LeftButton)
//        qDebug() << "lll";
//    else if (event->button() == Qt::RightButton)
//        qDebug() << "rrr";
//// 将点击事件传递给下面的窗口
//    QCoreApplication::sendEvent(QApplication::widgetAt(event->globalPos()), event);//    if (this->parentWidget())
////    {
////        //点下有去的那一刻屏蔽当前窗口及其所子部件的事件，此时被当前窗口遮挡住的父窗口的控件的事件被响应
////        //为什么不到这里就结束，因为如果这样做，只要点击过一次，当前窗口及其子部件就再也不会响应鼠标事件，比如移入移出
////        //因此点击之后还必须要把这个屏蔽取消掉
////        //但如果中间什么也不做的话，这个属性的设置就没有意义，因为最终是false
////        //所以需要手动将鼠标点击事件下发给父窗口中的对应部件
////        this->setAttribute(Qt::WA_TransparentForMouseEvents, true);
////        QPoint point = this->mapTo(this->parentWidget(), event->pos());    //将点击事件在当前部件的坐标转换为在父窗口坐标系中的坐标
////        QWidget *widget = this->parentWidget()->childAt(point);   //判断当前点击的位置是否是父窗口的一个子部件
////        if (widget)
////        {
////            point = widget->mapFrom(this->parentWidget(), point);    //将当前部件在父窗口的坐标转为在当前部件自己的坐标系中的坐标
////            QMouseEvent *mouseEvent = new QMouseEvent(event->type(), point, event->button(), event->buttons(), event->modifiers());
////            QApplication::postEvent(widget, mouseEvent);
////        }
////        this->setAttribute(Qt::WA_TransparentForMouseEvents, false);
////    }

//}