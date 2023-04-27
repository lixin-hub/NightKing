//
// Created by LIXIN on 2023/4/21.
//

#ifndef NIGHTKING_MYTHREAD_H
#define NIGHTKING_MYTHREAD_H

#include "QThread"

class MyThread : public QThread {
Q_OBJECT
public:
    MyThread(QWidget *parent);

    ~MyThread();

public slots:

    void StopThread();

protected:
    virtual void run() override;

private:
    bool is_stop_;
    int count_;
};


#endif //NIGHTKING_MYTHREAD_H
