//
// Created by LIXIN on 2023/5/7.
//

#ifndef NIGHTKING_MYWINEVENTFILTER_H
#define NIGHTKING_MYWINEVENTFILTER_H


#pragma once
#include <QAbstractNativeEventFilter>

class MyGlobalShortCut;

class MyWinEventFilter : public QAbstractNativeEventFilter
{
public:
    MyWinEventFilter(MyGlobalShortCut *shortcut);
    ~MyWinEventFilter();
    virtual bool nativeEventFilter(const QByteArray &eventType, void *message, long *);
private:
    MyGlobalShortCut *m_shortcut;
};



#endif //NIGHTKING_MYWINEVENTFILTER_H
