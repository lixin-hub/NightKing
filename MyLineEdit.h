//
// Created by LIXIN on 2023/5/7.
//

#ifndef NIGHTKING_MYLINEEDIT_H
#define NIGHTKING_MYLINEEDIT_H


#include <QLineEdit>

class MyLineEdit : public QLineEdit{
    Q_OBJECT
public:

    explicit MyLineEdit(QWidget *parent);

    void keyPressEvent(QKeyEvent *event) override;
    void keyReleaseEvent(QKeyEvent *event) override;
    QString key;
};


#endif //NIGHTKING_MYLINEEDIT_H
