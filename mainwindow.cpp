#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "config.h"
#include "connect_db.h"
#include <QDebug>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_connect_db_triggered()
{
    connect_db *l = new connect_db;
    l->show();
}
