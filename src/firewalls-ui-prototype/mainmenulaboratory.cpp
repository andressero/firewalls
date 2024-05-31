#include "mainmenulaboratory.h"
#include "ui_mainmenulaboratory.h"

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
    menuPersonalData* personalDataMenu = new menuPersonalData(this);
    personalDataMenu->show();
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

