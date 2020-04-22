#include "uncover_columns.h"
#include "ui_uncover_columns.h"
#include "config.h"
#include <QSqlDatabase>

uncover_columns::uncover_columns(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::uncover_columns)
{
    ui->setupUi(this);
    //this->setWindowFlag(Qt::FramelessWindowHint);
}

uncover_columns::~uncover_columns()
{
    delete ui;
}

void uncover_columns::prepeare_window(QString tablename)
{
    this->setWindowTitle("Раскрыть столбцы " + tablename);
    QStringList columns_in_curr_tablename;
    QSqlDatabase db = config::set_current_db();
    if (db.open())
    {
        QSqlRecord rec = db.record(tablename);
        for (int i = 0; i < rec.count(); i++)
            columns_in_curr_tablename.append(rec.fieldName(i));
    }

    QStringList col_hid = config::user.column_hides.split(";");
    col_hid.removeLast();
    QStringList result;
    tables_list_model = new QStringListModel;
    tables_list_model->setStringList(QStringList {});
    foreach(QString s, col_hid)
    {
        QStringList c = s.split(",");
        QString curr_table = c.at(0);
        QString index = c.at(1);
        if (curr_table == tablename)
           result.append(columns_in_curr_tablename.at(index.toInt()));
    }
    tables_list_model->setStringList(result);
    ui->listView->setModel(tables_list_model);
}

void uncover_columns::on_pushButton_back_clicked()
{
    this->close();
}

void uncover_columns::on_pushButton_continue_clicked()
{
    QModelIndex index = ui->listView->currentIndex();
    ui->listView->setSelectionMode(QAbstractItemView::ExtendedSelection);
    QString col_name = index.data(Qt::DisplayRole).toString();

    emit closed(col_name);
    this->close();
}
