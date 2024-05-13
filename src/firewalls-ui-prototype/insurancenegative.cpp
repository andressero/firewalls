#include "insurancenegative.h"
#include "ui_insurancenegative.h"

insuranceNegative::insuranceNegative(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::insuranceNegative)
{
    ui->setupUi(this);
}

insuranceNegative::~insuranceNegative()
{
    delete ui;
}

void insuranceNegative::on_backButton_clicked()
{
    this->close();
}

