#ifndef DISPLAYPAGE_H
#define DISPLAYPAGE_H

#include <QWidget>

namespace Ui {
    class DisplayPage;
}

class DisplayPage : public QWidget {
Q_OBJECT

public:
    explicit DisplayPage(QWidget *parent = nullptr);

    ~DisplayPage();

private:
    Ui::DisplayPage *ui;

    void initConnect();

    void initMonitorInfoList();
};

#endif // DISPLAYPAGE_H
