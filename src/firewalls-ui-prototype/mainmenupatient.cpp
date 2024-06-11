#include "mainmenupatient.h"
#include "ui_mainmenupatient.h"
#include "patientinsurancemenu.h"
#include "request.h"

mainMenuPatient::mainMenuPatient(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::mainMenuPatient)
{
    ui->setupUi(this);
}

mainMenuPatient::~mainMenuPatient()
{
    delete ui;
}

void mainMenuPatient::setMainMenuUsername(const std::string &username) {
    this->ui->label->setText(QString(username.c_str()));
}

void mainMenuPatient::on_laboratoryButton_clicked()
{
    Request &request = Request::getInstance();
    std::string labList;
    const std::vector<std::string> responseLines = request.splitString(request.requestLabList(), "\n");
    // for (size_t i = 1; i < responseLines.size(); ++i) {
    //     labList += responseLines[i];
    // }

    // if (responseLines[0].find("NOT_OK") != std::string::npos) {
    //     return;
    // }

    patientLabMenu* labMenu = new patientLabMenu(this);
    labMenu->updateLabList(responseLines);
    labMenu->show();
}


void mainMenuPatient::on_logoutButton_clicked()
{
    this->close();
}


void mainMenuPatient::on_personalDataButton_clicked()
{
    Request &request = Request::getInstance();
    std::string personalData;
    const std::vector<std::string> responseLines = request.splitString(request.requestUserData(), "\n");
    for (size_t i = 1; i < responseLines.size(); ++i) {
        personalData += responseLines[i];
    }

    if (responseLines[0].find("NOT_OK") != std::string::npos) {
        return;
    }
    menuPersonalData* personalDataMenu = new menuPersonalData(this);
    personalDataMenu->updatePersonalData(personalData);
    personalDataMenu->show();
    return;
}


void mainMenuPatient::on_insuranceButton_clicked()
{
    patientInsuranceMenu* insuranceMenu = new patientInsuranceMenu(this);
    insuranceMenu->show();
}

