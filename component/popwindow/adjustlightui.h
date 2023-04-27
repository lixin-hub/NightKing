#ifndef ADJUSTLIGHTUI_H
#define ADJUSTLIGHTUI_H

#include <QWidget>

namespace Ui {
    class AdjustLightUi;
}

class AdjustLightUi : public QWidget {
Q_OBJECT

public:
    explicit AdjustLightUi(QWidget *parent = nullptr);

    ~AdjustLightUi();

    int SetGamma(int bright);

private:
    Ui::AdjustLightUi *ui;
};

#endif // ADJUSTLIGHTUI_H
