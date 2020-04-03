#include "config.h"
#include <QDebug>

QSqlDatabase config::db = QSqlDatabase::addDatabase("QSQLITE");
QSqlDatabase config::work_db;

QVector<config::current_user> config::users;
config::current_user config::user;


void config::set_lastused()
{
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("config.db");
    if (db.open())
    {
        QSqlQuery q;
        q.prepare("UPDATE USERS SET LASTUSED = 0 WHERE USERNAME <> " + user.username);
        if (q.exec())
            user.lastused = 1;
    }
}

void config::load_config()
{
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
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
                current_user new_user;
                new_user.username = model.record(i).value("USERNAME").toString();
                new_user.lastused = 1;
                new_user.db_driver = model.record(i).value("DB_DRIVER").toString();
                new_user.dir_db_sqlite = model.record(i).value("DIR_DB_SQLITE").toString();
                users.append(new_user);
//                if (model.record(i).value("LASTUSED").toInt() == 1)
//                {
//                    user.username = model.record(i).value("USERNAME").toString();
//                    user.lastused = 1;
//                    user.db_driver = model.record(i).value("DB_DRIVER").toString();
//                    user.dir_db_sqlite = model.record(i).value("DIR_DB_SQLITE").toString();
//                }
            }

        }
    }
}

void config::save_config()
{
    for (int i = 0; i < config::users.size(); i++)
    {
        if (config::users.at(i).username == config::user.username)
        {
            users[i].username = config::user.username;
            users[i].lastused = 1;
            users[i].db_driver = config::user.db_driver;
            users[i].dir_db_sqlite = config::user.dir_db_sqlite;
        }
    }

    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("config.db");
    if (db.open())
    {
        QSqlQuery q;
        if (q.exec("DELETE FROM USERS"))
        {
            q.clear();
            foreach(config::current_user u, config::users)
            {
                if (q.exec("INSERT INTO USERS VALUES('"+u.username+"', "+QString::number(u.lastused)+", '"+u.db_driver+"', '"+u.dir_db_sqlite+"')"))
                    continue;
                else
                    qDebug() << q.lastError().text();
                q.clear();
            }
        }

    }
}


bool config::check_new_user(QString user)
{
    bool ans = true;
    for (int i = 0; i<users.size(); i++)
    {
        if (users.at(i).username == user)
        {ans = false;break;}
    }
    return  ans;
}

bool config::set_new_user(QString user)
{
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("config.db");
    if (db.open())
    {
        if (check_new_user(user))
        {
            QString username = user;
            current_user new_user;
            new_user.username = user;
            users.append(new_user);
            QSqlQuery qry("INSERT INTO USERS (USERNAME) VALUES ('" + username + "')");
            if (!qry.exec())
            {qDebug() << qry.lastError().text();return true;}
            else return true;
        }
        else return false;
    } else return false;
}

void config::set_current_user(QString username)
{
    foreach(config::current_user u, config::users)
    {
        if (u.username == username)
        {
            config::user = u;
            break;
        }
    }
}


void config::set_db_driver(QString db_driver)
{
    user.db_driver = db_driver;
}

void config::set_dir_db_sqlite(QString dir)
{
    user.dir_db_sqlite=dir;
}
