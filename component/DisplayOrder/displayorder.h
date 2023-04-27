#ifndef DISPLAYORDER_H
#define DISPLAYORDER_H

#include <QWidget>
#include"QListWidgetItem"
namespace Ui {
class DisplayOrder;
}

class DisplayOrder : public QWidget
{
    Q_OBJECT

public:


    ~DisplayOrder();
    DisplayOrder(QWidget *parent, QString name);

private:
    Ui::DisplayOrder *ui;
};

#endif // DISPLAYORDER_H
