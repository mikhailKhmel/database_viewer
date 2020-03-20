#include "config.h"
#include <QDebug>

QStringList config::users;
QSqlTableModel config::current_user;
QSqlDatabase config::db = QSqlDatabase::addDatabase("QSQLITE");
QString config::db_driver;
QString config::dir_db_sqlite;

void config::set_lastused()
{
    QSqlRecord record;
    record.setValue("LAST_USED",1);
    current_user.setRecord(0,record);
}

void config::load_config()
{
    db.setDatabaseName("config.db");
    if (db.open())
    {
        QSqlQuery check_query("SELECT * FROM 'USERS'");
        if (!check_query.exec())
        {
            QSqlQuery query("CREATE TABLE 'USERS' "
                            "('USERNAME'	TEXT UNIQUE,"
                            "'LASTUSED'	INTEGER, "
                            "'DB_DRIVER'	TEXT, 'DIR_DB_SQLITE' TEXT)");
            if (!query.exec())
                qDebug() << db.lastError().text();
        }
    }
}

void config::get_users()
{
    QSqlQueryModel model;
    model.setQuery("SELECT USERNAME FROM USERS");
    for (int i = 0; i < model.rowCount(); i++)
        users.append(model.record(i).value("USERNAME").toString());
}

void config::save_config()
{
    set_lastused();
    current_user.submitAll();
}

void config::null_users()
{
    users.clear();
}

bool config::check_new_user(QString user)
{
    user = "[" + user + "]";
    if (users.contains(user))
        return false;
    else
        return true;
}

bool config::set_new_user(QString user)
{
    if (check_new_user(user))
    {
        QString username = "[" + user + "]";
        users.append(username);
        QSqlQuery qry("INSERT INTO USERS (USERNAME) VALUES ('" + username + "')");
        if (!qry.exec())
            qDebug() << qry.lastError().text();
        return true;
    }
    else return false;
}

bool config::set_current_user()
{
    QSqlQueryModel model;
    model.setQuery("SELECT USERNAME, LAST_USED FROM USERS");
    bool last_used = false;
    for (int i = 0; i < model.rowCount(); i++)
    {
        if (model.record(i).value("LAST_USED").toInt() == 1)
        {
            last_used = true;
            QString username = model.record(i).value("USERNAME").toString();
            current_user.setTable("USERS");
            current_user.setFilter(username);
            current_user.select();
        }
    }
    return last_used;
}

void config::set_current_user(QString user)
{
    current_user.setTable("USERS");
    current_user.setFilter(user);
    current_user.select();
}


void config::set_db_driver(QString db_driver)
{
    config::db_driver = db_driver;

    QSqlRecord record;
    record.setValue("DB_DRIVER",db_driver);
    current_user.setRecord(0,record);
}

void config::set_dir_db_sqlite(QString dir)
{
    config::dir_db_sqlite = dir;

    QSqlRecord record;
    record.setValue("DIR_DB_SQLITE",dir);
    current_user.setRecord(0,record);
}
