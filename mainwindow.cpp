#include "mainwindow.h"
#include "ui_mainwindow.h"


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow) {
    ui->setupUi(this);

    //this->setWindowFlag(Qt::FramelessWindowHint);
    this->setAttribute(Qt::WA_DeleteOnClose);

    c_db = new connect_db;  //экзмепляр окна для соединения с бд
    create_table_window = new create_table; //экземпляр окна для создания новой таблицы
    r_c = new rename_column;    //экземпляр окна для переименования столбца
    u_c = new uncover_columns;  //экземпляр окна для раскрытия столбцов

    connect(c_db, SIGNAL(closed()), this, SLOT(listview_refresh()));    //обработка сигнала закрытия окна соединения с бд
    connect(create_table_window, SIGNAL(closed()), this, SLOT(listview_refresh())); //обработка сигнала закрытия окна создания новой таблицы

    ui->listWidget_tables->setContextMenuPolicy(Qt::CustomContextMenu);
    ui->tableWidget->setContextMenuPolicy(Qt::CustomContextMenu);

    connect(ui->listWidget_tables, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(showContextMenu(QPoint)));    //обработка сигнала открытия контекстного меню в списке таблиц
    connect(ui->tableWidget, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(showContextMenu_table(QPoint)));    //обработка сигнала открытия контекстного меню в текущей таблице
    connect(r_c, SIGNAL(closed(
                            const QString &)), this, SLOT(renameColumn1(
                                                              const QString &)));   //обработка сигнала закрытия окна переименования столбца
    connect(u_c, SIGNAL(closed(
                            const QString &)), this, SLOT(uncoverColumn1(
                                                              const QString &)));   //обработка сигнала закрытия окна для раскрытия столбцов

    ui->listWidget_tables->setViewMode(QListView::ListMode);    //режим списка

    //разбиение главное окна на элементы
    ui->splitter->setStretchFactor(0, 1);
    ui->splitter->setStretchFactor(1, 6);

    ui->splitter_2->setStretchFactor(0, 1);
    ui->splitter_2->setStretchFactor(1, 4);

    ui->splitter_3->setStretchFactor(0, 2);
    ui->splitter_3->setStretchFactor(1, 1);

    //создания вкладки пустого файла
    ui->tabWidget->addTab(new script_window, "Tab " + QString::number(count_tabs));
}

MainWindow::~MainWindow() {
    delete ui;
}

//метод заполнения QTableWidget из бд
//на вход присылается требуемая таблица для отображения и флаг, который поясняет, где хранится данная таблица: локально созданная или из базы данных
void MainWindow::show_table(QStringList curr_table, bool local) {
    clearTable();   //очистка виджета

    for (int i = 0; i < curr_table.count(); ++i) {
        QStringList elements = curr_table.at(i).split(",");
        elements.removeLast();
        if (i == 0) {   //если первая строка нулевая, значит это имена столбцов
            for (int j = 0; j < elements.count(); j++)
                ui->tableWidget->insertColumn(0);
            ui->tableWidget->setHorizontalHeaderLabels(elements);   //установка имен столбцов
        } else {    //иначе заполнение виджета
            ui->tableWidget->insertRow(0);
            for (int j = 0; j < elements.count(); ++j) {
                ui->tableWidget->setItem(0, j, new QTableWidgetItem(elements.at(j)));

            }
        }
        elements.clear();
    }
    if (local)  //если таблица локальная, дальнейшая обработка не требуется
        return;

    //Применение переименовки столбцов
    QStringList renames = config::user.column_renames.split("!");
    foreach(QString
            s, renames){
        QStringList params = s.split("?");
        int ind = run_tables_from_db.indexOf(curr_table);
        if (table_list_db.at(ind) == params.at(0)) {
            ui->tableWidget->setHorizontalHeaderItem(params.at(1).toInt(), new QTableWidgetItem(params.at(2)));
        }
    }

    //применение скрытия столбцов
    QStringList hides = config::user.column_hides.split("!");
    foreach(QString s, hides){
        QStringList params = s.split("?");
        int ind = run_tables_from_db.indexOf(curr_table);
        if (table_list_db.at(ind) == params.at(0)){
            ui->tableWidget->setColumnHidden(params.at(1).toInt(), true);
        }
    }
}

//перемещение окна
void MainWindow::mousePressEvent(QMouseEvent *event) {
    m_nMouseClick_X_Coordinate = event->x();
    m_nMouseClick_Y_Coordinate = event->y();
}

void MainWindow::mouseMoveEvent(QMouseEvent *event) {
    move(event->globalX() - m_nMouseClick_X_Coordinate, event->globalY() - m_nMouseClick_Y_Coordinate);
}

//отображение контекстного меню на виджете таблицы
void MainWindow::showContextMenu_table(const QPoint &pos) {
    QPoint globalPos = ui->tableWidget->mapToGlobal(pos);   //точка клика мышки на виджете таблицы

    // создание экзмепляра меню
    QMenu myMenu;

    //добавление разделов меню
    myMenu.addAction("Переименовать столбец", this, SLOT(renameColumn()));
    myMenu.addAction("Отменить переименование", this, SLOT(undo_renameColumn()));

    myMenu.addSeparator();

    myMenu.addAction("Скрыть столбец", this, SLOT(hideColumn()));
    myMenu.addAction("Раскрыть столбцы", this, SLOT(uncoverColumn()));


    // отобразить меню
    myMenu.exec(globalPos);
}

//метод вызова окна для раскрытия столбцов
void MainWindow::uncoverColumn() {
    //получаем имя текущей таблицы
    QModelIndex index = ui->listWidget_tables->currentIndex();
    ui->listWidget_tables->setSelectionMode(QAbstractItemView::ExtendedSelection);
    QString tablename = index.data(Qt::DisplayRole).toString();

    //подготовка и отображение окна для раскрытие столбцов
    u_c->prepeare_window(tablename);
    clearTable();
    u_c->show();
}

//метод раскрывающий столбцы
void MainWindow::uncoverColumn1(const QString &column_name) {
    //получаем имя текущей таблицы
    QModelIndex index = ui->listWidget_tables->currentIndex();
    ui->listWidget_tables->setSelectionMode(QAbstractItemView::ExtendedSelection);
    QString tablename = index.data(Qt::DisplayRole).toString();

    //создаем модель таблицы бд
    QSqlTableModel *model = new QSqlTableModel;
    model->setTable(tablename); //выбираем текущую таблицу

    int ind = model->fieldIndex(column_name);   //получаем индекс столбца в текущей таблицы

    ui->tableWidget->setColumnHidden(ind, false);   //раскрываем столбец

    //запись изменений в данные пользователя
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

//метод для отображения контекстного меню в списке таблиц
//аналогично методу showContextMenu_table
void MainWindow::showContextMenu(const QPoint &pos) {
    QPoint globalPos = ui->listWidget_tables->mapToGlobal(pos);

    QMenu myMenu;
    myMenu.addAction("Создать таблицу", this, SLOT(on_create_table_triggered()));
    myMenu.addAction("Удалить таблицу", this, SLOT(deleteTable()));

    myMenu.exec(globalPos);
}

//метод для скрытия текущего столбца
void MainWindow::hideColumn() {
    QModelIndex index = ui->listWidget_tables->currentIndex();
    ui->listWidget_tables->setSelectionMode(QAbstractItemView::ExtendedSelection);
    QString tablename = index.data(Qt::DisplayRole).toString(); //получаем имя текущей стаблицы


    int ind = ui->tableWidget->currentIndex().column();
    if (ind == -1) return;  //если индекс текущего столбца -1 (ничего не выбрано), то ничего не делать

    ui->tableWidget->hideColumn(ind);   //скрываем столбец

    //запись изменений в данные пользователя
    QStringList col_hid = config::user.column_hides.split("!");

    foreach(QString
            s, col_hid) {
        if (s.contains(tablename + "?" + ind))
            col_hid.removeOne(s);
    }

    config::user.column_hides = col_hid.join("!");
    config::user.column_hides.append(tablename + "?" + QString::number(ind) + "!");

}

//метод отображения окна переименования столбца
void MainWindow::renameColumn() {
    r_c->show();
}

//метод переименования текущего столбца
//аналогично методу hideColumn
void MainWindow::renameColumn1(const QString &new_column) {
    QModelIndex index = ui->listWidget_tables->currentIndex();
    ui->listWidget_tables->setSelectionMode(QAbstractItemView::ExtendedSelection);
    QString tablename = index.data(Qt::DisplayRole).toString();

    QString ind = QString::number(ui->tableWidget->currentIndex().column());
    QStringList col_ren = config::user.column_renames.split("!");

    foreach(QString
            s, col_ren) {
        if (s.contains(tablename + "?" + ind))
            col_ren.removeOne(s);
    }

    config::user.column_renames = col_ren.join("!");
    config::user.column_renames.append(tablename + "?" + ind + "?" + new_column + "!");

    //установка нового имени столбца по текущему индексу
    ui->tableWidget->setHorizontalHeaderItem(ind.toInt(), new QTableWidgetItem(new_column));
}

//отменение переименования текущего столбца
//аналогично методу renameColumn1
void MainWindow::undo_renameColumn()
{
    QModelIndex index = ui->listWidget_tables->currentIndex();
    ui->listWidget_tables->setSelectionMode(QAbstractItemView::ExtendedSelection);
    QString tablename = index.data(Qt::DisplayRole).toString();

    QString ind = QString::number(ui->tableWidget->currentIndex().column());
    QStringList col_ren = config::user.column_renames.split("!");

    foreach(QString
            s, col_ren) {
        if (s.contains(tablename + "?" + ind))
            col_ren.removeOne(s);
    }

    config::user.column_renames = col_ren.join("!");

    //отобразить изменение
    QStringList tbl = (table_list_db.contains(tablename)) ? run_tables_from_db.at(table_list_db.indexOf(tablename)) : run_table_local.at(table_list_local.indexOf(tablename));
    bool flag = (table_list_db.contains(tablename) ? false : true);
    show_table(tbl,flag);
}

//метод удаление текущей таблицы
void MainWindow::deleteTable() {
    //получение имени текущей таблицы
    QModelIndex index = ui->listWidget_tables->currentIndex();
    QString tableName = index.data(Qt::DisplayRole).toString();

    //если таблица хранится в базе данных
    //то через запрос удалить из бд
    if (table_list_db.contains(tableName)){

        QSqlDatabase db = config::set_current_db();
        if (db.open()) {
            if (db.tables().contains(tableName)) {
                QSqlQuery q;
                if (q.exec("DROP TABLE " + tableName)) {
                    MainWindow::prepare_window();
                } else
                    qDebug() << q.lastError().text();
            } else {
                int ind = MainWindow::table_list_db.indexOf(tableName);
                MainWindow::table_list_db.removeAt(ind);
                MainWindow::run_tables_from_db.removeAt(ind);
            }
        }
        QSqlDatabase::removeDatabase(config::curr_database_name);

        run_tables_from_db.removeAt(table_list_db.indexOf(tableName));
        table_list_db.removeOne(tableName);
    } else {    //иначе удалить информацию из оперативной памяти
        run_table_local.removeAt(table_list_local.indexOf(tableName));
        table_list_local.removeOne(tableName);
    }
    listview_refresh(); //обновить список таблиц
}

//метод подготовки окна
void MainWindow::prepare_window() {
    clearTable();   //очистка виджета таблицы
    this->setDisabled(false);
    listview_refresh(); //обновить список таблиц
    this->setWindowTitle("Текущий пользователь: " + config::user.username); //установить имя окна
}

//изменение размеров окна
void MainWindow::resizeEvent(QResizeEvent *event) {
    QSize s = event->size();
    ui->centralwidget->resize(s);
}

//отображение окна подключения к бд
void MainWindow::on_connect_db_triggered() {
    c_db->show();
}

//отобразить выбранную таблицу после двойного клика
void MainWindow::on_listWidget_tables_doubleClicked(const QModelIndex &index) {
    clearTable();   //очистить виджет таблицы
    ui->listWidget_tables->setSelectionMode(QAbstractItemView::ExtendedSelection);
    QString tablename = index.data(Qt::DisplayRole).toString(); //получение имени таблицы
    if (table_list_local.contains(tablename))
        show_table(run_table_local.at(table_list_local.indexOf(tablename)),true);
    else
        show_table(run_tables_from_db.at(table_list_db.indexOf(tablename)),false);
}

//метод очистки виджета таблицы
void MainWindow::clearTable() {
    ui->tableWidget->clear();
    ui->tableWidget->clearContents();
    ui->tableWidget->setRowCount(0);
    ui->tableWidget->setColumnCount(0);
    column_names.clear();
}

//метод для отображение окна создания новой таблицы
void MainWindow::on_create_table_triggered() {
    QSqlDatabase db = config::set_current_db(); //установить соединение
    if (db.open()) {
        clearTable();   //очистить виджет таблицы
        create_table_window->prepare_window();  //подготовить окно
        create_table_window->show();    //отобразить окно
    } else
        QMessageBox::warning(this, "Ошибка", "Сначала подключитесь к базе данных"); //иначе ошибка подключения
    QSqlDatabase::removeDatabase(config::curr_database_name);
}

//метод выхода из профиля
void MainWindow::on_exit_profile_triggered() {
    //очистить все данные текущего пользователя
    config::user.username = "";
    config::user.db_driver = "";
    config::user.port = "";
    config::user.hostname = "";
    config::user.lightmode = 0;
    config::user.db_password = "";
    config::user.db_username = "";
    config::user.column_hides = "";
    config::user.column_renames = "";
    config::user.databasename = "";
    config::user.dir_db_sqlite = "";

    emit closedd(); //вызов сигнала закрытия окна
    this->close();  //закрыть окно
}

//сохранить изменения текущего пользователя
void MainWindow::on_save_profile_triggered() {
    config::save_config();
    QMessageBox::information(this, "Сохранено", "Данные профиля успешно сохранены");
}

//закрыть программу
void MainWindow::on_quit_button_triggered() {
    qApp->closeAllWindows();
}

//применение изменения стилей
void MainWindow::on_toolButton_lightmode_clicked() {
    //lightmode 0 - темная тема; 1 - светлая


    if (config::user.lightmode == 0) {  //если темная тема
        QFile styleF;
        styleF.setFileName(":/light.css");  //открыть файл стилей
        styleF.open(QFile::ReadOnly);
        QString qssStr = styleF.readAll();  //прочитать всё в строку
        config::user.lightmode = 1; //изменить параметр
        qApp->setStyleSheet(qssStr);    //применить нвоый стиль
        ui->tabWidget->currentWidget()->setStyleSheet(qssStr);  //применить стиль для текущей вкладки
        for (int i = 0; i < ui->tabWidget->count(); ++i)    //применение стиля для всех вкладок
        {
            ui->tabWidget->widget(i)->setStyleSheet(qssStr);
            QTextEdit *textEdit_rows = ui->tabWidget->widget(i)->findChild<QTextEdit *>("textEdit_rows");
            textEdit_rows->setStyleSheet("QTextEdit: {background: #404040;}");

        }

    } else {    //если светлая
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

//сохранить текущий скрипт
void MainWindow::saveFile() {
    QTextEdit *textEdit = ui->tabWidget->currentWidget()->findChild<QTextEdit *>("textEdit");   //подключение к текстовуму полю текущего виджета
    dir = QFileDialog::getSaveFileName(0, "Сохранить файл", dir, "*.sql *.txt");    //открыть окно выбора пути
    if (!dir.isEmpty()) {
        QFile file(dir);
        if (file.open(QIODevice::ReadWrite)) {
            QTextStream stream(&file);
            stream << textEdit->toPlainText();  //запись в файл
            file.flush();
            file.close();
        } else {
            QMessageBox::critical(this, "Ошибка записи", "Ошибка записи файла");
        }
    }
}

//создать новый скрипт
void MainWindow::on_toolButton_create_clicked() {
    ui->tabWidget->addTab(new script_window, QString("Tab %1").arg(count_tabs));    //добавить новую вкладку
    ui->tabWidget->setCurrentIndex(ui->tabWidget->count() - 1); //установить новую вкладку текущей
    count_tabs++;
}

//открыть скрипт
//аналогично методу saveFile
void MainWindow::on_toolButton_open_clicked() {
    QTextEdit *textEdit = ui->tabWidget->currentWidget()->findChild<QTextEdit *>("textEdit");
    dir = QFileDialog::getOpenFileName(0, "Открыть файл", dir, "*.sql *.txt");  //выбрать файл для чтения
    if (!dir.isEmpty()) {
        QFile file(dir);
        if (file.open(QIODevice::ReadOnly)) {
            QTextStream stream(&file);
            textEdit->document()->setPlainText(stream.readAll());   //чтение файла и запись в текстового поля текущей вкладки
            file.flush();
            file.close();
        } else {
            QMessageBox::critical(this, "Ошибка чтения", "Ошибка чтения файла");
        }
        QString filename = dir.right(dir.count() - dir.lastIndexOf("/") - 1);   //установка имени файла в имя вкладки
        ui->tabWidget->setTabText(ui->tabWidget->currentIndex(), filename);
    }
}

//вызов метода сохранения файла
void MainWindow::on_toolButton_save_clicked() {
    saveFile();
}

//закрыть вкладку
void MainWindow::on_tabWidget_tabCloseRequested(int index) {
    QTextEdit *textEdit = ui->tabWidget->currentWidget()->findChild<QTextEdit *>("textEdit");
    if (!textEdit->document()->isEmpty()) { //если текстовое поле не пустое, предложить сохранить изменения
        QMessageBox::StandardButton reply;
        reply = QMessageBox::question(this, "Сохранение", "Сохранить изменения?",
                                      QMessageBox::Yes | QMessageBox::No);
        if (reply == QMessageBox::Yes) {
            saveFile();
        }
    }
    ui->tabWidget->removeTab(index);    //удалить вкладку
}

//запуск скрипта
void MainWindow::on_toolButton_run_clicked() {
    QSqlDatabase db = config::set_current_db(); //установка соединения
    if (db.open()) {
        QTextEdit *textEdit = ui->tabWidget->currentWidget()->findChild<QTextEdit *>("textEdit");
        QString query_str = textEdit->document()->toPlainText();    //считываем текст в текстового поля текущей вкладки
        query_str.remove("\n"); //удаляем все переносы строк
        QStringList query_list = query_str.split(";");  //собираем список запросов по сепаратору ;
        query_list.removeLast();
        QSqlQuery q;
        QStringList errors_list;    //список ошибок
        int curr_line = 1;  //счетчик строки
        foreach(QString
                s, query_list){ //для каждого запроса
            q.clear();  //очистка запроса
            if (s.startsWith("select", Qt::CaseInsensitive)) {  //если запросы начинается с select (любого регистра)
                if (q.exec(s)) {    //выполнить запрос
                    append_table(q,true);   //добавить новую локальную таблицу
                    MainWindow::table_list_local.append("table_" + QString::number(table_list_local.count()+1));    //сохранить название локальной таблицы
                } else {
                    errors_list.append("Line " + QString::number(curr_line) + ": " + q.lastError().text()); //иначе записать в ошибки
                }
            } else {    //если не select
                if (!q.exec(s)) //выполнить запрос. при неудаче, записать в ошибки
                    errors_list.append("Line " + QString::number(curr_line) + ": " + q.lastError().text());
            }
            curr_line++;
        }
        if (!errors_list.isEmpty()) {   //если список ошибок не пустой
            ui->tabWidget->addTab(new script_window, "ERRORS"); //создать новую вкладку и вывести список ошибок
            ui->tabWidget->setCurrentIndex(ui->tabWidget->count() - 1);
            QTextEdit *textEdit1 = ui->tabWidget->currentWidget()->findChild<QTextEdit *>("textEdit");
            textEdit1->document()->setPlainText(errors_list.join("\n"));
        }
    }
    db.commit();    //подтвердить изменения в бд

    listview_refresh(); //обновить список таблиц
}

//добавить новую таблицу из бд в оперативную память
void MainWindow::append_table(QSqlQuery q, bool local) {
    QStringList curr_table_query;
    QString tmp;
    for (int i = 0; i < q.record().count(); ++i) {
        tmp.append(q.record().fieldName(i) + ",");  //в нулевую строку заполнить именя столбцов
    }
    curr_table_query.append(tmp);
    while (q.next()) {  //пока не закончится результат запроса, заполнять временную строку
        tmp.clear();
        for (int i = 0; i < q.record().count(); ++i) {
            if (q.value(i).isNull()){ //если содержимое запроса отсутствует
                tmp.append("NULL,");
            } else {
                tmp.append(q.value(i).toString() + ",");

            }
        }
        curr_table_query.append(tmp); //добавить в список строк таблицы
    }
    if (local)  //сохранить в определенный список
        run_table_local.append(curr_table_query);
    else
        run_tables_from_db.append(curr_table_query);
}

//обновить список таблиц
void MainWindow::listview_refresh() {
    table_list_db.clear(); //очистить данные о таблиц из бд
    run_tables_from_db.clear();
    ui->listWidget_tables->clear();

    QSqlDatabase db = config::set_current_db(); //установка соединения
    if (db.open()) {
        QStringList tables = db.tables();
        foreach(QString
                curr_table, tables){    //выгрузить таблицы из бд в оперативную память
            QSqlQuery q;
            if (q.exec(QString("SELECT * FROM %1").arg(curr_table))) {
                append_table(q,false);
                table_list_db.append(curr_table);
                ui->listWidget_tables->addItem(curr_table); //отобразить имя таблицы в списке
            } else {}
            q.clear();
        }

    }
    QSqlDatabase::removeDatabase(config::curr_database_name);

    ui->listWidget_tables->addItems(table_list_local);  //добавить имена локальных таблиц в список
}

//метод поиска таблиц
//срабатывает при наборе текста
void MainWindow::on_lineEdit_search_textChanged(const QString &arg1)
{
    //поиск имени таблицы в списка локальных таблиц и из бд
    ui->listWidget_tables->clear();
    for (int i = 0; i < table_list_db.count(); ++i) {
        if (table_list_db.at(i).contains(arg1, Qt::CaseInsensitive))
            ui->listWidget_tables->addItem(table_list_db.at(i));    //добавить в список, если находит
    }
    for (int i = 0; i < table_list_local.count(); ++i) {
        if (table_list_local.at(i).contains(arg1, Qt::CaseInsensitive))
            ui->listWidget_tables->addItem(table_list_local.at(i));
    }
}

//вызов метод обновления списка
void MainWindow::on_toolButton_refresh_clicked()
{
    listview_refresh();
}

//вызов метода выхода из профиля
void MainWindow::on_toolButton_exit_clicked()
{
    on_exit_profile_triggered();
}
