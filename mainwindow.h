#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "config.h"
#include "connect_db.h"
#include <QDebug>
#include <QStringListModel>
#include <QAbstractItemView>
#include <QMainWindow>
#include <QResizeEvent>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
protected:
    void resizeEvent(QResizeEvent* event);


private slots:
    void on_connect_db_triggered();

    void load_db();

    void on_listView_tables_doubleClicked(const QModelIndex &index);

    void on_create_table_triggered();

private:
    Ui::MainWindow *ui;

    connect_db* l;
};

#endif // MAINWINDOW_H
