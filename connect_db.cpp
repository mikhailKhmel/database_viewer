#include "connect_db.h"
#include "ui_connect_db.h"

connect_db::connect_db(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::connect_db)
{
    ui->setupUi(this);
    if (ui->comboBox->currentText() == "SQLITE")
    {
        ui->layoutWidget->setVisible(true);
    }
    else
    {
        ui->layoutWidget->setVisible(false);
    }
}

connect_db::~connect_db()
{
    delete ui;
}


void connect_db::on_pushButton_clicked()
{

}

void connect_db::on_comboBox_currentTextChanged(const QString &arg1)
{
    if (arg1 == "SQLITE")
    {
        ui->layoutWidget->setVisible(true);
    }
    else
    {
        ui->layoutWidget->setVisible(false);
    }
}

