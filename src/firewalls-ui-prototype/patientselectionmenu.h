#ifndef PATIENTSELECTIONMENU_H
#define PATIENTSELECTIONMENU_H

#include <QWidget>
#include <QListWidgetItem>

namespace Ui {
class PatientSelectionMenu;
}

class PatientSelectionMenu : public QWidget
{
    Q_OBJECT

public:
    explicit PatientSelectionMenu(QWidget *parent = nullptr);
    ~PatientSelectionMenu();
    void updatePatientList(const std::vector<std::string>& patientList);

private slots:
    void on_backButton_clicked();

    void on_listWidget_itemDoubleClicked(QListWidgetItem *item);

private:
    Ui::PatientSelectionMenu *ui;
};

#endif // PATIENTSELECTIONMENU_H
