#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "config.h"
#include "connect_db.h"
#include "create_table.h"
#include "create_column.h"
#include "rename_column.h"
#include "uncover_columns.h"
#include "script_window.h"

#include <QDebug>
#include <QStringListModel>
#include <QAbstractItemView>
#include <QMainWindow>
#include <QResizeEvent>
#include <QMessageBox>
#include <QApplication>
#include <QMouseEvent>
#include <QTextEdit>
#include <QFileDialog>

namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);

    ~MainWindow();

public
    slots:
            void

    prepare_window();

protected:
    void resizeEvent(QResizeEvent *event);


private
    slots:
            void

    on_connect_db_triggered();

    void on_listWidget_tables_doubleClicked(const QModelIndex &index);

    void on_exit_profile_triggered();

    void on_save_profile_triggered();

    void showContextMenu(const QPoint &pos);

    void showContextMenu_table(const QPoint &pos);

    void renameColumn();

    void renameColumn1(const QString &new_column);

    void undo_renameColumn();

    void hideColumn();

    void deleteTable();

    void uncoverColumn();

    void uncoverColumn1(const QString &column_name);

    void on_quit_button_triggered();

    void on_toolButton_lightmode_clicked();

    void on_toolButton_create_clicked();

    void on_toolButton_open_clicked();

    void on_toolButton_save_clicked();

    void on_toolButton_run_clicked();

    void on_tabWidget_tabCloseRequested(int index);

    void listview_refresh();
    void on_lineEdit_search_textChanged(const QString &arg1);

    void on_toolButton_refresh_clicked();

    void on_toolButton_exit_clicked();

    void export_xlsx();

    void create_table_triggered();
signals:
            void

    closedd();

private:
    Ui::MainWindow *ui;
    connect_db *c_db;
    create_table *create_table_window;
    rename_column *r_c;
    uncover_columns *u_c;
    script_window *script_w;

    QStringListModel *tables_list_model;

    void mousePressEvent(QMouseEvent *event);

    void mouseMoveEvent(QMouseEvent *event);

    int m_nMouseClick_X_Coordinate;
    int m_nMouseClick_Y_Coordinate;

    void clearTable();

    void saveFile();

    void show_rows();

    int rows;
    int count_tabs = 1;

    QStringList column_types;
    QStringList column_names;

    QStringList table_list_db;
    QStringList table_list_local;

    QList <QStringList> run_tables_from_db;
    QList <QStringList> run_table_local;

    QString dir;

    void show_table(int index);

    void append_table(QSqlQuery q, bool local);

    void show_table(QStringList curr_table, bool local);

};

#endif // MAINWINDOW_H
