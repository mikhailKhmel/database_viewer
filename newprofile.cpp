#include "newprofile.h"
#include "ui_newprofile.h"
#include "userdialog.h"
#include "config.h"
#include <QDebug>

newprofile::newprofile(QWidget *parent) :
        QDialog(parent),
        ui(new Ui::newprofile) {
    ui->setupUi(this);
    this->setAttribute(Qt::WA_DeleteOnClose);
    ui->error_label->setVisible(false);
    //this->setWindowFlag(Qt::FramelessWindowHint);
}

newprofile::~newprofile() {
    delete ui;
}

//перемещение окна
void newprofile::mousePressEvent(QMouseEvent *event) {
    m_nMouseClick_X_Coordinate = event->x();
    m_nMouseClick_Y_Coordinate = event->y();
}

void newprofile::mouseMoveEvent(QMouseEvent *event) {
    move(event->globalX() - m_nMouseClick_X_Coordinate, event->globalY() - m_nMouseClick_Y_Coordinate);
}

//добавление нового пользователя
void newprofile::on_pushButton_clicked() {
    QString newuser = ui->lineEdit->text();
    if (!newuser.isEmpty()) {   //если текстовое поле не пустое
        bool result = config::set_new_user(newuser);    //установить новое имя пользователя, если имя уникально
        if (result) {
            emit closed();  //закрыть окно
            this->destroy();
        } else {
            ui->error_label->setText(config::LastError);    //вывести ошибку
            ui->error_label->setVisible(true);
        }
    }
}

//метод закрытия окна
void newprofile::on_pushButton_exit_clicked() {
    emit closed();
    this->destroy();
}
