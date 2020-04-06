#include "create_column.h"
#include "ui_create_column.h"

create_column::create_column(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::create_column)
{
    ui->setupUi(this);
    create_column::foreign_key_flag = false;
    ui->frame->setVisible(foreign_key_flag);

    ui->columnname->clear();
}

create_column::~create_column()
{
    delete ui;
}

void create_column::on_pushButton_2_clicked()
{
    if (create_column::foreign_key_flag)
        create_column::foreign_key_flag = false;
    else
        create_column::foreign_key_flag = true;

    ui->frame->setVisible(foreign_key_flag);

    ui->comboBox_foreigntables->addItems(config::work_db.tables());
}

void create_column::on_comboBox_foreigntables_currentIndexChanged(const QString &arg1)
{
    QSqlQuery q("SELECT * FROM " + arg1);
    q.exec();
    QSqlRecord rec = q.record();
    QStringList columns;
    for (int i = 0; i < rec.count(); i++)
        columns.append(rec.fieldName(i));

    ui->comboBox_2_foreigncolumns->addItems(columns);
}

void create_column::on_pushButton_clicked()
{
    QString tablename = ui->columnname->text();
    QString type_column = ui->comboBox_type->currentText();
    bool notnull = ui->checkBox_notnull->isChecked();
    bool pk = ui->checkBox_2_pk->isChecked();
    bool ai = ui->checkBox_3_ai->isChecked();
    bool unique = ui->checkBox_4_unique->isChecked();

    //foreign
    QString foreign_table = ui->comboBox_foreigntables->currentText();
    QString foreign_column = ui->comboBox_2_foreigncolumns->currentText();
    QString foreign_expr = ui->lineEdit_foreignexpr->text();

    QString query_str;

    if (!tablename.isEmpty())
    {
        query_str.append("'" + tablename + "' " + type_column + " ");
        if (pk)
            query_str.append("PRIMARY KEY ");

        if (notnull)
            query_str.append("NOT NULL ");

        if (ai)
            query_str.append("AUTOINCREMENT ");

        if (unique)
            query_str.append("UNIQUE ");

        if (!ui->default_value->text().isEmpty())
            query_str.append("DEFAULT " + ui->default_value->text());

        if (create_column::foreign_key_flag)
            query_str.append("FOREIGN KEY(" + foreign_column + ") REFERENCES " + foreign_table + "(" + foreign_column + ") " + foreign_expr);

        //query_str.append(", ");
        emit close(query_str);
        this->destroy();
    }
    else
        QMessageBox::warning(this,"Ошибка", "Задайте имя поля");
}
