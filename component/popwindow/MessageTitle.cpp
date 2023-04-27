#include "MessageTitle.h"
#include "QDebug"
#include "QPushButton"
#include "adjustlightui.h"
#include "component/myprogressbar/myprogressbar.h"

MessageTitle::MessageTitle(QWidget *parent)
        : QWidget{parent} {
    setWindowFlags(Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint);
    isEnter = false;
     w= size().width();
     h= size().height();
    QVBoxLayout *mainLayout = new QVBoxLayout();
    mainLayout->setMargin(0);
    AdjustLightUi *layout=new AdjustLightUi(this);
    mainLayout->addWidget(layout, 0, Qt::AlignRight);
    setAttribute(Qt::WA_TranslucentBackground);//设置窗口透明属性
    setLayout(mainLayout);
    this->activateWindow();
    this->setFocus();
    setFixedSize(sizeHint().width(), sizeHint().height());
    timerShow = new QTimer(this);
    connect(timerShow,&QTimer::timeout, this, &MessageTitle::myMove);
    timerStay = new QTimer(this);
    connect(timerStay,&QTimer::timeout, this, &MessageTitle::myStay);
    timerClose = new QTimer(this);
    connect(timerClose, &QTimer::timeout, this, &MessageTitle::myClose);
}


void MessageTitle::showAsQQ() {
    QDesktopWidget *deskTop = QApplication::desktop();
    QRect deskRect = deskTop->availableGeometry();
    normalPoint.setX(deskRect.width() - rect().width() - 20);
    normalPoint.setY(deskRect.height() - rect().height()-20);
    move(normalPoint.x(),normalPoint.y());
    timerShow->start(2);
    show();
}

//平滑显示出来

void MessageTitle::myMove() {
    static int beginY = QApplication::desktop()->height();
    beginY--;
    move(normalPoint.x(), beginY);
    if (beginY <= normalPoint.y()) {
        timerShow->stop();
        timerStay->start(1000);
    }
}

//停留显示
void MessageTitle::myStay() {
    static int timeCount = 0;
    timeCount++;
    if (timeCount >= 50) {
        timerStay->stop();
        timerClose->start(20);
    }
}

//自动关闭时实现淡出效果
void MessageTitle::myClose() {
    static double tran = 1.0;
    if (isEnter) {
        tran = 1.0;
        setWindowOpacity(tran);
        return;
    }
    tran -= 0.1;
    if (tran <= 0) {
        timerClose->stop();
        emit close();
    } else{
        setWindowOpacity(tran);
        move(pos().x(),pos().y()+10);
    }
}

void MessageTitle::enterEvent(QEvent *) {
    isEnter = true;
}

void MessageTitle::leaveEvent(QEvent *) {
    isEnter = false;
}

void MessageTitle::focusOutEvent(QFocusEvent *e) {
//    timerClose->start(20);
//    qDebug()<<"失去焦点";this->myClose();
}



void MessageTitle::myResize(){
    this->setMinimumSize(0, 0);//设定窗口最大最小大小
    this->setMaximumSize(2040, 2040);
    QRect tmp = this->geometry();
    QPoint centerPoint = tmp.center(); // 储存中心点坐标
    int adjustSize = 5;	//放大缩小的尺寸
        tmp.setWidth(tmp.width() - adjustSize);
        tmp.setHeight(tmp.height() - adjustSize);
    if (tmp.width() > 5) // 限制最小尺寸
    {
        tmp.moveTo(centerPoint); // 从中心缩放而非左上角处
        this->setGeometry(tmp);//改变窗口大小
    }
}