#include "mainmenudoctor.h"
#include "ui_mainmenudoctor.h"

mainMenuDoctor::mainMenuDoctor(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::mainMenuDoctor)
{
    ui->setupUi(this);
}

mainMenuDoctor::~mainMenuDoctor()
{
    delete ui;
}

void mainMenuDoctor::on_logoutButton_clicked()
{
    this->close();
}

