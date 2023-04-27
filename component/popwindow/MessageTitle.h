#ifndef MESSAGETITLE_H
#define MESSAGETITLE_H

#include <QWidget>
#include <QVBoxLayout>

#include <QLabel>
#include "QRect"
#include <QPainter>

#include <QBitmap>

#include <QDesktopWidget>

#include <QApplication>

#include <QTimer>

#include <QDesktopServices>
class MessageTitle : public QWidget
{
//    Q_OBJECT
public:
    explicit MessageTitle(QWidget *parent = nullptr);

signals:
public:
   bool isEnter;
    int h,w;
   QLabel * content;
    QTimer *timerShow;
    QTimer *timerStay;
    QTimer *timerClose;
    void setMsg(QString title, QString content, QString work);
    virtual void focusOutEvent(QFocusEvent *e);
    void myResize();
    void showAsQQ();
    void myMove();
    void myStay();
    void myClose();
    void enterEvent(QEvent *);
    void leaveEvent(QEvent *);
    QPoint normalPoint;
};

#endif // MESSAGETITLE_H
