#include "loginwindow.h"
#include "ui_loginwindow.h"
#include "uservalidation.h"
#include <QDebug>

LoginWindow::LoginWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::CCSS_Garrobo)
{
    // this->fileSystem = fs;
    ui->setupUi(this);
}

LoginWindow::LoginWindow(QWidget *parent, FileSystem* fs)
    : QMainWindow(parent)
    , ui(new Ui::CCSS_Garrobo), fileSystem(fs)
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
    QString password = ui->passwordLine->text();
    static QRegularExpression doctorID("D[0-9]{5}");
    static QRegularExpression laboratoristID("L[0-9]{5}");
    static QRegularExpression patientID("[0-9]{9}");
    static QRegularExpression passwordID(".{8,64}");

    if (passwordID.match(password).hasMatch() == false) {
        qInfo() << "LoginWindow: Not a valid password\n";
        return;
    }

    // Doctor login
    if (doctorID.match(username).hasMatch()) {
        if (validateLogin(this->fileSystem, username, password)) {
            mainMenuDoctor* doctorMenu = new mainMenuDoctor(this);
            doctorMenu->show();
        }
    }

    if (laboratoristID.match(username).hasMatch()) {
        if (validateLogin(this->fileSystem, username, password)) {
            mainMenuLaboratory* laboratoryMenu = new mainMenuLaboratory(this);
            laboratoryMenu->show();
        }
    }

    if (patientID.match(username).hasMatch()) {
        qInfo() << "LoginWindow: Valid patient\n";
        if (validateLogin(this->fileSystem, username, password)) {
            qInfo() << "LoginWindow: Successful login\n";
            mainMenuPatient* patientMenu = new mainMenuPatient(this, this->fileSystem);
            patientMenu->show();
        }
    }

}

