#ifndef INSURANCENEGATIVE_H
#define INSURANCENEGATIVE_H

#include <QWidget>

namespace Ui {
class insuranceNegative;
}

class insuranceNegative : public QWidget
{
    Q_OBJECT

public:
    explicit insuranceNegative(QWidget *parent = nullptr);
    ~insuranceNegative();

private slots:
    void on_backButton_clicked();

private:
    Ui::insuranceNegative *ui;
};

#endif // INSURANCENEGATIVE_H
