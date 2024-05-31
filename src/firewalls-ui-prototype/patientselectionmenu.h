#ifndef PATIENTSELECTIONMENU_H
#define PATIENTSELECTIONMENU_H

#include <QWidget>

namespace Ui {
class PatientSelectionMenu;
}

class PatientSelectionMenu : public QWidget
{
    Q_OBJECT

public:
    explicit PatientSelectionMenu(QWidget *parent = nullptr);
    ~PatientSelectionMenu();

private slots:
    void on_backButton_clicked();

private:
    Ui::PatientSelectionMenu *ui;
};

#endif // PATIENTSELECTIONMENU_H
