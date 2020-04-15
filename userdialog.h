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
    void on_toolButton_clicked();

    void on_toolButton_2_clicked();

    void showd();

    void on_toolButton_3_clicked();

private:
    Ui::UserDialog *ui;
    MainWindow *l;
    newprofile *np;
};

#endif // USERDIALOG_H
