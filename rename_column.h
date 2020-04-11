#ifndef RENAME_COLUMN_H
#define RENAME_COLUMN_H

#include <QWidget>

namespace Ui {
class rename_column;
}

class rename_column : public QWidget
{
    Q_OBJECT

public:
    explicit rename_column(QWidget *parent = 0);
    ~rename_column();

private:
    Ui::rename_column *ui;
};

#endif // RENAME_COLUMN_H
