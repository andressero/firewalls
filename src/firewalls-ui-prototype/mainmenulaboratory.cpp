#include "mainmenulaboratory.h"
#include "ui_mainmenulaboratory.h"
#include "lablistwindow.h"
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

void mainMenuLaboratory::setMainMenuUsername(const std::string &username) {
    this->ui->label->setText(QString(username.c_str()));
}

void mainMenuLaboratory::on_personalDataButton_clicked()
{
    Request& request = Request::getInstance();
    const std::vector<std::string> response = request.splitString( request.requestUserData(), "\n" );

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
    Request &request = Request::getInstance();
    const std::vector<std::string> responseLines = request.splitString(request.requestPatientList(), "\n");

    if (responseLines[0].find("NOT_OK") != std::string::npos) {
        return;
    }

    PatientSelectionMenu* patientSelectionMenu = new PatientSelectionMenu(this);
    patientSelectionMenu->updatePatientList(responseLines);
    patientSelectionMenu->show();
}


void mainMenuLaboratory::on_appointmentsButton_clicked()
{
    Request &request = Request::getInstance();
    const std::vector<std::string> responseLines = request.splitString(request.requestPatientList(), "\n");

    labListWindow* labList = new labListWindow(this);
    labList->updateLabList(responseLines);
    labList->show();

    //Must add a window for uploading tests already analized.
}

