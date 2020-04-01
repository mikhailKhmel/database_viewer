#include "create_table.h"
#include "ui_create_table.h"

create_table::create_table(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::create_table)
{
    ui->setupUi(this);
    //create_table::create_table_query.append("");
    l = new create_column;

    connect(l, SIGNAL(close(QString)), this, SLOT(save_new_column(QString)));

    create_table::create_table_query.clear();
    create_table::create_table_query.append("");
    create_table::create_table_query.append(");");
}

create_table::~create_table()
{
    delete ui;
}

void create_table::prepare_window()
{
    ui->textEdit->clear();
}

void create_table::update_query()
{
    ui->textEdit->clear();
    foreach(QString s, create_table::create_table_query)
        ui->textEdit->append(s);
}

void create_table::save_new_column(QString str)
{
    create_table::create_table_query[create_table::create_table_query.size()-2] = create_table::create_table_query[create_table::create_table_query.size()-2] + ", ";
    create_table::create_table_query.append(str);
    create_table::create_table_query.swap(create_table::create_table_query.size()-1,create_table::create_table_query.size()-2);
    create_table::update_query();
}

void create_table::on_toolButton_clicked()
{
    l->show();
}

void create_table::on_tablename_textChanged(const QString &arg1)
{
    create_table::create_table_query[0] = "CREATE TABLE '" + arg1 + "' (";
    create_table::update_query();
}

void create_table::on_pushButton_clicked()
{
    if (!ui->tablename->text().isEmpty())
    {
        create_table::create_table_query.append(");");
        QSqlQuery qry;
        QString q = ui->textEdit->toPlainText();
//        foreach(QString s, create_table::create_table_query)
//            q.append(s);
        //q.remove(" ,");
        q.remove("\n");
        q.replace("(,", "(");
        qry.prepare(q);
        if (qry.exec())
        {
            emit closed();
            this->destroy();
        }
        else
            QMessageBox::warning(this,"Error",qry.lastError().text());
    }
}