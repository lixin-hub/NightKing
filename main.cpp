#include "component/popwindow/MessageTitle.h"
#include "settingwindow/settingwindow.h"
#include <QApplication>

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);
//    MessageTitle w;
//    w.showAsQQ();
    SettingWindow w;
    w.show();
    return a.exec();
}
