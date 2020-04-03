#include "delete_column.h"
#include "ui_delete_column.h"

delete_column::delete_column(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::delete_column)
{
    ui->setupUi(this);
}

delete_column::~delete_column()
{
    delete ui;
}

void delete_column::prepare_window(QStringList fields, QString current_table)
{
    ui->comboBox->addItems(fields);
    delete_column::current_table = current_table;
}

void delete_column::on_pushButton_back_clicked()
{
    this->destroy();
}

void delete_column::on_pushButton_submit_clicked()
{
    if (config::user.db_driver == "QSQLITE")
    {
        QSqlDatabase db = QSqlDatabase::addDatabase(config::user.db_driver);
        db.setDatabaseName(config::user.dir_db_sqlite);
        if (db.open())
        {
            QSqlQuery q;
            q.prepare("PRAGMA table_info('"+current_table+"')");
            q.exec();
            QSqlRecord r = q.record();

            QStringList field_names;
            QStringList field_types;
            QStringList notnull;
            QStringList dflt_value;
            QStringList pk;
            while (q.next())
            {
                field_names.append(q.value(r.indexOf("name")).toString());
                field_types.append(q.value(r.indexOf("type")).toString());
                notnull.append(q.value(r.indexOf("notnull")).toString());
                dflt_value.append(q.value(r.indexOf("dflt_value")).toString());
                pk.append(q.value(r.indexOf("pk")).toString());
            }
            QStringList field_names_old = field_names;
            field_names.removeOne(ui->comboBox->currentText());
            q.clear();
            q.prepare("ALTER TABLE " + current_table + " RENAME TO " + current_table + "_old;");
            if (!q.exec())
                qDebug() << "ALTER TABLE RENAME " << q.lastError().text();

            QStringList create_t;
            create_t.append("CREATE TABLE '" + current_table + "' (");
            for (int i = 0; i < field_names.count(); i++)
            {
                QString s = "'" + field_names[i] + "' " + field_types[i];
                if (notnull[i] == "1") s.append(" NOT NULL");
                if (!dflt_value[i].isEmpty()) s.append(" DEFAULT '" + dflt_value[i]);
                if (pk[i] == "1") s.append(" PRIMARY KEY");
                s.append(" ,");
                create_t.append(s);
            }
            create_t[create_t.count()-1].remove(" ,");
            create_t.append(");");

            QString query_str;
            foreach (QString s, create_t)
                query_str.append(s);

            q.clear();
            q.prepare(query_str);
            if (q.exec())
            {
                field_names.removeOne(ui->comboBox->currentText());
                QString fields;
                foreach (QString s, field_names)
                    fields.append(s + ", ");
                fields.remove(fields.size() - 2, 2);
                q.clear();
                QString s = "INSERT INTO " + current_table + " (" + fields + ") SELECT "+fields+" FROM " + current_table + "_old;";
                q.prepare(s);
                if (q.exec())
                {
                    q.clear();
                    q.prepare("DROP TABLE " + current_table + "_old");
                    if (q.exec())
                    {
                        emit closed();
                        this->destroy();
                    }
                } else qDebug() << "insert into " << q.lastError().text();
            }
            else qDebug() << "create table " << q.lastError().text();
        }
    }
    else {/*для других драйверов*/}

}
