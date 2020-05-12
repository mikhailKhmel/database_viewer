#include "userdialog.h"

#include "ui_userdialog.h"
#include "config.h"

#include <QDebug>

UserDialog::UserDialog(QWidget *parent) :
        QDialog(parent),
        ui(new Ui::UserDialog) {
    ui->setupUi(this);
    this->setAttribute(Qt::WA_DeleteOnClose);
    //this->setWindowFlag(Qt::FramelessWindowHint);

    QStringList users;
    foreach(config::current_user
    s, config::users) { users.append(s.username); } //заполнение комбобокса именами профилей
    ui->comboBox->addItems(users);

    np = new newprofile;    //объект окна нового профиля
    connect(np, SIGNAL(closed()), this, SLOT(showd())); //принятие сигнала о закрытии окна нового профиля
    l = new MainWindow; //объейт окна основного окна
    connect(l, SIGNAL(closedd()), this, SLOT(showd())); //принятие сигнала о закрытии основного окна
}

UserDialog::~UserDialog() {
    delete ui;
}

void UserDialog::mousePressEvent(QMouseEvent *event) {  //метод для обработки событие нажатия кнопки мыши
    m_nMouseClick_X_Coordinate = event->x();
    m_nMouseClick_Y_Coordinate = event->y();
}

void UserDialog::mouseMoveEvent(QMouseEvent *event) {   //метод передвижения окна
    move(event->globalX() - m_nMouseClick_X_Coordinate, event->globalY() - m_nMouseClick_Y_Coordinate);
}

void UserDialog::showd() {  //слот обработки закрытие дочерних окон
    if (config::user.lightmode == 0) {  //применение стилей
        QFile styleF;
        styleF.setFileName(":/dark.css");
        styleF.open(QFile::ReadOnly);
        QString qssStr = styleF.readAll();
        qApp->setStyleSheet(qssStr);
    } else {
        QFile styleF;
        styleF.setFileName(":/light.css");
        styleF.open(QFile::ReadOnly);
        QString qssStr = styleF.readAll();
        qApp->setStyleSheet(qssStr);
    }

    //обновление комбобокса
    ui->comboBox->clear();
    QStringList users;
    foreach(config::current_user
    s, config::users) { users.append(s.username); }
    ui->comboBox->addItems(users);
    this->update();
    this->setVisible(true);
}

void UserDialog::on_toolButton_clicked() {

    np->show(); //открытие окна нового профиля
    this->setVisible(false);
    //this->close();
}

void UserDialog::on_toolButton_2_clicked() {
    QString username = ui->comboBox->currentText();
    if (!username.isEmpty())
        config::set_current_user(username); //установка текущего пользователя
    else
        return;

    l->prepare_window();    //подготовка окна
    l->show();  //отобразить основое окно
    this->setVisible(false);
}

void UserDialog::on_toolButton_3_clicked() {
    qApp->closeAllWindows();    //закрыть приложение
}

void UserDialog::on_comboBox_currentIndexChanged(const QString &arg1) {
    for (int i = 0; i < config::users.count(); ++i) {   //применение стилей при смене пользователя
        if (arg1 == config::users.at(i).username) {
            if (config::users.at(i).lightmode == 0) {
                QFile styleF;
                styleF.setFileName(":/dark.css");
                styleF.open(QFile::ReadOnly);
                QString qssStr = styleF.readAll();
                qApp->setStyleSheet(qssStr);
            } else {
                QFile styleF;
                styleF.setFileName(":/light.css");
                styleF.open(QFile::ReadOnly);
                QString qssStr = styleF.readAll();
                qApp->setStyleSheet(qssStr);
            }
        }
    }
}
