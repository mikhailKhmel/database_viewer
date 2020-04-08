#include "config.h"
#include <QDebug>
#include <QMessageBox>

QSqlDatabase config::db = QSqlDatabase::addDatabase("QSQLITE");
QSqlDatabase config::work_db;

QVector<config::current_user> config::users;
config::current_user config::user;
QString config::LastError;

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
                            "'DB_DRIVER'	TEXT, "
                            "'DIR_DB_SQLITE' TEXT,"
                            "'HOSTNAME' TEXT,"
                            "'DATABASENAME' TEXT,"
                            "'DB_USERNAME' TEXT,"
                            "'DB_PASSWORD' TEXT );");
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
                new_user.hostname = model.record(i).value("HOSTNAME").toString();
                new_user.databasename = model.record(i).value("DATABASENAME").toString();
                new_user.db_username = model.record(i).value("DB_USERNAME").toString();
                new_user.db_password = model.record(i).value("DB_PASSWORD").toString();
                users.append(new_user);
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
            users[i].hostname = config::user.hostname;
            users[i].databasename = config::user.databasename;
            users[i].db_username = config::user.db_username;
            users[i].db_password = config::user.db_password;
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
                if (q.exec("INSERT INTO USERS VALUES('"+u.username+"', "+QString::number(u.lastused)+", '"+u.db_driver+"', '"+u.dir_db_sqlite+"', "
                           "'" + u.hostname + "', '" + u.databasename + "', '" + u.db_username + "', '" + u.db_password + "')"))
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
            {LastError = qry.lastError().text(); qDebug() << qry.lastError().text();return true;}
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
