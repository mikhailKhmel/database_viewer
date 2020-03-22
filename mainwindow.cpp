#include "mainwindow.h"
#include "ui_mainwindow.h"


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    l = new connect_db;
    connect(l, SIGNAL(close()), this, SLOT(load_db()));
    ui->listView_tables->setViewMode(QListView::ListMode);
    ui->splitter->setStretchFactor(0,0);
    ui->splitter->setStretchFactor(1,1);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::resizeEvent(QResizeEvent *event)
{

    QSize s = event->size();
    ui->centralwidget->resize(s);
}

void MainWindow::on_connect_db_triggered()
{
    l->show();
}

void MainWindow::load_db()
{
    config::work_db.open();
    QStringList tables_list = config::work_db.tables();
    QStringListModel* tables_list_model = new QStringListModel(this);
    tables_list_model->setStringList(tables_list);
    ui->listView_tables->setModel(tables_list_model);
    qDebug() << tables_list;
}

void MainWindow::on_listView_tables_doubleClicked(const QModelIndex &index)
{
    ui->listView_tables->setSelectionMode(QAbstractItemView::ExtendedSelection);
    QString tablename = index.data(Qt::DisplayRole).toString();
    QSqlTableModel* model = new QSqlTableModel;
    model->setTable(tablename);
    model->select();
    ui->tableView->setModel(model);
    ui->tableView->show();
}

void MainWindow::on_create_table_triggered()
{

}

