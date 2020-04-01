#ifndef NEWPROFILE_H
#define NEWPROFILE_H

#include <QDialog>

namespace Ui {
class newprofile;
}

class newprofile : public QDialog
{
    Q_OBJECT

public:
    explicit newprofile(QWidget *parent = nullptr);
    ~newprofile();

private slots:
    void on_pushButton_clicked();
    void on_pushButton_exit_clicked();

signals:
    void closed();
    void close();
private:
    Ui::newprofile *ui;
};

#endif // NEWPROFILE_H
