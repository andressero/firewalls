#ifndef INSURANCEPOSITIVE_H
#define INSURANCEPOSITIVE_H

#include <QWidget>

namespace Ui {
class insurancePositive;
}

class insurancePositive : public QWidget
{
    Q_OBJECT

public:
    explicit insurancePositive(QWidget *parent = nullptr);
    ~insurancePositive();

private slots:
    void on_backButton_clicked();

private:
    Ui::insurancePositive *ui;
};

#endif // INSURANCEPOSITIVE_H
