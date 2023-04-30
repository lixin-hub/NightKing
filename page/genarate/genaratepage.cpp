#include "genaratepage.h"
#include "ui_genaratepage.h"
#include "common/Util/FileUitl/FileUtil.h"
#include "QLineEdit"
#include "common/Util/AutoBoot/AutoBoot.h"

GenaratePage::GenaratePage(QWidget *parent) : QWidget(parent), ui(new Ui::GenaratePage) {
    ui->setupUi(this);
    ui->language->setEditable(true);
    ui->theme->setEditable(true);
    ui->language->lineEdit()->setAlignment(Qt::AlignCenter);
    ui->theme->lineEdit()->setAlignment(Qt::AlignCenter);
    ui->language->setCurrentIndex(FileUtil::getValue("generate", "language", 0).toInt());
    ui->theme->setCurrentIndex(FileUtil::getValue("generate", "theme", 0).toInt());
    ui->autoBoot->setChecked(FileUtil::getValue("generate", "autoBoot", false).toBool());
    connect(ui->language, qOverload<int>(&QComboBox::currentIndexChanged), [](int index) {
        FileUtil::setValue("generate", "language", index);
    });
    connect(ui->theme, qOverload<int>(&QComboBox::currentIndexChanged), [](int index) {
        FileUtil::setValue("generate", "theme", index);
    });
    connect(ui->autoBoot, &QCheckBox::stateChanged, [this](int state) {
        if (ui->autoBoot->isChecked()) {
            autoBoot();
        } else {
            closeAutoBoot();
        }
        FileUtil::setValue("generate", "autoBoot", ui->autoBoot->isChecked());
    });
}

GenaratePage::~GenaratePage() {
    delete ui;
}
