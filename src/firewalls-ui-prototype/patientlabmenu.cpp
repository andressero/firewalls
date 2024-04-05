#include "patientlabmenu.h"
#include "ui_patientlabmenu.h"

patientLabMenu::patientLabMenu(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::patientLabMenu)
{
    ui->setupUi(this);
}

patientLabMenu::~patientLabMenu()
{
    delete ui;
}

void patientLabMenu::on_backButton_clicked()
{
    this->close();
}

