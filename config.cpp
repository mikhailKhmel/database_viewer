#include "config.h"
#include <QDebug>

QStringList config::users;
QSqlDatabase config::db = QSqlDatabase::addDatabase("QSQLITE");
QString config::db_driver;
QString config::dir_db_sqlite;
QSqlDatabase config::work_db;
QString config::hostname;
QString config::username;
QString config::password;
QString config::database_name;
QStringList config::tables_list;

config::current_user config::user;


void config::set_lastused()
{
    QSqlQuery q;
    q.prepare("UPDATE USERS SET LASTUSED = 0 WHERE USERNAME <> " + user.username);
    if (q.exec())
        user.lastused = 1;
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
        else
        {
            QSqlQueryModel model;
            model.setQuery("SELECT * FROM 'USERS'");
            for (int i = 0; i < model.rowCount(); i++)
            {
                users.append(model.record(i).value("USERNAME").toString());
                if (model.record(i).value("LASTUSED").toInt() == 1)
                {
                    user.username = model.record(i).value("USERNAME").toString();
                    user.lastused = 1;
                    user.db_driver = model.record(i).value("DB_DRIVER").toString();
                    user.dir_db_sqlite = model.record(i).value("DIR_DB_SQLITE").toString();
                }
            }

        }
    }
}

void config::save_config()
{
    QSqlQuery q;
    QString s = "UPDATE USERS SET LASTUSED = " + QString::number(user.lastused) + ", DB_DRIVER = '" + user.db_driver + "', DIR_DB_SQLITE = '" + user.dir_db_sqlite + "' WHERE USERNAME = '" + user.username + "'";
    q.prepare(s);
    if (!q.exec())
        qDebug() << q.lastError().text();
}


bool config::check_new_user(QString user)
{
    if (users.contains(user))
        return false;
    else
        return true;
}

bool config::set_new_user(QString user)
{
    if (check_new_user(user))
    {
        QString username = user;
        users.append(username);
        QSqlQuery qry("INSERT INTO USERS (USERNAME) VALUES ('" + username + "')");
        if (!qry.exec())
            qDebug() << qry.lastError().text();
        return true;
    }
    else return false;
}

void config::set_current_user(QString username)
{
    QSqlQuery q;
    if (q.exec("SELECT * FROM USERS WHERE USERNAME = '" + username + "'"))
    {
        user.username = username;
        user.lastused = 1;
        user.db_driver = q.record().value("DB_DRIVER").toString();
        user.dir_db_sqlite = q.record().value("DIR_DB_SQLITE").toString();
    }
    else
        qDebug() << q.lastError().text();

}


void config::set_db_driver(QString db_driver)
{
    config::db_driver = db_driver;
    user.db_driver = db_driver;
}

void config::set_dir_db_sqlite(QString dir)
{
    config::dir_db_sqlite = dir;
    user.dir_db_sqlite=dir;
}
