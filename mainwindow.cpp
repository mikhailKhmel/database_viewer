#include "mainwindow.h"
#include "ui_mainwindow.h"


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow) {
    ui->setupUi(this);

    //this->setWindowFlag(Qt::FramelessWindowHint);
    this->setAttribute(Qt::WA_DeleteOnClose);

    c_db = new connect_db;
    create_table_window = new create_table;
    r_c = new rename_column;
    u_c = new uncover_columns;

    connect(c_db, SIGNAL(closed()), this, SLOT(listview_refresh()));
    connect(create_table_window, SIGNAL(closed()), this, SLOT(listview_refresh()));

    ui->listWidget_tables->setContextMenuPolicy(Qt::CustomContextMenu);
    ui->tableWidget->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(ui->listWidget_tables, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(showContextMenu(QPoint)));
    connect(ui->tableWidget, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(showContextMenu_table(QPoint)));
    connect(r_c, SIGNAL(closed(
                            const QString &)), this, SLOT(renameColumn1(
                                                              const QString &)));
    connect(u_c, SIGNAL(closed(
                            const QString &)), this, SLOT(uncoverColumn1(
                                                              const QString &)));
    /*перенос функций редактора скрипта в основное окно*/
    ui->listWidget_tables->setViewMode(QListView::ListMode);
    ui->splitter->setStretchFactor(0, 1);
    ui->splitter->setStretchFactor(1, 4);

    ui->splitter_2->setStretchFactor(0, 2);
    ui->splitter_2->setStretchFactor(1, 1);


    ui->tabWidget->addTab(new script_window, "Tab_1");
}

MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::show_table(int index) {
    clearTable();

    QStringList curr_table = run_tables.at(index);

    for (int i = 0; i < curr_table.count(); ++i) {
        QStringList elements = curr_table.at(i).split(",");
        elements.removeLast();
        if (i == 0) {
            for (int j = 0; j < elements.count(); j++)
                ui->tableWidget->insertColumn(0);
            ui->tableWidget->setHorizontalHeaderLabels(elements);
        } else {
            ui->tableWidget->insertRow(0);
            for (int j = 0; j < elements.count(); ++j) {
                ui->tableWidget->setItem(0, j, new QTableWidgetItem(elements.at(j)));

            }
        }
        elements.clear();
    }

    qDebug() << config::user.column_renames;
    QStringList renames = config::user.column_renames.split(";");
    foreach(QString
            s, renames){
        QStringList params = s.split(",");
        if (table_list.at(index) == params.at(0)) {
            ui->tableWidget->setHorizontalHeaderItem(params.at(1).toInt(), new QTableWidgetItem(params.at(2)));
        }
    }

    qDebug() << config::user.column_hides;
}

void MainWindow::mousePressEvent(QMouseEvent *event) {
    m_nMouseClick_X_Coordinate = event->x();
    m_nMouseClick_Y_Coordinate = event->y();
}

void MainWindow::mouseMoveEvent(QMouseEvent *event) {
    move(event->globalX() - m_nMouseClick_X_Coordinate, event->globalY() - m_nMouseClick_Y_Coordinate);
}

void MainWindow::showContextMenu_table(const QPoint &pos) {
    QPoint globalPos = ui->tableWidget->mapToGlobal(pos);

    // Create menu and insert some actions
    QMenu myMenu;

    myMenu.addAction("Переименовать столбец", this, SLOT(renameColumn()));
    myMenu.addAction("Скрыть столбец", this, SLOT(hideColumn()));
    myMenu.addAction("Раскрыть столбцы", this, SLOT(uncoverColumn()));

    // Show context menu at handling position
    myMenu.exec(globalPos);
}

void MainWindow::uncoverColumn() {
    QModelIndex index = ui->listWidget_tables->currentIndex();
    ui->listWidget_tables->setSelectionMode(QAbstractItemView::ExtendedSelection);
    QString tablename = index.data(Qt::DisplayRole).toString();
    u_c->prepeare_window(tablename);
    clearTable();
    u_c->show();
}

void MainWindow::uncoverColumn1(const QString &column_name) {
    QModelIndex index = ui->listWidget_tables->currentIndex();
    ui->listWidget_tables->setSelectionMode(QAbstractItemView::ExtendedSelection);
    QString tablename = index.data(Qt::DisplayRole).toString();
    QSqlTableModel *model = new QSqlTableModel;
    model->setTable(tablename);

    int ind = model->fieldIndex(column_name);

    ui->tableWidget->setColumnHidden(ind, false);

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

}

void MainWindow::showContextMenu(const QPoint &pos) {
    // Handle global position
    QPoint globalPos = ui->listWidget_tables->mapToGlobal(pos);

    // Create menu and insert some actions
    QMenu myMenu;
    myMenu.addAction("Создать таблицу", this, SLOT(on_create_table_triggered()));
    myMenu.addAction("Удалить таблицу", this, SLOT(deleteTable()));

    // Show context menu at handling position
    myMenu.exec(globalPos);
}

void MainWindow::hideColumn() {
    QModelIndex index = ui->listWidget_tables->currentIndex();
    ui->listWidget_tables->setSelectionMode(QAbstractItemView::ExtendedSelection);
    QString tablename = index.data(Qt::DisplayRole).toString();


    int ind = ui->tableWidget->currentIndex().column();
    ui->tableWidget->hideColumn(ind);

    QStringList col_hid = config::user.column_hides.split(";");

    foreach(QString
            s, col_hid) {
        if (s.contains(tablename + "," + ind))
            col_hid.removeOne(s);
    }

    config::user.column_hides = col_hid.join(";");
    config::user.column_hides.append(tablename + "," + QString::number(ind) + ";");

}

void MainWindow::renameColumn() {
    r_c->show();
}

void MainWindow::renameColumn1(const QString &new_column) {
    QModelIndex index = ui->listWidget_tables->currentIndex();
    ui->listWidget_tables->setSelectionMode(QAbstractItemView::ExtendedSelection);
    QString tablename = index.data(Qt::DisplayRole).toString();

    QString ind = QString::number(ui->tableWidget->currentIndex().column());
    QStringList col_ren = config::user.column_renames.split(";");

    foreach(QString
            s, col_ren) {
        if (s.contains(tablename + "," + ind))
            col_ren.removeOne(s);
    }

    config::user.column_renames = col_ren.join(";");
    config::user.column_renames.append(tablename + "," + ind + "," + new_column + ";");

    ui->tableWidget->setHorizontalHeaderItem(ind.toInt(), new QTableWidgetItem(new_column));
}

void MainWindow::deleteTable() {

    QModelIndex index = ui->listWidget_tables->currentIndex();
    QString tableName = index.data(Qt::DisplayRole).toString();

    QSqlDatabase db = config::set_current_db();
    if (db.open()) {
        if (db.tables().contains(tableName)) {
            QSqlQuery q;
            if (q.exec("DROP TABLE " + tableName)) {
                MainWindow::prepare_window();
            } else
                qDebug() << q.lastError().text();
        } else {
            int ind = MainWindow::table_list.indexOf(tableName);
            MainWindow::table_list.removeAt(ind);
            MainWindow::run_tables.removeAt(ind);
        }
    }
    QSqlDatabase::removeDatabase(config::curr_database_name);
    listview_refresh();
}

void MainWindow::prepare_window() {
    clearTable();
    this->setDisabled(false);
    QSqlDatabase db = config::set_current_db();
    if (db.open()) {
        foreach(QString
                curr_table, db.tables()){
            QSqlQuery q;
            if (q.exec(QString("SELECT * FROM %1").arg(curr_table))) {
                append_table(q);
            }
        }
        table_list.append(db.tables());
        ui->listWidget_tables->addItems(table_list);
    } else
        qDebug() << db.lastError().text();
    QSqlDatabase::removeDatabase(config::curr_database_name);
    this->setWindowTitle("Текущий пользователь: " + config::user.username);
}

void MainWindow::resizeEvent(QResizeEvent *event) {

    QSize s = event->size();
    ui->centralwidget->resize(s);
}

void MainWindow::on_connect_db_triggered() {

    c_db->show();
}


void MainWindow::on_listWidget_tables_doubleClicked(const QModelIndex &index) {
    clearTable();
    ui->listWidget_tables->setSelectionMode(QAbstractItemView::ExtendedSelection);
    QString tablename = index.data(Qt::DisplayRole).toString();
    show_table(index.row());
}

void MainWindow::clearTable() {
    ui->tableWidget->clear();
    ui->tableWidget->clearContents();
    ui->tableWidget->setRowCount(0);
    column_names.clear();
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
        ui->tabWidget->currentWidget()->setStyleSheet(qssStr);
        for (int i = 0; i < ui->tabWidget->count(); ++i)
        {
            ui->tabWidget->widget(i)->setStyleSheet(qssStr);
            QTextEdit *textEdit_rows = ui->tabWidget->widget(i)->findChild<QTextEdit *>("textEdit_rows");
            textEdit_rows->setStyleSheet("QTextEdit: {background: #404040;}");

        }

    } else {
        QFile styleF;
        styleF.setFileName(":/dark.css");
        styleF.open(QFile::ReadOnly);
        QString qssStr = styleF.readAll();
        config::user.lightmode = 0;
        qApp->setStyleSheet(qssStr);
        ui->tabWidget->currentWidget()->setStyleSheet(qssStr);
        for (int i = 0; i < ui->tabWidget->count(); ++i)
        {
            ui->tabWidget->widget(i)->setStyleSheet(qssStr);
            QTextEdit *textEdit_rows = ui->tabWidget->widget(i)->findChild<QTextEdit *>("textEdit_rows");
            textEdit_rows->setStyleSheet("QTextEdit: {background: white;}");
        }
    }
}

void MainWindow::saveFile() {
    QTextEdit *textEdit = ui->tabWidget->currentWidget()->findChild<QTextEdit *>("textEdit");
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

void MainWindow::on_toolButton_create_clicked() {
    ui->tabWidget->addTab(new script_window, QString("Tab %1").arg(count_tabs));
    ui->tabWidget->setCurrentIndex(ui->tabWidget->count() - 1);
    count_tabs++:
}

void MainWindow::on_toolButton_open_clicked() {
    QTextEdit *textEdit = ui->tabWidget->currentWidget()->findChild<QTextEdit *>("textEdit");
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
        QString filename = dir.right(dir.count() - dir.lastIndexOf("/") - 1);
        ui->tabWidget->setTabText(ui->tabWidget->currentIndex(), filename);
    }
}

void MainWindow::on_toolButton_save_clicked() {
    saveFile();
}

void MainWindow::on_tabWidget_tabCloseRequested(int index) {
    QTextEdit *textEdit = ui->tabWidget->currentWidget()->findChild<QTextEdit *>("textEdit");
    if (!textEdit->document()->isEmpty()) {
        QMessageBox::StandardButton reply;
        reply = QMessageBox::question(this, "Сохранение", "Сохранить изменения?",
                                      QMessageBox::Yes | QMessageBox::No);
        if (reply == QMessageBox::Yes) {
            saveFile();
        }
    }
    ui->tabWidget->removeTab(index);
}

void MainWindow::on_toolButton_run_clicked() {
    QSqlDatabase db = config::set_current_db();
    if (db.open()) {
        QTextEdit *textEdit = ui->tabWidget->currentWidget()->findChild<QTextEdit *>("textEdit");
        QString query_str = textEdit->document()->toPlainText();
        query_str.remove("\n");
        QStringList query_list = query_str.split(";");
        query_list.removeLast();
        QSqlQuery q;
        QStringList errors_list;
        int curr_line = 1;
        foreach(QString
                s, query_list){
            q.clear();
            if (s.startsWith("select", Qt::CaseInsensitive)) {
                if (q.exec(s)) {
                    append_table(q);
                    MainWindow::table_list.append("table_" + QString::number(table_list.count()));
                } else {
                    errors_list.append("Line " + QString::number(curr_line) + ": " + q.lastError().text());
                }
            } else {
                if (!q.exec(s))
                    errors_list.append("Line " + QString::number(curr_line) + ": " + q.lastError().text());
            }
            curr_line++;
        }
        if (!errors_list.isEmpty()) {
            ui->tabWidget->addTab(new script_window, "ERRORS");
            ui->tabWidget->setCurrentIndex(ui->tabWidget->count() - 1);
            QTextEdit *textEdit1 = ui->tabWidget->currentWidget()->findChild<QTextEdit *>("textEdit");
            textEdit1->document()->setPlainText(errors_list.join("\n"));
        }
    }
    listview_refresh();
}

void MainWindow::append_table(QSqlQuery q) {
    QStringList curr_table_query;
    QString tmp;
    for (int i = 0; i < q.record().count(); ++i) {
        tmp.append(q.record().fieldName(i) + ",");
    }
    curr_table_query.append(tmp);
    while (q.next()) {
        tmp.clear();
        for (int i = 0; i < q.record().count(); ++i) {
            tmp.append(q.value(i).toString() + ",");
        }
        curr_table_query.append(tmp);
    }

    run_tables.append(curr_table_query);
}

void MainWindow::listview_refresh() {
    table_list.clear();
    ui->listWidget_tables->clear();
    QSqlDatabase db = config::set_current_db();
    if (db.open()) {
        foreach(QString
                curr_table, db.tables()){
            QSqlQuery q;
            if (q.exec(QString("SELECT * FROM %1").arg(curr_table))) {
                append_table(q);
            }
        }
        table_list.append(db.tables());
        ui->listWidget_tables->addItems(table_list);
    }
    QSqlDatabase::removeDatabase(config::curr_database_name);

}
