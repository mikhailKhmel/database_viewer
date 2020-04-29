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

    void on_comboBox_driver_textActivated(const QString &arg1);

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
    void mousePressEvent(QMouseEvent *event);
        void mouseMoveEvent(QMouseEvent *event);
        int m_nMouseClick_X_Coordinate;
        int m_nMouseClick_Y_Coordinate;
};

#endif // CONNECT_DB_H
