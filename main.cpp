#include "component/popwindow/AdjustLightWindow.h"
#include "QApplication"
#include "QDebug"
int main(int argc, char *argv[]) {
    QApplication a(argc, argv);
    AdjustLightWindow w;
    w.setFixedSize(w.sizeHint());
    w.showAsQQ();
    return a.exec();
}
