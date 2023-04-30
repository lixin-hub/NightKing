#include "timerpage.h"
#include "ui_timerpage.h"

TimerPage::TimerPage(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::TimerPage)
{
    ui->setupUi(this);
}

TimerPage::~TimerPage()
{
    delete ui;
}
