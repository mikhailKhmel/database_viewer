#include "config.h"
#include <QFile>
#include <QTextStream>
#include <QDebug>

QStringList config::config_list;
QStringList config::users;
QStringList config::current_user;

config::config()
{
}

void config::get_config()
{
    QFile file("config.txt");
    if ((file.exists())&&(file.open(QIODevice::ReadOnly)))
    {
        while (!file.atEnd())
            config_list << file.readLine();
        file.close();
    }
    else
    {
        file.open(QIODevice::WriteOnly);
        file.close();
    }
}

void config::get_users()
{
    for (int i = 0; i < config_list.length(); i++)
        if (config_list[i].startsWith("["))
            users.append(config_list[i]);
}

void config::save_config()
{
    QFile file("config.txt");
    if ((file.exists()) && (file.open(QIODevice::WriteOnly)))
    {
        QTextStream stream(&file);
        foreach(QString s, config_list)
            stream<<s;
    }
    file.close();
}

void config::null_users()
{
    users.clear();
}

bool config::check_new_user(QString user)
{
    user = "[" + user + "]";
    if (users.contains(user)||config_list.contains(user))
        return false;
    else
        return true;
}

bool config::set_new_user(QString user)
{
    if (check_new_user(user))
    {
        users.append("[" + user + "]");
        config_list.append("\r\n[" + user + "]");
        return true;
    }
    else return false;
}


void config::set_current_user(QString user)
{
    int user_index=0;
    for (int i = 0; i < config_list.length(); i++)
    {
        if (user == config_list[i])
        {
            current_user.append(config_list[i]);
            user_index = i+1;
            break;
        }
    }
    for (int i = user_index; i < config_list.length(); i++)
    {
        if (config_list[i].contains("[") == false)
            current_user.append(config_list[i]);
        else break;
    }
}
