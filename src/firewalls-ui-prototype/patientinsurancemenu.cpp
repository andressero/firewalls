#include "patientinsurancemenu.h"
#include "ui_patientinsurancemenu.h"
// #include "uservalidation.h"
#include "insurancenegative.h"
#include "insurancepositive.h"

patientInsuranceMenu::patientInsuranceMenu(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::patientInsuranceMenu)
{
    ui->setupUi(this);
}

patientInsuranceMenu::patientInsuranceMenu(QWidget *parent, FileSystem* fs)
    :  QWidget(parent)
    , ui(new Ui::patientInsuranceMenu)
    ,fs(fs) {
    ui->setupUi(this);
}

patientInsuranceMenu::~patientInsuranceMenu()
{
    delete ui;
}

void patientInsuranceMenu::on_backButton_clicked()
{
    this->close();
}


void patientInsuranceMenu::on_consultButton_clicked()
{
    QString id = this->ui->idLine->text();
    if (id == "123456789") {
        // positive window
        insurancePositive* positiveInsurance = new insurancePositive(this);
        positiveInsurance->show();
    } else {
        //                 // negative window
        insuranceNegative* negativeInsurance = new insuranceNegative(this);
        negativeInsurance->show();
    }
    // // Look for insurance bit on FS
    // fs->open("Users");
    // fs->change2ReadMode("Users");
    // QString file(fs->read("Users", 279).c_str());
    // fs->close("Users");
    // int usersSize = -1;

    // UserData* users = tokenizer(file, &usersSize);

    // if(users) {
    //     for (int i = 0; i < usersSize; ++i) {
    //         if (users[i].userID == id) {
    //             if (users[i].rightState == 0) {
    //                 // positive window
    //                 insurancePositive* positiveInsurance = new insurancePositive(this);
    //                 positiveInsurance->show();
    //             } else {
    //                 // negative window
    //                 insuranceNegative* negativeInsurance = new insuranceNegative(this);
    //                 negativeInsurance->show();
    //             }
    //         }
    //     }
    //     delete[] users;
    // }
}

