#ifndef PATIENTINSURANCEMENU_H
#define PATIENTINSURANCEMENU_H

#include <QWidget>

namespace Ui {
class patientInsuranceMenu;
}

class patientInsuranceMenu : public QWidget
{
    Q_OBJECT

public:
    explicit patientInsuranceMenu(QWidget *parent = nullptr);
    ~patientInsuranceMenu();
    std::string getIDText();

private slots:
    void on_backButton_clicked();

    void on_consultButton_clicked();

private:
    Ui::patientInsuranceMenu *ui;
};

#endif // PATIENTINSURANCEMENU_H
