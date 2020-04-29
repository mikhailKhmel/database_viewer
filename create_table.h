#ifndef CREATE_TABLE_H
#define CREATE_TABLE_H

#include "create_column.h"
#include <QWidget>
#include <QSqlQuery>
#include <QSqlError>
#include <QMessageBox>
#include <QDebug>
#include <QMouseEvent>

namespace Ui {
    class create_table;
}

class create_table : public QWidget {
    Q_OBJECT

public:
    explicit create_table(QWidget *parent = nullptr);

    ~create_table();

    void prepare_window();

private
    slots:
            void

    on_toolButton_clicked();

    void save_new_column(const QString &);

    void on_tablename_textChanged(const QString &arg1);

    void on_pushButton_clicked();


    void on_pushButton_2_clicked();

    signals:
            void

    closed();

private:
    Ui::create_table *ui;

    create_column *l;
    QStringList create_table_query;

    void update_query();

    void mousePressEvent(QMouseEvent *event);

    void mouseMoveEvent(QMouseEvent *event);

    int m_nMouseClick_X_Coordinate;
    int m_nMouseClick_Y_Coordinate;
};

#endif // CREATE_TABLE_H
