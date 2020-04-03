#include "mainwindow.h"
#include "ui_mainwindow.h"


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    tables_list_model = new QStringListModel(this);

    this->setAttribute(Qt::WA_DeleteOnClose);

    c_db = new connect_db;
    create_table_window = new create_table;
    connect(c_db, SIGNAL(closed()), this, SLOT(prepare_window()));
    connect(create_table_window, SIGNAL(closed()), this , SLOT(prepare_window()));

    ui->listView_tables->setContextMenuPolicy(Qt::CustomContextMenu);
    ui->tableView->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(ui->listView_tables, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(showContextMenu(QPoint)));
    connect(ui->tableView, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(showContextMenu_table(QPoint)));


    ui->listView_tables->setViewMode(QListView::ListMode);
    ui->splitter->setStretchFactor(0,0);
    ui->splitter->setStretchFactor(1,1);
    ui->listView_tables->show();

    //ui->listView_tables->addAction(new QAction("Удалить таблицу" ,this));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::showContextMenu_table(const QPoint &pos)
{
    QPoint globalPos = ui->listView_tables->mapToGlobal(pos);

    // Create menu and insert some actions
    QMenu myMenu;
    myMenu.addAction("Добавить строку", this, SLOT(addRow()));

    // Show context menu at handling position
    myMenu.exec(globalPos);
}

void MainWindow::showContextMenu(const QPoint &pos)
{
    // Handle global position
    QPoint globalPos = ui->listView_tables->mapToGlobal(pos);

    // Create menu and insert some actions
    QMenu myMenu;
    myMenu.addAction("Создать таблицу", this, SLOT(on_create_table_triggered()));
    myMenu.addAction("Удалить таблицу", this, SLOT(deleteTable()));

    // Show context menu at handling position
    myMenu.exec(globalPos);
}

void MainWindow::addRow()
{
    QModelIndex index = ui->listView_tables->currentIndex();
    ui->listView_tables->setSelectionMode(QAbstractItemView::ExtendedSelection);
    QString tablename = index.data(Qt::DisplayRole).toString();
    QSqlTableModel* model = new QSqlTableModel;
    model->setTable(tablename);
    model->select();
    model->insertRow(0);
    ui->tableView->setModel(model);
    ui->tableView->show();
}

void MainWindow::deleteTable()
{

    QModelIndex index = ui->listView_tables->currentIndex();
    QString tableName = index.data(Qt::DisplayRole).toString();

    QSqlDatabase db = QSqlDatabase::addDatabase(config::user.db_driver);
    db.setDatabaseName(config::user.dir_db_sqlite);
    if (db.open())
    {
        QSqlQuery q;
        q.prepare("DROP TABLE "+tableName);
        if (q.exec())
        {
            MainWindow::prepare_window();
            QModelIndex index = ui->listView_tables->currentIndex();
            ui->listView_tables->setSelectionMode(QAbstractItemView::ExtendedSelection);
            QString tablename = index.data(Qt::DisplayRole).toString();
            QSqlTableModel* model = new QSqlTableModel;
            model->setTable(tablename);
            model->select();
            ui->tableView->setModel(model);
            ui->tableView->show();
        }
        else
            qDebug() << q.lastError().text();
    }
    else
        qDebug() << db.lastError().text();

    ui->tableView->reset();
}

void MainWindow::prepare_window()
{
    QSqlDatabase db = QSqlDatabase::addDatabase(config::user.db_driver);
    db.setDatabaseName(config::user.dir_db_sqlite);
    if (db.open())
    {
        tables_list_model = new QStringListModel;
        tables_list_model->setStringList(QStringList {});
        tables_list_model->setStringList(db.tables());
        ui->listView_tables->setModel(tables_list_model);
        ui->listView_tables->show();

        qDebug() << "tables_list";
    }
    else
        qDebug() << db.lastError().text();

    this->setWindowTitle("Текущий пользователь: "+config::user.username);
}

void MainWindow::resizeEvent(QResizeEvent *event)
{

    QSize s = event->size();
    ui->centralwidget->resize(s);
}

void MainWindow::on_connect_db_triggered()
{
    c_db->show();
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
    QSqlDatabase db = QSqlDatabase::addDatabase(config::user.db_driver);

    db.setDatabaseName(config::user.dir_db_sqlite);
    if (db.open())
    {
        create_table_window->prepare_window();
        create_table_window->show();
    }
    else
        QMessageBox::warning(this,"Ошибка", "Сначала подключитесь к базе данных");
}

void MainWindow::on_tableView_doubleClicked(const QModelIndex &index)
{
    ui->listView_tables->setSelectionMode(QAbstractItemView::ExtendedSelection);
    QString tablename = index.data(Qt::DisplayRole).toString();
    QSqlTableModel* model = new QSqlTableModel;
    model->setTable(tablename);
    model->select();
    model->insertRow(0, index);
    ui->tableView->setModel(model);
    ui->tableView->show();
}

void MainWindow::on_exit_profile_triggered()
{
    config::user.lastused = 0;
    config::user.username = "";
    config::user.db_driver = "";
    config::user.dir_db_sqlite = "";
    emit closedd();
    this->destroy();
}

void MainWindow::on_save_profile_triggered()
{
    config::save_config();
}

void MainWindow::on_listView_tables_customContextMenuRequested(const QPoint &pos)
{

}
