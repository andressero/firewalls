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
        const std::vector<std::string> responseLines = request.splitString(request.requestInsuranceStatus(id), "\n");

        const std::string& status = responseLines[1];

        if (status == "Yes") {
            insurancePositive* positiveInsurance = new insurancePositive(this);
            positiveInsurance->show();
        } else if(status == "No") {
            insuranceNegative* negativeInsurance = new insuranceNegative(this);
            negativeInsurance->show();
        } else {
            // say id not found
        }
    }
}

std::string patientInsuranceMenu::getIDText(){
    return this->ui->idLine->text().toStdString();
}

