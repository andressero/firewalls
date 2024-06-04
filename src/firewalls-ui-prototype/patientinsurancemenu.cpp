#include "patientinsurancemenu.h"
#include "ui_patientinsurancemenu.h"
// #include "uservalidation.h"
#include "insurancenegative.h"
#include "insurancepositive.h"

patientInsuranceMenu::patientInsuranceMenu(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::patientInsuranceMenu)
{
    ui->setupUi(this);
}

patientInsuranceMenu::~patientInsuranceMenu()
{
    delete ui;
}

void patientInsuranceMenu::on_backButton_clicked()
{
    this->close();
}


void patientInsuranceMenu::on_consultButton_clicked()
{
    QString id = this->ui->idLine->text();
    if (id == "123456789") {
        // positive window
        insurancePositive* positiveInsurance = new insurancePositive(this);
        positiveInsurance->show();
    } else {
        //                 // negative window
        insuranceNegative* negativeInsurance = new insuranceNegative(this);
        negativeInsurance->show();
    }
}

