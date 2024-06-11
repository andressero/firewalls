#include "patientlabmenu.h"
#include "ui_patientlabmenu.h"
#include "request.h"
#include "menupersonaldata.h"

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

void patientLabMenu::updateLabList(const std::vector<std::string>& labList) {
    const size_t labListSize = labList.size();
    for (size_t i = 1; i < labListSize; ++i) {
        this->ui->listWidget->addItem(QString(labList[i].c_str()));
    }
}

void patientLabMenu::on_listWidget_itemDoubleClicked(QListWidgetItem *item)
{
    Request& request = Request::getInstance();
    std::string labItem = item->text().toStdString();
    std::string labDate = request.splitString(labItem, " ")[0];

    std::string labResult = request.requestLabResult(labDate);
    menuPersonalData* personalDataMenu= new menuPersonalData(this);
    personalDataMenu->updatePersonalData(labResult);
    personalDataMenu->show();
}

