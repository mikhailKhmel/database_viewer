#ifndef NEWPROFILE_H
#define NEWPROFILE_H

#include <QDialog>
#include <QMouseEvent>

namespace Ui {
    class newprofile;
}

class newprofile : public QDialog {
    Q_OBJECT

public:
    explicit newprofile(QWidget *parent = nullptr);

    ~newprofile();

private
    slots:
            void

    on_pushButton_clicked();

    void on_pushButton_exit_clicked();

    signals:
            void

    closed();

    void close();

private:
    Ui::newprofile *ui;
    void mousePressEvent(QMouseEvent *event);
        void mouseMoveEvent(QMouseEvent *event);
        int m_nMouseClick_X_Coordinate;
        int m_nMouseClick_Y_Coordinate;
};

#endif // NEWPROFILE_H
