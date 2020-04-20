#include "userdialog.h"
#include "mainwindow.h"
#include "config.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    a.addLibraryPath(QString("./plugins/sqldrivers"));
    config::load_config();

    QFile styleF;
    styleF.setFileName(":/style.css");
    styleF.open(QFile::ReadOnly);
    QString qssStr = styleF.readAll();
    a.setStyleSheet(qssStr);

    UserDialog w;
    w.show();
    a.exec();

    config::user.lastused = 1;
    config::save_config();
    //exit(1);
    return 0;
}
