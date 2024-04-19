#ifndef LOGINWINDOW_H
#define LOGINWINDOW_H

#include <QMainWindow>
#include <QRegularExpression>
#include "mainmenudoctor.h"
#include "mainmenupatient.h"
#include "mainmenulaboratory.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class CCSS_Garrobo;
}
QT_END_NAMESPACE

class LoginWindow : public QMainWindow
{
    Q_OBJECT

public:
    LoginWindow(QWidget *parent = nullptr);
    ~LoginWindow();

private slots:
    void on_loginButton_clicked();

private:
    Ui::CCSS_Garrobo *ui;
};
#endif // LOGINWINDOW_H
