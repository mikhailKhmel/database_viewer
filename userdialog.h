#ifndef USERDIALOG_H
#define USERDIALOG_H

#include <QDialog>
#include "mainwindow.h"
#include "newprofile.h"

namespace Ui {
class UserDialog;
}

class UserDialog : public QDialog
{
    Q_OBJECT

public:
    explicit UserDialog(QWidget *parent = nullptr);
    ~UserDialog();

private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

    void showd();

private:
    Ui::UserDialog *ui;
    MainWindow *l;
    newprofile *np;
};

#endif // USERDIALOG_H
