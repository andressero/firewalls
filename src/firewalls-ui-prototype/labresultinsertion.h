#ifndef LABRESULTINSERTION_H
#define LABRESULTINSERTION_H

#include <QWidget>

namespace Ui {
class LabResultInsertion;
}

class LabResultInsertion : public QWidget
{
    Q_OBJECT

public:
    explicit LabResultInsertion(QWidget *parent = nullptr);
    ~LabResultInsertion();

    void setUsername(const std::string& username);

private slots:
    void on_checkPatientData_clicked();

    void on_addButton_clicked();

    void on_backButton_clicked();

private:
    Ui::LabResultInsertion *ui;
    std::string patientUsername;
};

#endif // LABRESULTINSERTION_H
