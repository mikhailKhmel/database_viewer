#include "connect_db.h"
#include "ui_connect_db.h"
#include "config.h"
#include <QFileDialog>
#include <QDebug>

connect_db::connect_db(QWidget *parent) :
        QDialog(parent),
        ui(new Ui::connect_db) {
    ui->setupUi(this);

    this->setWindowFlag(Qt::FramelessWindowHint);

    enable_layout(ui->comboBox_driver->currentText());
    ui->connection_result->setVisible(false);
}

connect_db::~connect_db() {
    delete ui;
}

void connect_db::set_connection_data() {
    if (ui->comboBox_driver->currentText() == "SQLITE")
        config::user.db_driver = "QSQLITE";
    else if (ui->comboBox_driver->currentText() == "MYSQL")
        config::user.db_driver = "QMYSQL";
    else if (ui->comboBox_driver->currentText() == "POSTGRESQL")
        config::user.db_driver = "QPSQL";
    else if (ui->comboBox_driver->currentText() == "MICROSOFT SQL") {
        config::user.db_driver = "QODBC3";
    }
    config::user.hostname = ui->hostname_edit->text();
    config::user.port = ui->lineEdit_port->text();
    config::user.db_username = ui->username->text();
    config::user.db_password = ui->password->text();
    config::user.dir_db_sqlite = ui->lineEdit_sqlite->text();
    config::user.databasename = ui->db->text();
}

void connect_db::on_pushButton_clicked() {
    set_connection_data();
    QSqlDatabase db = config::set_current_db();
    if (db.open()) {
        emit closed();
        this->close();
    } else
        ui->connection_result->setText("ОШИБКА");
    QSqlDatabase::removeDatabase(config::curr_database_name);

    ui->connection_result->setVisible(true);
}

void connect_db::enable_layout(QString dr) {
    if (dr == "SQLITE") {
        ui->select_file_sqlite->setVisible(true);
        ui->lineEdit_sqlite->setVisible(true);

        ui->label_4->setVisible(false);
        ui->label_2->setVisible(false);
        ui->label_3->setVisible(false);
        ui->label_5->setVisible(false);
        ui->username->setVisible(false);
        ui->password->setVisible(false);
        ui->hostname_edit->setVisible(false);
        ui->lineEdit_port->setVisible(false);
        ui->db->setVisible(false);

        QSize size(516, 144);
        this->setFixedSize(size);

        ui->pushButton_test->setFixedSize(QSize(171, 31));
        ui->connection_result->setGeometry(10, 110, 201, 16);
    } else {
        ui->select_file_sqlite->setVisible(false);
        ui->lineEdit_sqlite->setVisible(false);

        ui->label_5->setVisible(true);
        ui->label_2->setVisible(true);
        ui->label_3->setVisible(true);
        ui->label_4->setVisible(true);
        ui->username->setVisible(true);
        ui->password->setVisible(true);
        ui->hostname_edit->setVisible(true);
        ui->lineEdit_port->setVisible(true);
        ui->db->setVisible(true);

        QSize size(516, 295);
        this->setFixedSize(size);

        ui->pushButton_test->setFixedSize(QSize(171, 141));
        ui->connection_result->setGeometry(10, 270, 201, 16);
    }
}


void connect_db::on_comboBox_driver_textActivated(const QString &arg1) {
    enable_layout(arg1);
    ui->connection_result->clear();

    if (arg1 == "SQLITE")
        config::user.db_driver = "QSQLITE";
    else if (arg1 == "MYSQL")
        config::user.db_driver = "QMYSQL";
    else if (arg1 == "POSTGRESQL")
        config::user.db_driver = "QPSQL";
    else if (arg1 == "MICROSOFT SQL") {
        config::user.db_driver = "QODBC3";
        ui->connection_result->setText("Поддерживается только аутентификация Microsoft SQL Server");
    }
}


void connect_db::on_select_file_sqlite_clicked() {
    QString dir = QFileDialog::getOpenFileName(0, "Выберите базу данных", "C:\\", "*.db *.sqlite *.sqlite3 *.dll");
    ui->lineEdit_sqlite->setText(dir);
}

void connect_db::on_pushButton_test_clicked() {
    set_connection_data();
    QSqlDatabase db = config::set_current_db();
    if (db.open()) {
        ui->connection_result->setText("Подключенно!");
    } else
        ui->connection_result->setText("ОШИБКА");
    QSqlDatabase::removeDatabase(config::curr_database_name);


    ui->connection_result->setVisible(true);
}


void connect_db::on_pushButton_2_clicked() {
    this->close();
}
