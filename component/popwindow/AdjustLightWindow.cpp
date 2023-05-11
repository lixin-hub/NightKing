#include "AdjustLightWindow.h"
#include "QDebug"
#include "adjustlightui.h"

#include <qaction.h>
#include <qmenu.h>
#include <qsystemtrayicon.h>
#include <QPushButton>
#include "QScreen"
#include "QCloseEvent"
#include "common/Util/TimeUtil/TimeUtil.h"

int closeTime = 10;//10hz

int startTime = 1;//1hz

int staySecond = 20;//20s

AdjustLightWindow::AdjustLightWindow(QWidget *parent) : QWidget{parent} {
    setWindowFlags(Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint | Qt::SubWindow | Qt::Popup);
    setAttribute(Qt::WA_TranslucentBackground);//设置窗口透明属性
    initContextMenu();
    initLayout();
    timerShow = new QTimer(this);
    connect(timerShow, &QTimer::timeout, this, &AdjustLightWindow::myMove);
    timerStay = new QTimer(this);
    connect(timerStay, &QTimer::timeout, this, &AdjustLightWindow::myStay);
    timerClose = new QTimer(this);
    connect(timerClose, &QTimer::timeout, this, &AdjustLightWindow::myClose);
    TimeUtil::startTimeCount();
}

void AdjustLightWindow::closeEvent(QCloseEvent *event) {
//    QMessageBox::information(this, "Tips", "The Program is miniSize,you can see it on the windows bottom");
    setVisible(false);
    event->ignore();
}

void AdjustLightWindow::showAsQQ() {
    setVisible(true);
    this->activateWindow();
    setWindowOpacity(1);
    QScreen *desktop = QGuiApplication::primaryScreen();
    QRect deskRect = desktop->availableGeometry();
    targetPoint.setX(deskRect.width() - rect().width() - 20);
    targetPoint.setY(deskRect.height() - rect().height() - 20);
    move(targetPoint.x(), desktop->size().height());
    timerShow->start(startTime);
    timerStay->start(1000);
    beginX = desktop->size().width();
    show();
}

//平滑显示出来

void AdjustLightWindow::myMove() {
    beginX -= 10;
    move(beginX, targetPoint.y());
    if (beginX <= targetPoint.x()) {
        timerShow->stop();
        this->raise();
    }
}

//停留显示
void AdjustLightWindow::myStay() {
    if (!this->isVisible()) {
        timerStay->stop();
        return;
    }
    static int timeCount = 0;
    timeCount++;
    if (timeCount >= staySecond) {
        timerStay->stop();
        timerClose->start(closeTime);
    }
}

//自动关闭时实现淡出效果
void AdjustLightWindow::myClose() {
    if (isEnter) {
        tran = 1.0;
        setWindowOpacity(tran);
        return;
    }
    tran -= 0.1;
    if (tran <= 0) {
        timerClose->stop();
        emit close();
    } else {
        setWindowOpacity(tran);
        targetPoint.setX(pos().x());
        targetPoint.setY(pos().y() + 10);
        move(targetPoint);
    }
}

void AdjustLightWindow::enterEvent(QEvent *) {
    isEnter = true;
}

void AdjustLightWindow::leaveEvent(QEvent *) {
    isEnter = false;
}


void AdjustLightWindow::myResize() {
    this->setMinimumSize(0, 0);//设定窗口最大最小大小
    this->setMaximumSize(2040, 2040);
    QRect tmp = this->geometry();
    QPoint centerPoint = tmp.center(); // 储存中心点坐标
    int adjustSize = 5;    //放大缩小的尺寸
    tmp.setWidth(tmp.width() - adjustSize);
    tmp.setHeight(tmp.height() - adjustSize);
    if (tmp.width() > 5) // 限制最小尺寸
    {
        tmp.moveTo(centerPoint); // 从中心缩放而非左上角处
        this->setGeometry(tmp);//改变窗口大小
    }
}


bool AdjustLightWindow::event(QEvent *event) {
    if (QEvent::WindowDeactivate == event->type()) {
        QRect rect = this->geometry();
        QPoint point = QCursor::pos();
        if ((point.x() < rect.left() || point.x() > rect.right()) ||
            (point.y() < rect.top() || point.y() > rect.bottom())) {
            qDebug() << "失去焦点";
            timerClose->start(closeTime);
            isEnter = false;
            tran = 1.0;
        }
    }
    return QWidget::event(event);

}

void AdjustLightWindow::initContextMenu() {
    auto *icon = new QSystemTrayIcon(this);
    icon->setIcon(QIcon(":/img/shezhi.svg"));
    icon->setContextMenu(new QMenu(this));
    // 创建一个 "Quit" 菜单项
    auto *quitAction = new QAction("Quit", icon->contextMenu());
    quitAction->setIcon(QIcon(":/images/exit.png"));
    quitAction->setToolTip("Quit the App");
    // 将 "Quit" 菜单项添加到菜单中
    icon->contextMenu()->addAction(quitAction);
    connect(quitAction, &QAction::triggered, this, &QWidget::deleteLater);
    connect(icon, &QSystemTrayIcon::activated, this, [=](QSystemTrayIcon::ActivationReason reason) mutable {
        if (reason == QSystemTrayIcon::Trigger) {
            qDebug() << "点击托盘图标";
            initLayout();
            this->showAsQQ();
        }
    });
    // 将菜单显示在托盘中
    icon->show();

}

void AdjustLightWindow::initLayout() {
    if (layout != nullptr) {
        layout->deleteLater();
        layout = nullptr;
    }
    auto *mainLayout = new QVBoxLayout();
    mainLayout->setMargin(0);
    if (layout == nullptr) {
        layout = new AdjustLightUi(this);
        QPushButton *close = layout->getCloeButton();
        connect(close, &QPushButton::clicked, [this]() {
            timerClose->start(closeTime);
            isEnter = false;
            tran = 1.0;
        });
    }
    qDebug() << layout->sizeHint();
    layout->setFixedSize(layout->sizeHint());
    mainLayout->addWidget(layout);
    this->setLayout(mainLayout);
    this->setStyleSheet("background-color:red");
}
