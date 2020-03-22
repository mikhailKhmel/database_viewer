#ifndef CREATE_TABLE_H
#define CREATE_TABLE_H

#include <QWidget>

namespace Ui {
class create_table;
}

class create_table : public QWidget
{
    Q_OBJECT

public:
    explicit create_table(QWidget *parent = nullptr);
    ~create_table();

private:
    Ui::create_table *ui;
};

#endif // CREATE_TABLE_H
