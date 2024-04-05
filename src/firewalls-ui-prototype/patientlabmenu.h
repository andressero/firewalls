#ifndef PATIENTLABMENU_H
#define PATIENTLABMENU_H

#include <QWidget>

namespace Ui {
class patientLabMenu;
}

class patientLabMenu : public QWidget
{
    Q_OBJECT

public:
    explicit patientLabMenu(QWidget *parent = nullptr);
    ~patientLabMenu();

private slots:

    void on_backButton_clicked();

private:
    Ui::patientLabMenu *ui;
};

#endif // PATIENTLABMENU_H
