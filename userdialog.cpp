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

    l = new MainWindow();
    np = new newprofile();

    connect(l, SIGNAL(closed()), this, SLOT(showd()));
    connect(np, SIGNAL(closed()), this, SLOT(showd()));
    connect(np, SIGNAL(close()), this, SLOT(showd()));
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
    UserDialog::show();
}

void UserDialog::on_pushButton_clicked()
{
    np->show();
    this->close();
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
    this->close();
}
