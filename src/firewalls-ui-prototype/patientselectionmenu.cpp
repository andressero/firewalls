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

void PatientSelectionMenu::updatePatientList(const std::vector<std::string>& patientList) {
    const size_t patientListSize = patientList.size();

    for (size_t i = 1; i < patientListSize; ++i) {
        this->ui->listWidget->addItem(QString(patientList[i].c_str()));
    }
}

void PatientSelectionMenu::on_listWidget_itemDoubleClicked(QListWidgetItem *item)
{

}

