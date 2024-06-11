#include "patientinsurancemenu.h"
#include "ui_patientinsurancemenu.h"
// #include "uservalidation.h"
#include "insurancenegative.h"
#include "insurancepositive.h"
#include "request.h"

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
    std::string id = this->ui->idLine->text().toStdString();

    if (id.size() > 0) {
        Request& request = Request::getInstance();
        // TODO(any): make server accept any id
        std::string status = request.requestInsuranceStatus(id);

        //qInfo() << status << "\n";
        if (status == "OK\nOK\n1\n") {
            insurancePositive* positiveInsurance = new insurancePositive(this);
            positiveInsurance->show();
        } else if(status == "OK\nOK\n0\n") {
            insuranceNegative* negativeInsurance = new insuranceNegative(this);
            negativeInsurance->show();
        }
    }
}

