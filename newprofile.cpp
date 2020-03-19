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
            this->close();
            UserDialog *l = new UserDialog;
            l->show();
        }
        else
            ui->error_label->setVisible(true);
    }
}
