//
// Created by LIXIN on 2023/4/22.
//

#ifndef NIGHTKINGSETTING_BASEWINDOW_H
#define NIGHTKINGSETTING_BASEWINDOW_H


#include <QWidget>
#include "mytitlebar.h"

class BaseWindow : public QWidget
{
Q_OBJECT

public:
    BaseWindow(QWidget *parent = 0);
    ~BaseWindow();

private:
    void paintEvent(QPaintEvent *event);
    void loadStyleSheet(const QString &sheetName);

private slots:
    void onButtonMinClicked();
    void onButtonRestoreClicked();
    void onButtonMaxClicked();
    void onButtonCloseClicked();

protected:
    MyTitleBar* m_titleBar;

    void initTitleBar();
};



#endif //NIGHTKINGSETTING_BASEWINDOW_H
