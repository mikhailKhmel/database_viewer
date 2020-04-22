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
