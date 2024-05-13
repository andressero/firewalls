#include "mainmenupatient.h"
#include "ui_mainmenupatient.h"
#include "patientinsurancemenu.h"

mainMenuPatient::mainMenuPatient(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::mainMenuPatient)
{
    ui->setupUi(this);
}

mainMenuPatient::mainMenuPatient(QWidget *parent, FileSystem* fs)
    : QWidget(parent)
    , ui(new Ui::mainMenuPatient)
    , fs(fs)
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


void mainMenuPatient::on_insuranceButton_clicked()
{
    patientInsuranceMenu* insuranceMenu = new patientInsuranceMenu(this, this->fs);
    insuranceMenu->show();
}

