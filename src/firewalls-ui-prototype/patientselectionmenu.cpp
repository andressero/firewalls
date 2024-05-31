#include "patientselectionmenu.h"
#include "ui_patientselectionmenu.h"

PatientSelectionMenu::PatientSelectionMenu(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::PatientSelectionMenu)
{
    ui->setupUi(this);
    ui->listWidget->addItem("Joe Biden - 288884444");
}

PatientSelectionMenu::~PatientSelectionMenu()
{
    delete ui;
}

void PatientSelectionMenu::on_backButton_clicked()
{
    this->close();
}

