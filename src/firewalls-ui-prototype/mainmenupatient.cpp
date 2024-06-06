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

void mainMenuPatient::on_laboratoryButton_clicked()
{
    patientLabMenu* labMenu = new patientLabMenu(this);
    labMenu->show();
}


void mainMenuPatient::on_logoutButton_clicked()
{
    this->close();
}


void mainMenuPatient::on_personalDataButton_clicked()
{
    Request &request = Request::getInstance();
    std::string personalData = request.requestUserData();

    if (personalData == "ERROR\n") {
        // send error and don't create the window
    }

    menuPersonalData* personalDataMenu = new menuPersonalData(this);
    personalDataMenu->updatePersonalData(personalData);
    personalDataMenu->show();
}


void mainMenuPatient::on_insuranceButton_clicked()
{
    patientInsuranceMenu* insuranceMenu = new patientInsuranceMenu(this);
    insuranceMenu->show();
}

