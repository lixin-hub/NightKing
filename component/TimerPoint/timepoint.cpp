#include "timepoint.h"
#include "ui_timepoint.h"
#include "QTime"
TimePoint::TimePoint(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::TimePoint)
{
    ui->setupUi(this);
}

TimePoint::~TimePoint()
{
    delete ui;
}
