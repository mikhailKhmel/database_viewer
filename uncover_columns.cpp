#include "uncover_columns.h"
#include "ui_uncover_columns.h"
#include "config.h"
#include <QSqlDatabase>

uncover_columns::uncover_columns(QWidget *parent) :
        QWidget(parent),
        ui(new Ui::uncover_columns) {
    ui->setupUi(this);
    this->setAttribute(Qt::WA_DeleteOnClose);
    //this->setWindowFlag(Qt::FramelessWindowHint);
}

uncover_columns::~uncover_columns() {
    delete ui;
}

void uncover_columns::mousePressEvent(QMouseEvent *event) {
    m_nMouseClick_X_Coordinate = event->x();
    m_nMouseClick_Y_Coordinate = event->y();
}

void uncover_columns::mouseMoveEvent(QMouseEvent *event) {
    move(event->globalX() - m_nMouseClick_X_Coordinate, event->globalY() - m_nMouseClick_Y_Coordinate);
}

void uncover_columns::prepeare_window(QString tablename) {
    this->setWindowTitle("Раскрыть столбцы " + tablename);

    QSqlDatabase db = config::set_current_db();
    if (db.open()) {
        QSqlRecord rec = db.record(tablename);
        for (int i = 0; i < rec.count(); i++)
            columns_in_curr_tablename.append(rec.fieldName(i));
    }
    QSqlDatabase::removeDatabase(config::curr_database_name);

    QStringList col_hid = config::user.column_hides.split("!");
    col_hid.removeLast();
    QStringList result;
    tables_list_model = new QStringListModel;
    tables_list_model->setStringList(QStringList{});
    foreach(QString
    s, col_hid)
    {
        QStringList c = s.split("?");
        QString curr_table = c.at(0);
        QString index = c.at(1);
        if (curr_table == tablename)
            result.append(columns_in_curr_tablename.at(index.toInt()));
    }
    tables_list_model->setStringList(result);
    ui->listView->setModel(tables_list_model);
}

void uncover_columns::on_pushButton_back_clicked() {
    this->close();
}

void uncover_columns::on_pushButton_continue_clicked() {
    QModelIndex index = ui->listView->currentIndex();
    ui->listView->setSelectionMode(QAbstractItemView::ExtendedSelection);

    emit closed(columns_in_curr_tablename.at(index.row()));
    this->close();
}
