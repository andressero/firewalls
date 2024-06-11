#ifndef MAINMENULABORATORY_H
#define MAINMENULABORATORY_H

#include <QWidget>
#include "menupersonaldata.h"
#include "patientselectionmenu.h"

namespace Ui {
class mainMenuLaboratory;
}

class mainMenuLaboratory : public QWidget
{
    Q_OBJECT

public:
    explicit mainMenuLaboratory(QWidget *parent = nullptr);
    ~mainMenuLaboratory();
    void setMainMenuUsername(const std::string& username);

private slots:
    void on_logoutButton_clicked();

    void on_personalDataButton_clicked();

    void on_patientButton_clicked();

    void on_appointmentsButton_clicked();

private:
    Ui::mainMenuLaboratory *ui;
};

#endif // MAINMENULABORATORY_H
