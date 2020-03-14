#include "userdialog.h"
#include "config.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    config::get_config();
    config::null_users();
    config::get_users();

    UserDialog w;
    w.show();
    a.exec();

    config::save_config();
    return 0;
}

/*
TODO: подключение к бд на основе конфига. ищем сначала такую настройку в конфиге, потом подключаем соответствующий драйвер.
*/
