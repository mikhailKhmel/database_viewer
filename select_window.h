#ifndef SELECT_WINDOW_H
#define SELECT_WINDOW_H

#include "config.h"
#include <QWidget>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QStringListModel>


namespace Ui {
    class select_window;
}

class select_window : public QWidget {
    Q_OBJECT

public:
    explicit select_window(QWidget *parent = nullptr);

    ~select_window();

    void prepareWindow(QString tablename);


private
    slots:
            void

    on_pushButton_cancel_clicked();

    void on_pushButton_select_clicked();

    void on_comboBox_select_rovno_activated(const QString &arg1);

    signals:
            void

    closed(const QString &query);

private:
    Ui::select_window *ui;

    QString curr_table;
};

#endif // SELECT_WINDOW_H
