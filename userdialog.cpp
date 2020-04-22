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
    if (config::user.lightmode == 0)
    {
        QFile styleF;
        styleF.setFileName(":/dark.css");
        styleF.open(QFile::ReadOnly);
        QString qssStr = styleF.readAll();
        qApp->setStyleSheet(qssStr);
    }
    else
    {
        QFile styleF;
        styleF.setFileName(":/light.css");
        styleF.open(QFile::ReadOnly);
        QString qssStr = styleF.readAll();
        qApp->setStyleSheet(qssStr);
    }

    ui->comboBox->clear();
    QStringList users;
    foreach(config::current_user s, config::users) {users.append(s.username);}
    ui->comboBox->addItems(users);
    this->update();
    this->setVisible(true);
}

void UserDialog::on_toolButton_clicked()
{

    np->show();
    this->setVisible(false);
    //this->close();
}

void UserDialog::on_toolButton_2_clicked()
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

void UserDialog::on_toolButton_3_clicked()
{
    this->close();
}

void UserDialog::on_comboBox_currentIndexChanged(const QString &arg1)
{
    for (int i = 0; i < config::users.count(); ++i) {
        if (arg1 == config::users.at(i).username)
        {
            if (config::users.at(i).lightmode == 0)
            {
                QFile styleF;
                styleF.setFileName(":/dark.css");
                styleF.open(QFile::ReadOnly);
                QString qssStr = styleF.readAll();
                qApp->setStyleSheet(qssStr);
            }
            else
            {
                QFile styleF;
                styleF.setFileName(":/light.css");
                styleF.open(QFile::ReadOnly);
                QString qssStr = styleF.readAll();
                qApp->setStyleSheet(qssStr);
            }
        }
    }
}
