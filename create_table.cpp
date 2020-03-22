#include "create_table.h"
#include "ui_create_table.h"

create_table::create_table(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::create_table)
{
    ui->setupUi(this);
}

create_table::~create_table()
{
    delete ui;
}
