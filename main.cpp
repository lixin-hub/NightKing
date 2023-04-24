#include "page/component/popwindow/MessageTitle.h"
#include <QApplication>
#include "page/settingwindow/settingwindow.h"
int main(int argc, char *argv[]) {
    QApplication a(argc, argv);
    MessageTitle w;
    w.showAsQQ();
    return a.exec();
}
