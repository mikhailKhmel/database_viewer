#include "newprofile.h"
#include "ui_newprofile.h"
#include "userdialog.h"
#include "config.h"
#include <QDebug>

newprofile::newprofile(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::newprofile)
{
    ui->setupUi(this);
    this->setAttribute(Qt::WA_DeleteOnClose);
    ui->error_label->setVisible(false);
}

newprofile::~newprofile()
{
    delete ui;
}


void newprofile::on_pushButton_clicked()
{
    QString newuser = ui->lineEdit->text();
    if (!newuser.isEmpty())
    {
        if (config::set_new_user(newuser))
        {
            emit closed();
            this->close();
        }
        else
            ui->error_label->setVisible(true);
    }
}

void newprofile::on_pushButton_exit_clicked()
{
    emit closed();
    this->destroy();
}
