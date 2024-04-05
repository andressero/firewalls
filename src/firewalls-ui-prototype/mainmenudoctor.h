#ifndef MAINMENUDOCTOR_H
#define MAINMENUDOCTOR_H

#include <QWidget>

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

private:
    Ui::mainMenuDoctor *ui;
};

#endif // MAINMENUDOCTOR_H
