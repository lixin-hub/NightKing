#ifndef TIMEPOINT_H
#define TIMEPOINT_H

#include <QWidget>

namespace Ui {
class TimePoint;
}

class TimePoint : public QWidget
{
    Q_OBJECT

public:
    explicit TimePoint(QWidget *parent = nullptr);
    ~TimePoint();

private:
    Ui::TimePoint *ui;
};

#endif // TIMEPOINT_H
