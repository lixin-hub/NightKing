//
// Created by LIXIN on 2023/5/7.
//

#ifndef NIGHTKING_KEYMAPPAGE_H
#define NIGHTKING_KEYMAPPAGE_H

#include <QWidget>


QT_BEGIN_NAMESPACE
namespace Ui { class KeyMapPage; }
QT_END_NAMESPACE

class KeyMapPage : public QWidget {
Q_OBJECT

public:
    explicit KeyMapPage(QWidget *parent = nullptr);

    ~KeyMapPage() override;

private:
    Ui::KeyMapPage *ui;
};


#endif //NIGHTKING_KEYMAPPAGE_H
