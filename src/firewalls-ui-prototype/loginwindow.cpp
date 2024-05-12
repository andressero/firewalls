#include "loginwindow.h"
#include "ui_loginwindow.h"
#include "userValidation.h"

LoginWindow::LoginWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::CCSS_Garrobo)
{
    // this->fileSystem = fs;
    ui->setupUi(this);
}

// LoginWindow::LoginWindow(QWidget *parent, FileSystem& fs)
//     : QMainWindow(parent)
//     , ui(new Ui::CCSS_Garrobo), fileSystem(fs)
// {
//     // this->fileSystem = fs;
//     ui->setupUi(this);
// }

LoginWindow::~LoginWindow()
{
    delete ui;
}

void LoginWindow::on_loginButton_clicked()
{
    QString username = ui->userLine->text();
    // QString password = ui->passwordLine->text();
    static QRegularExpression doctorID("D[0-9]{5}");
    static QRegularExpression laboratoristID("L[0-9]{5}");
    static QRegularExpression patientID("[0-9]{9}");
    static QRegularExpression passwordID("^(?=.[a-z])(?=.[A-Z])(?=.\d)(?=.[#$%^&+=!_-()=?*.<>]).{8,64}$");

    QString password = ui->passwordLine->text();

    if (!passwordID.match(password).hasMatch()) {
        return;
    }



    // Doctor login
    if (doctorID.match(username).hasMatch()) {
        // if (validateLogin(this->fileSystem, username, password)) {
        //     mainMenuDoctor* doctorMenu = new mainMenuDoctor(this);
        //     doctorMenu->show();
        // }

        mainMenuDoctor* doctorMenu = new mainMenuDoctor(this);
        doctorMenu->show();
    }

    if (laboratoristID.match(username).hasMatch()) {
        // if (validateUser(this->fileSystem, username, password)) {
        //     mainMenuLaboratory* laboratoryMenu = new mainMenuLaboratory(this);
        //     laboratoryMenu->show();
        // }
        mainMenuLaboratory* laboratoryMenu = new mainMenuLaboratory(this);
        laboratoryMenu->show();
    }

    if (patientID.match(username).hasMatch()) {
        // if (validateUser(this->fileSystem, username, password)) {
        //     mainMenuPatient* patientMenu = new mainMenuPatient(this);
        //     patientMenu->show();
        // }
        mainMenuPatient* patientMenu = new mainMenuPatient(this);
        patientMenu->show();
    }

}

