#include "menupersonaldata.h"
#include "ui_menupersonaldata.h"

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

