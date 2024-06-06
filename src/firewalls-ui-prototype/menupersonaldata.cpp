#include "menupersonaldata.h"
#include "ui_menupersonaldata.h"
#include <QString>

menuPersonalData::menuPersonalData(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::menuPersonalData)
{
    ui->setupUi(this);
}

menuPersonalData::~menuPersonalData()
{
    delete ui;
}

void menuPersonalData::on_backButton_clicked()
{
    this->close();
}

void menuPersonalData::updatePersonalData(const std::string& personalData) {
    this->ui->textBrowser->setText(QString(personalData.c_str()));
}
