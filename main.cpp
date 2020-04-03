#include "userdialog.h"
#include "mainwindow.h"
#include "config.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    config::load_config();

    UserDialog w;
    w.show();
    a.exec();


    config::user.lastused = 1;
    config::save_config();
    //exit(1);
    return 0;
}

/*
TODO
1.  Меню модификации таблицы.
    Можно использовать меню создания новой таблицы,
    но заранее подгружать туда данные о текущей таблице.

2. Прописать подключение к остальным драйверам. Пока что без тестирования.

3. Переделать параметры профиля. Добавить новые параметры для будущих новвоведений.

4. Добавить возможность скрывать столбцы. Добавить соответстующий параметр в данные профиля.

*/
