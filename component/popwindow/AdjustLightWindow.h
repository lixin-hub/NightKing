#ifndef MESSAGETITLE_H
#define MESSAGETITLE_H

#include <QWidget>
#include <QVBoxLayout>

#include <QLabel>
#include "QRect"
#include "adjustlightui.h"
#include <QPainter>

#include <QBitmap>


#include <QApplication>

#include <QTimer>

#include <QDesktopServices>

class AdjustLightWindow : public QWidget {
//    Q_OBJECT
public:
    explicit AdjustLightWindow(QWidget *parent = nullptr);

signals:
public:
    bool isEnter{};
    int h{}, w{};
    QLabel *content{};
    QTimer *timerShow;
    QTimer *timerStay;
    QTimer *timerClose;
    double tran = 1.0;

    void setMsg(QString title, QString content, QString work);

    bool event(QEvent *event) override;

    void myResize();

    void showAsQQ();

    void myMove();

    void myStay();

    void myClose();

    void enterEvent(QEvent *);

    void leaveEvent(QEvent *);

    QPoint targetPoint;

    void closeEvent(QCloseEvent *event);

    int beginX{};
    AdjustLightUi* layout= nullptr;
    void initContextMenu();

    void initLayout();
};

#endif // MESSAGETITLE_H
