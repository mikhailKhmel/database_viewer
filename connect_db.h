#ifndef CONNECT_DB_H
#define CONNECT_DB_H

#include <QDialog>
#include <QSql>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QMouseEvent>

namespace Ui {
    class connect_db;
}

class connect_db : public QDialog {
    Q_OBJECT

public:
    explicit connect_db(QWidget *parent = nullptr);

    ~connect_db();

private
    slots:
            void

    on_pushButton_clicked();

    void on_select_file_sqlite_clicked();

    void on_pushButton_test_clicked();

    void on_pushButton_2_clicked();


    signals:
            void

    closed();

private:
    Ui::connect_db *ui;

    void enable_layout(QString dr);

    void set_connection_data();

};

#endif // CONNECT_DB_H
