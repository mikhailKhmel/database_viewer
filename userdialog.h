#ifndef USERDIALOG_H
#define USERDIALOG_H

#include <QDialog>
#include <QApplication>
#include <QMouseEvent>
#include "mainwindow.h"
#include "newprofile.h"

namespace Ui {
    class UserDialog;
}

class UserDialog : public QDialog {
    Q_OBJECT

public:
    explicit UserDialog(QWidget *parent = nullptr);

    ~UserDialog();

private
    slots:
            void

    on_toolButton_clicked();

    void on_toolButton_2_clicked();

    void showd();

    void on_toolButton_3_clicked();

    void on_comboBox_currentIndexChanged(const QString &arg1);

private:
    Ui::UserDialog *ui;
    MainWindow *l;
    newprofile *np;

    void mousePressEvent(QMouseEvent *event);

    void mouseMoveEvent(QMouseEvent *event);

    int m_nMouseClick_X_Coordinate;
    int m_nMouseClick_Y_Coordinate;
};

#endif // USERDIALOG_H
