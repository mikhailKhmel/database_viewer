#ifndef CONNECT_DB_H
#define CONNECT_DB_H

#include <QDialog>
#include <QSql>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>

namespace Ui {
class connect_db;
}

class connect_db : public QDialog
{
    Q_OBJECT

public:
    explicit connect_db(QWidget *parent = nullptr);
    ~connect_db();

private slots:
    void on_pushButton_clicked();
    void on_comboBox_driver_textActivated(const QString &arg1);

    void on_select_file_sqlite_clicked();

    void on_pushButton_test_clicked();

private:
    Ui::connect_db *ui;
    void enable_layout(QString dr);
    void connect_to_sqlite();
    void connect_to_mysql_or_postgre();
    void connect_to_mssql();
    QString return_qdriver(QString driver);
};

#endif // CONNECT_DB_H
