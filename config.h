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
    struct current_user {                       //структура текущего пользователя
        QString username;                       
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

    static void load_config();                  	//загрузка конфига в память
    static void get_users();                    	//выделение только имен пользователей из конфига
    static bool set_new_user(QString user);     	//создания нового пользователя
    static bool set_current_user();             	//установка текущего профиля на основе параметра в конфигурации
    static void set_current_user(QString user); 	//установка текущего профиля на основе имени из списка
    static void null_users();                   	//очищения списка пользователей
    static void save_config();                  	//сохранения конфига из памяти
    static bool check_new_user(QString user);   	//проверка на уникальности имени пользователя
    static QSqlDatabase set_current_db();         //установка соединения с базой данных
    static QVector <current_user> users;          //список пользователей
    static current_user user;                     //данные о текущем пользователе
    static QString curr_database_name;            //имя текущей базы данных. используется для подключение по odbc
    static const QString local_db;                //имя локальной базы данных

private:
    static QSqlDatabase db;                       //экземпляр соединения базы данных
};

#endif // CONFIG_H
