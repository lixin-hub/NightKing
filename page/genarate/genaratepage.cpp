#include "genaratepage.h"
#include "ui_genaratepage.h"
#include "common/Util/FileUitl/FileUtil.h"
#include "QLineEdit"
GenaratePage::GenaratePage(QWidget *parent) : QWidget(parent), ui(new Ui::GenaratePage) {
    ui->setupUi(this);
    ui->language->setEditable(true);
    ui->theme->setEditable(true);
    ui->language->lineEdit()->setAlignment(Qt::AlignCenter);
    ui->theme->lineEdit()->setAlignment(Qt::AlignCenter);
    ui->language->setCurrentIndex(FileUtil::getValue("generate","language",0).toInt());
    ui->theme->setCurrentIndex(FileUtil::getValue("generate","theme",0).toInt());
    connect(ui->language,qOverload<int>(&QComboBox::currentIndexChanged),[](int index){
        FileUtil::setValue("generate","language",index);
    }); connect(ui->theme,qOverload<int>(&QComboBox::currentIndexChanged),[](int index){
        FileUtil::setValue("generate","theme",index);
    });
}

GenaratePage::~GenaratePage() {
    delete ui;
}
