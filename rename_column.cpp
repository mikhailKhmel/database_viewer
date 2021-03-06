#include "rename_column.h"
#include "ui_rename_column.h"

rename_column::rename_column(QWidget *parent) :
        QWidget(parent),
        ui(new Ui::rename_column) {
    ui->setupUi(this);
    this->setAttribute(Qt::WA_DeleteOnClose);
    //this->setWindowFlag(Qt::FramelessWindowHint);
}

rename_column::~rename_column() {
    delete ui;
}

//метод перемещения окна
void rename_column::mousePressEvent(QMouseEvent *event) {
    m_nMouseClick_X_Coordinate = event->x();
    m_nMouseClick_Y_Coordinate = event->y();
}

void rename_column::mouseMoveEvent(QMouseEvent *event) {
    move(event->globalX() - m_nMouseClick_X_Coordinate, event->globalY() - m_nMouseClick_Y_Coordinate);
}

//закрыть окно
void rename_column::on_pushButton_back_clicked() {
    this->close();
}

void rename_column::on_pushButton_continue_clicked() {
    if (!ui->lineEdit->text().isEmpty()) {
        emit closed(ui->lineEdit->text()); //вызов сигнала и передача содержимого текстового поля, если не пустое
        this->close();
    }
}
