#ifndef UNCOVER_COLUMNS_H
#define UNCOVER_COLUMNS_H

#include "config.h"
#include <QSqlDatabase>
#include <QStringListModel>
#include <QWidget>

namespace Ui {
    class uncover_columns;
}

class uncover_columns : public QWidget {
    Q_OBJECT

public:
    explicit uncover_columns(QWidget *parent = 0);

    ~uncover_columns();

    void prepeare_window(QString tablename);

private
    slots:
            void

    on_pushButton_back_clicked();

    void on_pushButton_continue_clicked();

    signals:
            void
    closed(QString
    col_name);

private:
    Ui::uncover_columns *ui;

    QStringListModel *tables_list_model;
};

#endif // UNCOVER_COLUMNS_H
