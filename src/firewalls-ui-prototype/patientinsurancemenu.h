#ifndef PATIENTINSURANCEMENU_H
#define PATIENTINSURANCEMENU_H

#include <QWidget>
#include "filesystem.h"

namespace Ui {
class patientInsuranceMenu;
}

class patientInsuranceMenu : public QWidget
{
    Q_OBJECT

public:
    explicit patientInsuranceMenu(QWidget *parent = nullptr);
    patientInsuranceMenu(QWidget *parent = nullptr, FileSystem* fs = nullptr);
    ~patientInsuranceMenu();

private slots:
    void on_backButton_clicked();

    void on_consultButton_clicked();

private:
    Ui::patientInsuranceMenu *ui;
    FileSystem* fs;
};

#endif // PATIENTINSURANCEMENU_H
