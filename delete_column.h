#ifndef DELETE_COLUMN_H
#define DELETE_COLUMN_H

#include <QWidget>
#include <QSqlQuery>
#include <QSqlDatabase>
#include <QDebug>
#include <QMouseEvent>
#include "config.h"

namespace Ui {
    class delete_column;
}

class delete_column : public QWidget {
    Q_OBJECT

public:
    explicit delete_column(QWidget *parent = nullptr);

    ~delete_column();

    void prepare_window(QStringList fields, QString current_table);

    signals:
            void

    closed();

private
    slots:
            void

    on_pushButton_back_clicked();

    void on_pushButton_submit_clicked();


private:
    Ui::delete_column *ui;

    QString current_table;
    void mousePressEvent(QMouseEvent *event);
        void mouseMoveEvent(QMouseEvent *event);
        int m_nMouseClick_X_Coordinate;
        int m_nMouseClick_Y_Coordinate;
};

#endif // DELETE_COLUMN_H
