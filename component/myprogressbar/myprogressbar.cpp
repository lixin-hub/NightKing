#include "myprogressbar.h"
#include "ui_myprogressbar.h"

MyProgressBar::MyProgressBar(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MyProgressBar)
{
    ui->setupUi(this);
}

MyProgressBar::~MyProgressBar()
{
    delete ui;
}

void MyProgressBar::setName(QString name)
{
    this->ui->monitor->setText(name);
}
void MyProgressBar::setValue(int value)
{
this->ui->progressBar->setValue(value);
}

QSlider *MyProgressBar::getProgressbar() {
    return ui->progressBar;
}
QSlider *MyProgressBar::getGamaProgressBar() {
//    return ui->gamaProgress;
}


