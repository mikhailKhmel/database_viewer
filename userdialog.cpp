#include "userdialog.h"

#include "ui_userdialog.h"
#include "config.h"

#include <QDebug>

UserDialog::UserDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::UserDialog)
{
    ui->setupUi(this);
    this->setAttribute(Qt::WA_DeleteOnClose);

    QStringList users;
    foreach(config::current_user s, config::users) {users.append(s.username);}
    ui->comboBox->addItems(users);

//    l = new MainWindow();
//    np = new newprofile();
    np = new newprofile;
    connect(np, SIGNAL(closed()), this, SLOT(showd()));
    l = new MainWindow;
    connect(l, SIGNAL(closedd()), this, SLOT(showd()));

}

UserDialog::~UserDialog()
{
    delete ui;
}

void UserDialog::showd()
{
    ui->comboBox->clear();
    QStringList users;
    foreach(config::current_user s, config::users) {users.append(s.username);}
    ui->comboBox->addItems(users);
    this->update();
    this->setVisible(true);
}

void UserDialog::on_pushButton_clicked()
{

    np->show();
    this->setVisible(false);
    //this->close();
}

void UserDialog::on_pushButton_2_clicked()
{
    QString username =ui->comboBox->currentText();
    if (!username.isEmpty())
        config::set_current_user(username);
    else
        return;

    l->prepare_window();
    l->show();
    this->setVisible(false);
}
