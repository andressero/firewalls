#include "mainmenupatient.h"
#include "ui_mainmenupatient.h"

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
    menuPersonalData* personalDataMenu = new menuPersonalData(this);
    personalDataMenu->show();
}

