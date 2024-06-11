#include "mainmenulaboratory.h"
#include "ui_mainmenulaboratory.h"
#include <request.h>

mainMenuLaboratory::mainMenuLaboratory(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::mainMenuLaboratory)
{
    ui->setupUi(this);
}

mainMenuLaboratory::~mainMenuLaboratory()
{
    delete ui;
}

void mainMenuLaboratory::on_logoutButton_clicked()
{
    this->close();
}

void mainMenuLaboratory::on_personalDataButton_clicked()
{
    Request& request = Request::getInstance();
    const std::vector<std::string> response = request.splitString( request.requestLogin(), "\n" );

    if (response[0].find("NOT_OK") != std::string::npos) {
        return;
    }
    menuPersonalData* personalDataMenu = new menuPersonalData(this);
    personalDataMenu->updatePersonalData(response[1]);
    personalDataMenu->show();
    return;
}

void mainMenuLaboratory::on_patientButton_clicked()
{
    PatientSelectionMenu* patientSelectionMenu = new PatientSelectionMenu();
    patientSelectionMenu->show();
}


void mainMenuLaboratory::on_appointmentsButton_clicked()
{
    //Must add a window for uploading tests already analized.
}

