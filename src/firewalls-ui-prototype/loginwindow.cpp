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
        if (validateLogin(username.toStdString(), password.toStdString())) {
            mainMenuDoctor* doctorMenu = new mainMenuDoctor(this);
            doctorMenu->show();
        } else {
            ui->incorrectPasswordLabel->setText("Contraseña incorrecta");
        }
    }

    if (laboratoristID.match(username).hasMatch()) {
        if (validateLogin(username.toStdString(), password.toStdString())) {
            mainMenuLaboratory* laboratoryMenu = new mainMenuLaboratory(this);
            laboratoryMenu->show();
        } else {
            ui->incorrectPasswordLabel->setText("Contraseña incorrecta");
        }
    }

    if (patientID.match(username).hasMatch()) {
        qInfo() << "LoginWindow: Valid patient\n";
        if (validateLogin(username.toStdString(), password.toStdString())) {
            qInfo() << "LoginWindow: Successful login\n";
            mainMenuPatient* patientMenu = new mainMenuPatient(this);
            patientMenu->show();
        } else {
            ui->incorrectPasswordLabel->setText("Contraseña incorrecta");
            qInfo() << "LoginWindows: Unsuccessful login\n";
        }
    }

}

