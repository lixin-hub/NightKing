#ifndef MARKDISPLAY_H
#define MARKDISPLAY_H

#include <QWidget>
#include "QString"
namespace Ui {
class MarkDisplay;
}

class MarkDisplay : public QWidget
{
    Q_OBJECT

public:
    explicit MarkDisplay(QWidget *parent = nullptr,QString name="未识别",QScreen * qScreen= nullptr);
    ~MarkDisplay();

private:
    Ui::MarkDisplay *ui;
};

#endif // MARKDISPLAY_H
