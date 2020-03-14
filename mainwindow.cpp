#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "config.h"
#include <QDebug>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    foreach(QString s, config::current_user)
        qDebug() << s;
}

MainWindow::~MainWindow()
{
    delete ui;
}
