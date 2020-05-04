#ifndef SCRIPT_WINDOW_H
#define SCRIPT_WINDOW_H

#include "config.h"
#include <QWidget>
#include <QScrollBar>

namespace Ui {
    class script_window;
}

class script_window : public QWidget {
    Q_OBJECT

public:
    explicit script_window(QWidget *parent = nullptr);

    ~script_window();


private
    slots:
            void

    on_textEdit_cursorPositionChanged();
            void setV();


private:
    Ui::script_window *ui;

    int rows;

    void show_rows();
};

#endif // SCRIPT_WINDOW_H
