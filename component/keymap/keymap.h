//
// Created by LIXIN on 2023/5/7.
//

#ifndef NIGHTKING_KEYMAP_H
#define NIGHTKING_KEYMAP_H

#include <QWidget>


QT_BEGIN_NAMESPACE
namespace Ui { class KeyMap; }
QT_END_NAMESPACE

class KeyMap : public QWidget {
Q_OBJECT

public:
    explicit KeyMap(QWidget *parent,QString name,int index);

    ~KeyMap() override;
private:
    int index;
    QString name;
    QString key;
    Ui::KeyMap *ui;
};


#endif //NIGHTKING_KEYMAP_H
