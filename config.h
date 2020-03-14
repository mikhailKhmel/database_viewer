#ifndef CONFIG_H
#define CONFIG_H
#include <QStringList>

class config
{
public:
    config();
    static void get_config();
    static void get_users();
    static bool set_new_user(QString user);
    static void set_current_user(QString user);
    static void null_users();
    static void save_config();
    static bool check_new_user(QString user);
    static QStringList config_list;
    static QStringList users;
    static QStringList current_user;
};

#endif // CONFIG_H
