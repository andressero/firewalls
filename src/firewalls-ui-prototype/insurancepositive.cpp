#include "insurancepositive.h"
#include "ui_insurancepositive.h"

insurancePositive::insurancePositive(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::insurancePositive)
{
    ui->setupUi(this);
}

insurancePositive::~insurancePositive()
{
    delete ui;
}

void insurancePositive::on_backButton_clicked()
{
    this->close();
}

