#ifndef MAINMENUDOCTOR_H
#define MAINMENUDOCTOR_H

#include <QWidget>
#include "menupersonaldata.h"
#include "patientselectionmenu.h"

namespace Ui {
class mainMenuDoctor;
}

class mainMenuDoctor : public QWidget
{
    Q_OBJECT

public:
    explicit mainMenuDoctor(QWidget *parent = nullptr);
    ~mainMenuDoctor();

private slots:
    void on_logoutButton_clicked();

    void on_personalDataButton_clicked();

    void on_patientsButton_clicked();

private:
    Ui::mainMenuDoctor *ui;
};

#endif // MAINMENUDOCTOR_H
