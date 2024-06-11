#include "labresultinsertion.h"
#include "ui_labresultinsertion.h"
#include "request.h"
#include "menupersonaldata.h"

LabResultInsertion::LabResultInsertion(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::LabResultInsertion)
{
    ui->setupUi(this);
}

LabResultInsertion::~LabResultInsertion()
{
    delete ui;
}

void LabResultInsertion::setUsername(const std::string &username) {
    this->patientUsername = username;
}

void LabResultInsertion::on_checkPatientData_clicked()
{
    Request& request = Request::getInstance();
    std::string patientData = request.requestPatientData(this->patientUsername);

    menuPersonalData* patientDataMenu = new menuPersonalData(this);
    patientDataMenu->updatePersonalData(patientData);
    patientDataMenu->show();
}


void LabResultInsertion::on_addButton_clicked()
{
    std::string date = this->ui->dateLineEdit->text().toStdString();
    std::string form = this->ui->formTextEdit->toPlainText().toStdString();
    std::string pending = (this->ui->isPending->isChecked()) ? "Yes":"No";

    size_t formSize = form.size();
    for (size_t i = 0; i < formSize; ++i) {
        if (form[i] == ' ') {
            form[i] = '_';
        }
    }

    // std::string form = this->ui->formTextEdit->text();

    std::string result = this->patientUsername + " " + date + " " + pending + " " + form;

    Request& request = Request::getInstance();
    request.requestDataInsertion(result);

}


void LabResultInsertion::on_backButton_clicked()
{
    this->close();
}

