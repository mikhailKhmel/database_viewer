#ifndef DELETE_COLUMN_H
#define DELETE_COLUMN_H

#include <QWidget>
#include <QSqlQuery>
#include <QSqlDatabase>
#include <QDebug>
#include "config.h"

namespace Ui {
class delete_column;
}

class delete_column : public QWidget
{
    Q_OBJECT

public:
    explicit delete_column(QWidget *parent = nullptr);
    ~delete_column();
    void prepare_window(QStringList fields, QString current_table);
signals:
    void closed();
private slots:
    void on_pushButton_back_clicked();

    void on_pushButton_submit_clicked();

private:
    Ui::delete_column *ui;

    QString current_table;
};

#endif // DELETE_COLUMN_H
