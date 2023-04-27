#include "markdisplay.h"
#include "ui_markdisplay.h"
#include "QDesktopWidget"
#include "QApplication"
#include "QScreen"
#include "QGuiApplication"
#include "QDebug"
#include "QPainter"
#include "QPen"
#include "QTimer"

MarkDisplay::MarkDisplay(QWidget *parent, QString name, QScreen *qScreen) : QWidget(parent), ui(new Ui::MarkDisplay) {
    ui->setupUi(this);
    auto *timerShow = new QTimer(this);
    int i=6;
    connect(timerShow, &QTimer::timeout, [=]() mutable{
        if(--i==0)
        close();
        else{
            QString string=name;
            ui->text->setText(string.append("\n").append(QString::number(i)));
        }
    });
    timerShow->start(1000);
    setWindowFlags(Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint|Qt::SubWindow);
    setAttribute(Qt::WA_TranslucentBackground);//设置窗口透明属性
    ui->text->setStyleSheet("font-size:200px;\n"
                            "background-color:rgba(225,225,225,0.5);\n");
    if (!qScreen) {
        close();
    }
    ui->text->setText(name+"\n");
    setGeometry(qScreen->geometry());
    resize(qScreen->size() / 2);
    move(qScreen->geometry().x() + this->size().width() / 2, qScreen->geometry().y() + this->size().height() / 2);

}

MarkDisplay::~MarkDisplay() {
    delete ui;
}


