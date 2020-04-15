#include "rename_column.h"
#include "ui_rename_column.h"

rename_column::rename_column(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::rename_column)
{
    ui->setupUi(this);
}

rename_column::~rename_column()
{
    delete ui;
}

void rename_column::on_pushButton_back_clicked()
{
    this->close();
}

void rename_column::on_pushButton_continue_clicked()
{
    emit closed(ui->lineEdit->text());
    this->close();
}
