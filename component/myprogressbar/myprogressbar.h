#ifndef MYPROGRESSBAR_H
#define MYPROGRESSBAR_H

#include <QProgressBar>
#include <QWidget>
#include <QSlider>
namespace Ui {
class MyProgressBar;
}

class MyProgressBar : public QWidget
{
public:
    explicit MyProgressBar(QWidget *parent = nullptr);
    ~MyProgressBar() override;
    void setName(QString name);
    void setValue(int value);
    QSlider *getProgressbar();

private:
    Ui::MyProgressBar *ui;
};

#endif // MYPROGRESSBAR_H
