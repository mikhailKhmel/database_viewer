#include "userdialog.h"
#include "mainwindow.h"
#include "config.h"
#include <QApplication>

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);

    config::load_config();  //выгрузка информации о всех пользователях


    UserDialog w;
    w.show();               //открываем окно выбора пользователя
    a.exec();

    config::save_config();  //после завершения программы, выгруить данные из оперативной памяти в локальную бд
    return 0;
}
