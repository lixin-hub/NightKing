#ifndef SETTINGWINDOW_H
#define SETTINGWINDOW_H

#include <QWidget>
#include "component/BaseWindow.h"
namespace Ui {
class SettingWindow;
}

class SettingWindow : public QWidget
{
    Q_OBJECT

public:
    explicit SettingWindow(QWidget *parent = nullptr);
    ~SettingWindow();
    void resizeEvent(QResizeEvent *event);
    void closeEvent(QCloseEvent *event) override;
        void resizeBlock();
private:
    Ui::SettingWindow *ui;

    void initMenu();
};

#endif // SETTINGWINDOW_H
