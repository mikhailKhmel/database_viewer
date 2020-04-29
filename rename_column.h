#ifndef RENAME_COLUMN_H
#define RENAME_COLUMN_H

#include <QWidget>
#include <QMouseEvent>

namespace Ui {
    class rename_column;
}

class rename_column : public QWidget {
    Q_OBJECT

public:
    explicit rename_column(QWidget *parent = 0);

    ~rename_column();

    signals:
            void

    closed(const QString &col);

private
    slots:
            void

    on_pushButton_back_clicked();

    void on_pushButton_continue_clicked();

private:
    Ui::rename_column *ui;
    void mousePressEvent(QMouseEvent *event);
        void mouseMoveEvent(QMouseEvent *event);
        int m_nMouseClick_X_Coordinate;
        int m_nMouseClick_Y_Coordinate;
};

#endif // RENAME_COLUMN_H
