#ifndef CREATE_COLUMN_H
#define CREATE_COLUMN_H

#include "config.h"
#include <QWidget>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QStringList>
#include <QMessageBox>
#include <QMouseEvent>

namespace Ui {
    class create_column;
}

class create_column : public QWidget {
    Q_OBJECT

public:
    explicit create_column(QWidget *parent = nullptr);

    ~create_column();

private
    slots:
            void

    on_pushButton_2_clicked();

    void on_comboBox_foreigntables_currentIndexChanged(const QString &arg1);

    void on_pushButton_clicked();

    void on_pushButton_3_clicked();


    signals:
            void

    closed(const QString &str);

private:
    Ui::create_column *ui;
    bool foreign_key_flag; //false-выкл. true-вкл
    void mousePressEvent(QMouseEvent *event);

    int m_nMouseClick_X_Coordinate;
    int m_nMouseClick_Y_Coordinate;

    void clear_w();
};

#endif // CREATE_COLUMN_H
