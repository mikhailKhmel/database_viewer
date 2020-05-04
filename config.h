#ifndef CONFIG_H
#define CONFIG_H

#include <QStringList>
#include <QSql>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QSqlQueryModel>
#include <QSqlTableModel>
#include <QSqlRecord>
#include <QVector>

class config {
public:
    struct current_user {
        QString username;
        int lastused;
        QString db_driver;
        QString dir_db_sqlite;
        QString hostname;
        QString port;
        QString databasename;
        QString db_username;
        QString db_password;

        QString column_renames;
        QString column_hides;
        int lightmode;
    };

    static void load_config();                  //загрузка конфига в память
    static void get_users();                    //выделение только имен пользователей из конфига
    static bool set_new_user(QString user);     //создания нового пользователя
    static bool set_current_user();             //установка текущего профиля на основе параметра в конфигурации
    static void set_current_user(QString user); //установка текущего профиля на основе имени из списка
    static void null_users();                   //очищения списка пользователей
    static void save_config();                  //сохранения конфига из памяти
    static bool check_new_user(QString user);   //проверка на уникальности имени пользователя
    static void set_lastused();

    static void set_db_driver(QString db_driver);

    static void set_dir_db_sqlite(QString dir);

    static QSqlDatabase set_current_db();

    static QVector <current_user> users;
    static QSqlDatabase work_db;

    static QString LastError;

    static current_user user;

    static QString curr_database_name;
    static const QString local_db;

private:
    static QSqlDatabase db;
};

#endif // CONFIG_H
