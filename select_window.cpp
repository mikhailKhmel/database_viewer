#include "select_window.h"
#include "ui_select_window.h"

select_window::select_window(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::select_window)
{
    ui->setupUi(this);

}

select_window::~select_window()
{
    delete ui;
}

void select_window::prepareWindow(QString tablename)
{
    QSqlDatabase db = QSqlDatabase::addDatabase(config::user.db_driver);
    if (config::user.db_driver == "QSQLITE")
        db.setDatabaseName(config::user.dir_db_sqlite);
    else
    {
        db.setHostName(config::user.hostname);
        db.setDatabaseName(config::user.databasename);
        db.setUserName(config::user.db_username);
        db.setPassword(config::user.db_password);
    }

    QStringList columns;
    if (db.open())
    {
        QSqlQuery q;
        if (q.exec("SELECT * FROM " + tablename))
        {
            for (int i = 0; i < q.record().count(); i++)
            {
                columns.append(q.record().fieldName(i));
            }
        }
    }
    db.close();
    select_window::curr_table.append(tablename);
    ui->comboBox_selectColumn->clear();
    ui->comboBox_selectColumn->addItems(columns);
    ui->comboBox_select_rovno->setCurrentIndex(0);
    ui->lineEdit->clear();
    ui->lineEdit_2->clear();
    ui->lineEdit_2->setVisible(false);
}

void select_window::on_pushButton_cancel_clicked()
{
    this->close();
}

void select_window::on_pushButton_select_clicked()
{
        QString query_str;
        QString rovno_str;
        query_str.append(ui->comboBox_selectColumn->currentText());
        if (ui->comboBox_select_rovno->currentText() == "Равно")
            rovno_str.append(" = '" + ui->lineEdit->text() + "'");
        else if (ui->comboBox_select_rovno->currentText() == "Не равно")
            rovno_str.append(" <> '" + ui->lineEdit->text() + "'");
        else if (ui->comboBox_select_rovno->currentText() == "Больше")
            rovno_str.append(" > " + ui->lineEdit->text());
        else if (ui->comboBox_select_rovno->currentText() == "Не больше")
            rovno_str.append(" >= " + ui->lineEdit->text());
        else if (ui->comboBox_select_rovno->currentText() == "Меньше")
            rovno_str.append(" < " + ui->lineEdit->text());
        else if (ui->comboBox_select_rovno->currentText() == "Меньше или равно")
            rovno_str.append(" <= " + ui->lineEdit->text());
        else if (ui->comboBox_select_rovno->currentText() == "Содержит")
            rovno_str.append(" LIKE '%"  + ui->lineEdit->text() + "%'");
        else if (ui->comboBox_select_rovno->currentText() == "Не содержит")
            rovno_str.append(" NOT LIKE '%"  + ui->lineEdit->text() + "%'");
        else if ((ui->comboBox_select_rovno->currentText() == "Между") || (ui->lineEdit_2->isVisible()))
            rovno_str.append(" BETWEEN " + ui->lineEdit->text() + " AND " + ui->lineEdit_2->text());
        else if ((ui->comboBox_select_rovno->currentText() == "Вне диапазона") || (ui->lineEdit_2->isVisible()))
            rovno_str.append(" NOT BETWEEN " + ui->lineEdit->text() + " AND " + ui->lineEdit_2->text());

        query_str.append(rovno_str);

        emit closed(query_str);
        this->close();
}

void select_window::on_comboBox_select_rovno_activated(const QString &arg1)
{
    if ((arg1 == "Между") || (arg1 == "Вне диапозона"))
        ui->lineEdit_2->setVisible(true);
    else
        ui->lineEdit_2->setVisible(false);
}
