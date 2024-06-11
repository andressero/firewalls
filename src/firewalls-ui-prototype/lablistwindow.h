#ifndef LABLISTWINDOW_H
#define LABLISTWINDOW_H

#include <QWidget>
#include <QListWidgetItem>

namespace Ui {
class labListWindow;
}

class labListWindow : public QWidget
{
    Q_OBJECT

public:
    explicit labListWindow(QWidget *parent = nullptr);
    ~labListWindow();
    void updateLabList(const std::vector<std::string>& labList);

private slots:
    void on_backButton_clicked();

    void on_listWidget_itemDoubleClicked(QListWidgetItem *item);

private:
    Ui::labListWindow *ui;
};

#endif // LABLISTWINDOW_H
