#ifndef MAINMENUPATIENT_H
#define MAINMENUPATIENT_H

#include <QWidget>
#include "patientlabmenu.h"
#include "menupersonaldata.h"
#include "filesystem.h"

namespace Ui {
class mainMenuPatient;
}

class mainMenuPatient : public QWidget
{
    Q_OBJECT

public:
    explicit mainMenuPatient(QWidget *parent = nullptr);
    ~mainMenuPatient();

private slots:
    void on_laboratoryButton_clicked();

    void on_logoutButton_clicked();

    void on_personalDataButton_clicked();

    void on_insuranceButton_clicked();

private:
    Ui::mainMenuPatient *ui;
};

#endif // MAINMENUPATIENT_H
