#ifndef ADJUSTLIGHTUI_H
#define ADJUSTLIGHTUI_H

#include <QWidget>
#include <QPushButton>

namespace Ui {
    class AdjustLightUi;
}

class AdjustLightUi : public QWidget {
Q_OBJECT

    void init();

public:

    QPushButton *getCloeButton();

    explicit AdjustLightUi(QWidget *parent = nullptr);

    ~AdjustLightUi() override;

    int SetGamma(int bright);

private:
    Ui::AdjustLightUi *ui;
};

#endif // ADJUSTLIGHTUI_H
