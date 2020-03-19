#ifndef CONNECT_DB_H
#define CONNECT_DB_H

#include <QDialog>

namespace Ui {
class connect_db;
}

class connect_db : public QDialog
{
    Q_OBJECT

public:
    explicit connect_db(QWidget *parent = nullptr);
    ~connect_db();

private slots:
    void on_pushButton_clicked();

    void on_comboBox_currentTextChanged(const QString &arg1);


private:
    Ui::connect_db *ui;
};

#endif // CONNECT_DB_H
