#ifndef PATIENTLABMENU_H
#define PATIENTLABMENU_H

#include <QWidget>
#include <QListWidgetItem>

namespace Ui {
class patientLabMenu;
}

class patientLabMenu : public QWidget
{
    Q_OBJECT

public:
    explicit patientLabMenu(QWidget *parent = nullptr);
    ~patientLabMenu();
    void updateLabList(const std::vector<std::string>& labList);

private slots:

    void on_backButton_clicked();

    void on_listWidget_itemDoubleClicked(QListWidgetItem *item);

private:
    Ui::patientLabMenu *ui;
};

#endif // PATIENTLABMENU_H
