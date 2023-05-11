#ifndef SETTINGWINDOW_H
#define SETTINGWINDOW_H

#include <QWidget>
#include "component/BaseWindow.h"
namespace Ui {
class SettingWindow;
}

class SettingWindow : public QWidget {
Q_OBJECT

public:
    explicit SettingWindow(QWidget *parent = nullptr);

    ~SettingWindow() override;

    void closeEvent(QCloseEvent *event) override;

private:
    Ui::SettingWindow *ui;

    void initMenu();


    void initLayout(QWidget *widget, QWidget *target);
};

#endif // SETTINGWINDOW_H
