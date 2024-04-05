#include "loginwindow.h"
#include "ui_loginwindow.h"

LoginWindow::LoginWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::CCSSApp)
{
    ui->setupUi(this);
}

LoginWindow::~LoginWindow()
{
    delete ui;
}

void LoginWindow::on_loginButton_clicked()
{
    QString username = ui->userLine->text();
    // QString password = ui->passwordLine->text();
    static QRegularExpression doctorID("[A-Z][0-9]{5}");
    static QRegularExpression patientID("[0-9]{9}");

    // Doctor login
    if (doctorID.match(username).hasMatch()) {
        mainMenuDoctor* doctorMenu = new mainMenuDoctor(this);
        doctorMenu->show();
        // delete(doctorMenu);

    }
    if (patientID.match(username).hasMatch()) {
        mainMenuPatient* patientMenu = new mainMenuPatient(this);
        patientMenu->show();
        // delete(patientMenu);
    }

}

