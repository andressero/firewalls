#ifndef MENUPERSONALDATA_H
#define MENUPERSONALDATA_H

#include <QWidget>

namespace Ui {
class menuPersonalData;
}

class menuPersonalData : public QWidget
{
    Q_OBJECT

public:
    explicit menuPersonalData(QWidget *parent = nullptr);
    ~menuPersonalData();

private slots:
    void on_backButton_clicked();

private:
    Ui::menuPersonalData *ui;
};

#endif // MENUPERSONALDATA_H
