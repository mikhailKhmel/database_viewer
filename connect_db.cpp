#include "connect_db.h"
#include "ui_connect_db.h"
#include "config.h"
#include <QFileDialog>
#include <QDebug>

connect_db::connect_db(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::connect_db)
{
    ui->setupUi(this);
    enable_layout(ui->comboBox_driver->currentText());
    ui->connection_result->setVisible(false);
}

connect_db::~connect_db()
{
    delete ui;
}


void connect_db::on_pushButton_clicked()
{
    QString driver = return_qdriver(ui->comboBox_driver->currentText());

    if (QFile::exists(ui->lineEdit_sqlite->text()))
    {
        if (driver == "QSQLITE")
        {
            QSqlDatabase db = QSqlDatabase::addDatabase(driver);
            db.setDatabaseName(ui->lineEdit_sqlite->text());
            if (db.open())
            {
                config::work_db = db;
                config::user.dir_db_sqlite = ui->lineEdit_sqlite->text();
                config::user.db_driver = driver;
                emit closed();
                this->close();
            }
            else
                ui->connection_result->setText("ОШИБКА");
        }
        else
        {
            QSqlDatabase db = QSqlDatabase::addDatabase(driver);
            db.setHostName(ui->hostname_edit->text());
            db.setDatabaseName(ui->db->text());
            db.setUserName(ui->username->text());
            db.setPassword(ui->password->text());
            if (db.open())
            {
                config::work_db = db;
                config::user.hostname = ui->hostname_edit->text();
                config::user.databasename = ui->db->text();
                config::user.db_username = ui->username->text();
                config::user.db_password = ui->password->text();
                config::user.db_driver = driver;
                emit closed();
                this->close();
            }
            else
                ui->connection_result->setText("ОШИБКА");
        }
    }
    else
        ui->connection_result->setText("ОШИБКА");


    ui->connection_result->setVisible(true);
}

void connect_db::enable_layout(QString dr)
{
    if (dr == "SQLITE")
    {
        ui->select_file_sqlite->setVisible(true);
        ui->lineEdit_sqlite->setVisible(true);

        ui->label_4->setVisible(false);
        ui->label_2->setVisible(false);
        ui->label_3->setVisible(false);
        ui->label_5->setVisible(false);
        ui->username->setVisible(false);
        ui->password->setVisible(false);
        ui->hostname_edit->setVisible(false);
        ui->db->setVisible(false);

        QSize size(516,144);
        this->setFixedSize(size);

        ui->pushButton_test->setFixedSize(QSize(171,31));
        ui->connection_result->setGeometry(10,110,201,16);
    }
    else
    {
        ui->select_file_sqlite->setVisible(false);
        ui->lineEdit_sqlite->setVisible(false);

        ui->label_5->setVisible(true);
        ui->label_2->setVisible(true);
        ui->label_3->setVisible(true);
        ui->label_4->setVisible(true);
        ui->username->setVisible(true);
        ui->password->setVisible(true);
        ui->hostname_edit->setVisible(true);
        ui->db->setVisible(true);

        QSize size(516,295);
        this->setFixedSize(size);

        ui->pushButton_test->setFixedSize(QSize(171,141));
        ui->connection_result->setGeometry(10,270,201,16);
    }
}


void connect_db::on_comboBox_driver_textActivated(const QString &arg1)
{
    enable_layout(arg1);

    if (arg1 == "SQLITE")
        config::set_db_driver("QSQLITE");
    else if (arg1 == "MYSQL")
        config::set_db_driver("QMYSQL");
    else if (arg1 == "POSTGRESQL")
        config::set_db_driver("QPSQL");
    else if (arg1 == "MICROSOFT SQL")
        config::set_db_driver("QODBC");
}

QString connect_db::return_qdriver(QString driver)
{
    if (driver == "SQLITE")
        return QString("QSQLITE");
        //config::set_db_driver("QSQLITE");
    else if (driver == "MYSQL")
        return QString("QMYSQL");
        //config::set_db_driver("QMYSQL");
    else if (driver == "POSTGRESQL")
        return QString("QPSQL");
        //config::set_db_driver("QPSQL");
    else if (driver == "MICROSOFT SQL")
        return QString("QODBC");
        //config::set_db_driver("QODBC");
}

void connect_db::on_select_file_sqlite_clicked()
{
    QString dir = QFileDialog::getOpenFileName(0, "Выберите базу данных", "C:\\", "*.db *.sqlite *.sqlite3 *.dll");
    qDebug() << dir;

    //config::set_dir_db_sqlite(dir);

    ui->lineEdit_sqlite->setText(dir);
}

void connect_db::on_pushButton_test_clicked()
{
    QString driver = return_qdriver(ui->comboBox_driver->currentText());

    if (QFile::exists(ui->lineEdit_sqlite->text()))
    {
        if (driver == "QSQLITE")
        {
            QSqlDatabase db = QSqlDatabase::addDatabase(driver);
            db.setDatabaseName(ui->lineEdit_sqlite->text());
            if (db.open())
                ui->connection_result->setText("Подключено!");
            else
                ui->connection_result->setText("ОШИБКА");
        }
        else
        {

        }
    }
    else
        ui->connection_result->setText("ОШИБКА");


    ui->connection_result->setVisible(true);
}

