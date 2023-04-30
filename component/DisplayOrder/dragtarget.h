#ifndef DRAGTARGET_H
#define DRAGTARGET_H

#include <QWidget>

namespace Ui {
    class DragTarget;
}

class DragTarget : public QWidget {
Q_OBJECT

public:

    void clear();

    explicit DragTarget(QWidget *parent = nullptr);

    void dragEnterEvent(QDragEnterEvent *event) override;

    void dropEvent(QDropEvent *event) override;

    ~DragTarget() override;

    int nums;
    QList<int> indexList;

    void deleteItem(QLayout *layout);

private:
    Ui::DragTarget *ui;
};

#endif // DRAGTARGET_H
