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

class config
{
public:
    static void load_config();                          //загрузка конфига в память
    static void get_users();                            //выделение только имен пользователей из конфига
    static bool set_new_user(QString user);             //создания нового пользователя
    static bool set_current_user();                     //установка текущего профиля на основе параметра в конфигурации
    static void set_current_user(QString user);         //установка текущего профиля на основе имени из списка
    static void null_users();                           //очищения списка пользователей
    static void save_config();                          //сохранения конфига из памяти
    static bool check_new_user(QString user);           //проверка на уникальности имени пользователя
    static void set_lastused();
    static void set_db_driver(QString db_driver);
    static void set_dir_db_sqlite(QString dir);
    static QStringList users;                           //контейнер для хранения только имен пользователей
    static QSqlTableModel current_user;                    //имя текущего пользователя
    static QString db_driver;
    static QString dir_db_sqlite;
    static QString hostname;
    static QString username;
    static QString password;
    static QString database_name;

    static QSqlDatabase work_db;

private:
    static QSqlDatabase db;
};

#endif // CONFIG_H
