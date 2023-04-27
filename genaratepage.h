#ifndef GENARATEPAGE_H
#define GENARATEPAGE_H

#include <QWidget>

namespace Ui {
class GenaratePage;
}

class GenaratePage : public QWidget
{
    Q_OBJECT

public:
    explicit GenaratePage(QWidget *parent = nullptr);
    ~GenaratePage();

private:
    Ui::GenaratePage *ui;
};

#endif // GENARATEPAGE_H
