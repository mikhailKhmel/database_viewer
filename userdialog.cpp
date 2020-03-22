#include "userdialog.h"
#include "newprofile.h"
#include "ui_userdialog.h"
#include "config.h"
#include "mainwindow.h"
#include <QDebug>

UserDialog::UserDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::UserDialog)
{
    ui->setupUi(this);
    ui->comboBox->addItems(config::users);
}

UserDialog::~UserDialog()
{
    delete ui;
}

void UserDialog::on_pushButton_clicked()
{
    this->close();
    newprofile *l = new newprofile;
    l->show();
}

void UserDialog::on_pushButton_2_clicked()
{
    QString username =ui->comboBox->currentText();
    if (!username.isEmpty())
        config::set_current_user();
    else
        return;
    this->close();
    MainWindow *l = new MainWindow;
    l->show();
}
