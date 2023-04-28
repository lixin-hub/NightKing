#ifndef TRANSPARENTWINDOW_H
#define TRANSPARENTWINDOW_H

#include <QWidget>
#include <QPainter>
#include <QtWidgets/QApplication>

class TransparentWindow : public QWidget {
Q_OBJECT



public:
    TransparentWindow(QWidget *parent, HMONITOR hm, int index);


    void setBrightness(int value);

protected:
    int brightness{};
    void paintEvent(QPaintEvent *event) override;
};

#endif // TRANSPARENTWINDOW_H