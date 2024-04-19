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
