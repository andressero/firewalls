#include "lablistwindow.h"
#include "ui_lablistwindow.h"
#include "labresultinsertion.h"

labListWindow::labListWindow(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::labListWindow)
{
    ui->setupUi(this);
}

labListWindow::~labListWindow()
{
    delete ui;
}

void labListWindow::updateLabList(const std::vector<std::string> &labList) {
    size_t labListSize = labList.size();
    for (size_t i = 1; i < labListSize; ++i) {
        this->ui->listWidget->addItem(QString(labList[i].c_str()));
    }
}

void labListWindow::on_backButton_clicked()
{
    this->close();
}


void labListWindow::on_listWidget_itemDoubleClicked(QListWidgetItem *item)
{
    const std::string& username = item->text().toStdString();
    LabResultInsertion* labResultInsertion = new LabResultInsertion(this);
    labResultInsertion->setUsername(username);
    labResultInsertion->show();
}

