#ifndef SCRIPT_WINDOW_H
#define SCRIPT_WINDOW_H

#include "config.h"
#include <QWidget>
#include <QMessageBox>
#include <QResizeEvent>
#include <QFileDialog>
#include <QTextStream>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QSqlQueryModel>
#include <QDebug>
#include <QTableView>
#include <QStringListModel>
#include <QListView>

namespace Ui {
class script_window;
}

class script_window : public QWidget
{
    Q_OBJECT

public:
    explicit script_window(QWidget *parent = nullptr);
    ~script_window();

    void prepare_window();

protected:
    void resizeEvent(QResizeEvent* event);

signals:
    void closed();

private slots:
    void on_toolButton_create_clicked();

    void on_toolButton_open_clicked();

    void on_toolButton_save_clicked();

    void on_toolButton_runscript_clicked();

    void on_textEdit_textChanged();

private:
    Ui::script_window *ui;

    void saveFile();
    QString dir;
    QStringList errors;
    void closeEvent(QCloseEvent *event);
};

#endif // SCRIPT_WINDOW_H