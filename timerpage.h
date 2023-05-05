#ifndef TIMERPAGE_H
#define TIMERPAGE_H

#include <QWidget>

namespace Ui {
class TimerPage;
}

class TimerPage : public QWidget
{
    Q_OBJECT

public:
    explicit TimerPage(QWidget *parent = nullptr);
    ~TimerPage();

private:
    Ui::TimerPage *ui;

    void initState(int index);
};

#endif // TIMERPAGE_H
