#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "config.h"
#include "connect_db.h"
#include "create_table.h"
#include "delete_column.h"
#include "create_column.h"
#include <QDebug>
#include <QStringListModel>
#include <QAbstractItemView>
#include <QMainWindow>
#include <QResizeEvent>
#include <QMessageBox>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

public slots:
    void prepare_window();

protected:
    void resizeEvent(QResizeEvent* event);


private slots:
    void on_connect_db_triggered();

    void on_listView_tables_doubleClicked(const QModelIndex &index);

    void on_create_table_triggered();


   void on_tableView_doubleClicked(const QModelIndex &index);

   void on_exit_profile_triggered();

   void on_save_profile_triggered();


   void showContextMenu(const QPoint &pos);
   void showContextMenu_table(const QPoint &pos);

   void reset_tableview();

   void addRow();
   void deleteRow();
   void addColumn();
   void deleteColumn();
   void addColumn1(QString column_command);

   void deleteTable();

   void test_change();
   signals:
       void closedd();

private:
    Ui::MainWindow *ui;
    connect_db* c_db;
    create_table* create_table_window;
    delete_column* d_c;
    create_column* c_c;

    QStringListModel* tables_list_model;
};

#endif // MAINWINDOW_H
