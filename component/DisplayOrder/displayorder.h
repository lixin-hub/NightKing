#ifndef DISPLAYORDER_H
#define DISPLAYORDER_H

#include <QWidget>
#include"QListWidgetItem"

namespace Ui {
    class DisplayOrder;
}

class DisplayOrder : public QWidget {
Q_OBJECT

public:


    ~DisplayOrder();


    DisplayOrder(QWidget *parent, QString name, int index);


    void mousePressEvent(QMouseEvent *event) override;

private:

    Ui::DisplayOrder *ui;
    QString name;
    int index;
};

#endif // DISPLAYORDER_H
