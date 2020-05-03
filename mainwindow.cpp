#include "mainwindow.h"
#include "ui_mainwindow.h"


MainWindow::MainWindow(QWidget *parent) :
        QMainWindow(parent),
        ui(new Ui::MainWindow) {
    ui->setupUi(this);
    tables_list_model = new QStringListModel(this);

    //this->setWindowFlag(Qt::FramelessWindowHint);
    this->setAttribute(Qt::WA_DeleteOnClose);

    c_db = new connect_db;
    create_table_window = new create_table;
    r_c = new rename_column;
    u_c = new uncover_columns;

    connect(c_db, SIGNAL(closed()), this, SLOT(prepare_window()));
    connect(create_table_window, SIGNAL(closed()), this, SLOT(prepare_window()));

    ui->listView_tables->setContextMenuPolicy(Qt::CustomContextMenu);
    ui->tableView->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(ui->listView_tables, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(showContextMenu(QPoint)));
    connect(ui->tableView, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(showContextMenu_table(QPoint)));
    connect(r_c, SIGNAL(closed(
    const QString &)), this, SLOT(renameColumn1(
    const QString &)));
    connect(u_c, SIGNAL(closed(
    const QString &)), this, SLOT(uncoverColumn1(
    const QString &)));
/*перенос функций редактора скрипта в основное окно*/
    ui->listView_tables->setViewMode(QListView::ListMode);
    ui->splitter->setStretchFactor(0, 1);
    ui->splitter->setStretchFactor(1, 4);

    ui->splitter_2->setStretchFactor(0,2);
    ui->splitter_2->setStretchFactor(1,1);

    ui->listView_tables->show();
}

MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::mousePressEvent(QMouseEvent *event) {
    m_nMouseClick_X_Coordinate = event->x();
    m_nMouseClick_Y_Coordinate = event->y();
}

void MainWindow::mouseMoveEvent(QMouseEvent *event) {
    move(event->globalX() - m_nMouseClick_X_Coordinate, event->globalY() - m_nMouseClick_Y_Coordinate);
}

void MainWindow::showContextMenu_table(const QPoint &pos) {
    QPoint globalPos = ui->tableView->mapToGlobal(pos);

    // Create menu and insert some actions
    QMenu myMenu;

    myMenu.addAction("Переименовать столбец", this, SLOT(renameColumn()));
    myMenu.addAction("Скрыть столбец", this, SLOT(hideColumn()));
    myMenu.addAction("Раскрыть столбцы", this, SLOT(uncoverColumn()));

    // Show context menu at handling position
    myMenu.exec(globalPos);
}

void MainWindow::uncoverColumn() {
    QModelIndex index = ui->listView_tables->currentIndex();
    ui->listView_tables->setSelectionMode(QAbstractItemView::ExtendedSelection);
    QString tablename = index.data(Qt::DisplayRole).toString();
    u_c->prepeare_window(tablename);
    clearTable();
    u_c->show();
}

void MainWindow::uncoverColumn1(const QString &column_name) {
    QModelIndex index = ui->listView_tables->currentIndex();
    ui->listView_tables->setSelectionMode(QAbstractItemView::ExtendedSelection);
    QString tablename = index.data(Qt::DisplayRole).toString();
    QSqlTableModel *model = new QSqlTableModel;
    model->setTable(tablename);

    int ind = model->fieldIndex(column_name);
    ui->tableView->setColumnHidden(ind, false);

    QStringList col_hid = config::user.column_hides.split(";");
    col_hid.removeLast();
    for (int i = 0; i < col_hid.count(); i++) {
        QString test_str = tablename + "," + QString::number(ind);
        if (col_hid.at(i) == test_str) {
            col_hid.removeAt(i);
        }
    }

    config::user.column_hides.clear();
    config::user.column_hides.append(col_hid.join(";"));
    model->select();
    ui->tableView->setModel(model);
    ui->tableView->show();
}

void MainWindow::showContextMenu(const QPoint &pos) {
    // Handle global position
    QPoint globalPos = ui->listView_tables->mapToGlobal(pos);

    // Create menu and insert some actions
    QMenu myMenu;
    myMenu.addAction("Создать таблицу", this, SLOT(on_create_table_triggered()));
    myMenu.addAction("Удалить таблицу", this, SLOT(deleteTable()));

    // Show context menu at handling position
    myMenu.exec(globalPos);
}

void MainWindow::hideColumn() {
    QModelIndex index = ui->listView_tables->currentIndex();
    ui->listView_tables->setSelectionMode(QAbstractItemView::ExtendedSelection);
    QString tablename = index.data(Qt::DisplayRole).toString();
    QSqlTableModel *model = new QSqlTableModel;
    model->setTable(tablename);

    int ind = ui->tableView->currentIndex().column();
    ui->tableView->hideColumn(ind);

    QStringList col_hid = config::user.column_hides.split(";");

    foreach(QString
    s, col_hid) {
        if (s.contains(tablename + "," + ind))
            col_hid.removeOne(s);
    }

    config::user.column_hides = col_hid.join(";");
    config::user.column_hides.append(tablename + "," + QString::number(ind) + ";");

    model->select();
    ui->tableView->setModel(model);
    ui->tableView->show();
}

void MainWindow::reset_tableview() {
    QModelIndex index = ui->listView_tables->currentIndex();
    ui->listView_tables->setSelectionMode(QAbstractItemView::ExtendedSelection);
    QString tablename = index.data(Qt::DisplayRole).toString();
    QSqlTableModel *model = new QSqlTableModel;
    model->setTable(tablename);
    model->select();
    ui->tableView->setModel(model);
    ui->tableView->show();
}

void MainWindow::renameColumn() {
    r_c->show();
}

void MainWindow::renameColumn1(const QString &new_column) {
    QModelIndex index = ui->listView_tables->currentIndex();
    ui->listView_tables->setSelectionMode(QAbstractItemView::ExtendedSelection);
    QString tablename = index.data(Qt::DisplayRole).toString();
    QSqlTableModel *model = new QSqlTableModel;
    model->setTable(tablename);

    QString ind = QString::number(ui->tableView->currentIndex().column());
    QStringList col_ren = config::user.column_renames.split(";");

    foreach(QString
    s, col_ren) {
        if (s.contains(tablename + "," + ind))
            col_ren.removeOne(s);
    }

    config::user.column_renames = col_ren.join(";");
    config::user.column_renames.append(tablename + "," + ind + "," + new_column + ";");

    model->setHeaderData(ui->tableView->currentIndex().column(), Qt::Horizontal, tr(new_column.toUtf8()));
    model->select();
    ui->tableView->setModel(model);
    ui->tableView->show();

}

void MainWindow::deleteTable() {

    QModelIndex index = ui->listView_tables->currentIndex();
    QString tableName = index.data(Qt::DisplayRole).toString();

    QSqlDatabase db = config::set_current_db();
    if (db.open()) {
        QSqlQuery q;
        if (q.exec("DROP TABLE " + tableName)) {
            MainWindow::prepare_window();
            QModelIndex index = ui->listView_tables->currentIndex();
            ui->listView_tables->setSelectionMode(QAbstractItemView::ExtendedSelection);
            QString tablename = index.data(Qt::DisplayRole).toString();
            QSqlTableModel *model = new QSqlTableModel;
            model->setTable(tablename);
            model->select();
            ui->tableView->setModel(model);
            ui->tableView->show();
        } else
            qDebug() << q.lastError().text();
    } else
        qDebug() << db.lastError().text();
    QSqlDatabase::removeDatabase(config::curr_database_name);
    ui->tableView->reset();
}

void MainWindow::prepare_window() {
    this->setDisabled(false);
    QSqlDatabase db = config::set_current_db();
    if (db.open()) {
        tables_list_model = new QStringListModel;
        tables_list_model->setStringList(QStringList{});
        tables_list_model->setStringList(db.tables());
        ui->listView_tables->setModel(tables_list_model);
        ui->listView_tables->show();
    } else
        qDebug() << db.lastError().text();
    QSqlDatabase::removeDatabase(config::curr_database_name);
    this->setWindowTitle("Текущий пользователь: " + config::user.username);

    ui->tabWidget->addTab(new script_window, QString("Tab %1").arg(ui->tabWidget->count()+1));
}

void MainWindow::resizeEvent(QResizeEvent *event) {

    QSize s = event->size();
    ui->centralwidget->resize(s);
}

void MainWindow::on_connect_db_triggered() {

    c_db->show();
}


void MainWindow::on_listView_tables_doubleClicked(const QModelIndex &index) {
    ui->listView_tables->setSelectionMode(QAbstractItemView::ExtendedSelection);
    QString tablename = index.data(Qt::DisplayRole).toString();

    QSqlDatabase db = config::set_current_db();
    if (db.open()) {
        QSqlTableModel *model = new QSqlTableModel;
        model->setTable(tablename);

        if (!config::user.column_renames.isEmpty()) {
            QStringList params = config::user.column_renames.split(";");
            params.removeLast();
            foreach(QString
            s, params)
            {
                QStringList params1 = s.split(",");
                QString table = params1[0];
                int index = params1[1].toInt();
                QString new_column = params1[2];
                if (table == tablename) {
                    model->setHeaderData(index, Qt::Horizontal, tr(new_column.toUtf8()));
                }
            }
        }

        model->select();

        ui->tableView->setModel(model);

        if (!config::user.column_hides.isEmpty()) {
            QStringList params = config::user.column_hides.split(";");
            params.removeLast();
            foreach(QString
            s, params)
            {
                QStringList params1 = s.split(",");
                QString table = params1[0];
                int index = params1[1].toInt();
                if (table == tablename) {
                    ui->tableView->setColumnHidden(index, true);
                }
            }
        }
        ui->tableView->show();
    }
    QSqlDatabase::removeDatabase(config::curr_database_name);
}

void MainWindow::clearTable() {
    QModelIndex index = ui->listView_tables->currentIndex();
    ui->listView_tables->setSelectionMode(QAbstractItemView::ExtendedSelection);
    QString tablename = index.data(Qt::DisplayRole).toString();
    QSqlTableModel *model = new QSqlTableModel;
    model->setTable(tablename);
    model->select();
    model->insertRow(0, index);
    ui->tableView->setModel(model);
    model->clear();
}

void MainWindow::on_create_table_triggered() {
    QSqlDatabase db = config::set_current_db();
    if (db.open()) {
        clearTable();
        create_table_window->prepare_window();
        create_table_window->show();
    } else
        QMessageBox::warning(this, "Ошибка", "Сначала подключитесь к базе данных");
    QSqlDatabase::removeDatabase(config::curr_database_name);
}

void MainWindow::on_exit_profile_triggered() {
    config::user.lastused = 0;
    config::user.username = "";
    config::user.db_driver = "";
    config::user.dir_db_sqlite = "";
    emit closedd();
    this->close();
}

void MainWindow::on_save_profile_triggered() {
    config::save_config();
    QMessageBox::information(this, "Сохранено", "Данные профиля успешно сохранены");
}

void MainWindow::on_quit_button_triggered() {
    qApp->closeAllWindows();
}

void MainWindow::on_toolButton_lightmode_clicked() {
    //lightmode 0 - темная тема; 1 - светлая

    if (config::user.lightmode == 0) {
        QFile styleF;
        styleF.setFileName(":/light.css");
        styleF.open(QFile::ReadOnly);
        QString qssStr = styleF.readAll();
        config::user.lightmode = 1;
        qApp->setStyleSheet(qssStr);
    } else {
        QFile styleF;
        styleF.setFileName(":/dark.css");
        styleF.open(QFile::ReadOnly);
        QString qssStr = styleF.readAll();
        config::user.lightmode = 0;
        qApp->setStyleSheet(qssStr);
    }
}

void MainWindow::get_column_info(QString tablename)
{
    QSqlQuery q;
    QString query_str;
    query_str = "EXEC sp_columns " + tablename;
    if (!q.exec(query_str))
        qDebug() << q.lastError().text();
    QSqlRecord rec = q.record();
    int typeCol = rec.indexOf("TYPE_NAME");
    int nameCol = rec.indexOf("COLUMN_NAME");

    while (q.next())
    {
        column_types.append(q.value(typeCol).toString());
        column_names.append(q.value(nameCol).toString());
    }

}

void MainWindow::saveFile()
{
    QTextEdit* textEdit = ui->tabWidget->currentWidget()->findChild<QTextEdit*>("textEdit");
    dir = QFileDialog::getSaveFileName(0, "Сохранить файл", dir, "*.sql *.txt");
    if (!dir.isEmpty()) {
        QFile file(dir);
        if (file.open(QIODevice::ReadWrite)) {
            QTextStream stream(&file);
            stream << textEdit->toPlainText();
            file.flush();
            file.close();
        } else {
            QMessageBox::critical(this, "Ошибка записи", "Ошибка записи файла");
        }
    }
}

void MainWindow::on_toolButton_create_clicked()
{
    ui->tabWidget->addTab(new script_window, QString("Tab %1").arg(ui->tabWidget->count()+1));
    ui->tabWidget->setCurrentIndex(ui->tabWidget->count()-1);
}

void MainWindow::on_toolButton_open_clicked()
{
    QTextEdit* textEdit = ui->tabWidget->currentWidget()->findChild<QTextEdit*>("textEdit");
    dir = QFileDialog::getOpenFileName(0, "Открыть файл", dir, "*.sql *.txt");
    if (!dir.isEmpty()) {
        QFile file(dir);
        if (file.open(QIODevice::ReadOnly)) {
            QTextStream stream(&file);
            textEdit->document()->setPlainText(stream.readAll());
            file.flush();
            file.close();
        } else {
            QMessageBox::critical(this, "Ошибка чтения", "Ошибка чтения файла");
        }
    }
}

void MainWindow::on_toolButton_save_clicked()
{
    saveFile();
}

void MainWindow::on_tabWidget_tabCloseRequested(int index)
{
    ui->tabWidget->removeTab(index);
}

void MainWindow::on_toolButton_run_clicked()
{

}
