//
// Created by LIXIN on 2023/5/6.
//

#ifndef NIGHTKING_SENCEPAGE_H
#define NIGHTKING_SENCEPAGE_H

#include <QWidget>


QT_BEGIN_NAMESPACE
namespace Ui { class SencePage; }
QT_END_NAMESPACE

class SencePage : public QWidget {
Q_OBJECT

public:
    explicit SencePage(QWidget *parent = nullptr);

    ~SencePage() override;

private:
    Ui::SencePage *ui;
};


#endif //NIGHTKING_SENCEPAGE_H
