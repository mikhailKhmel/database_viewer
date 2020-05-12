#include "create_column.h"
#include "ui_create_column.h"
#include <QDebug>

create_column::create_column(QWidget *parent) :
        QWidget(parent),
        ui(new Ui::create_column) {
    ui->setupUi(this);
    this->setAttribute(Qt::WA_DeleteOnClose);
    //this->setWindowFlag(Qt::FramelessWindowHint);

    create_column::foreign_key_flag = false;
    ui->frame->setVisible(foreign_key_flag);

    ui->columnname->clear();
}

create_column::~create_column() {
    delete ui;
}

//методы для перемещения окна
void create_column::mousePressEvent(QMouseEvent *event) {
    m_nMouseClick_X_Coordinate = event->x();
    m_nMouseClick_Y_Coordinate = event->y();
}

void create_column::mouseMoveEvent(QMouseEvent *event) {
    move(event->globalX() - m_nMouseClick_X_Coordinate, event->globalY() - m_nMouseClick_Y_Coordinate);
}

//раскрытие элементов для работы с внешними ключами
void create_column::on_pushButton_2_clicked() {
    if (create_column::foreign_key_flag)
        create_column::foreign_key_flag = false;
    else
        create_column::foreign_key_flag = true;

    ui->frame->setVisible(foreign_key_flag);
	
    QSqlDatabase curr_db = config::set_current_db();
    ui->comboBox_foreigntables->addItems(curr_db.tables());
}

//
void create_column::on_comboBox_foreigntables_currentIndexChanged(const QString &arg1) {
    QSqlDatabase db = config::set_current_db();
	if (db.open()){
				QSqlQuery q("SELECT * FROM " + arg1);
				q.exec();
				QSqlRecord rec = q.record();
				QStringList columns;
				for (int i = 0; i < rec.count(); i++)
					columns.append(rec.fieldName(i));

				ui->comboBox_2_foreigncolumns->addItems(columns);

	}
}

//создание строки новой колонки таблицы
void create_column::on_pushButton_clicked() {
    QString tablename = ui->columnname->text();			//имя таблицы
    QString type_column = ui->lineEdit_type->text();	//тип таблицы
    bool notnull = ui->checkBox_notnull->isChecked();	//not null
    bool pk = ui->checkBox_2_pk->isChecked();			//primary key
    bool ai = ui->checkBox_3_ai->isChecked();			//autoincrement
    bool unique = ui->checkBox_4_unique->isChecked();	//unique

    //foreign key
    QString foreign_table = ui->comboBox_foreigntables->currentText();
    QString foreign_column = ui->comboBox_2_foreigncolumns->currentText();
    QString foreign_expr = ui->lineEdit_foreignexpr->text();

    QString query_str = "";
    query_str.clear();
	
	//сбор данных и добавление в конец результативной строки
    if (!tablename.isEmpty()) {
        query_str.append(tablename + " " + type_column + " ");
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
        else
            query_str.append("DEFAULT NULL");

        if (create_column::foreign_key_flag)
            query_str.append(
                    "FOREIGN KEY(" + foreign_column + ") REFERENCES " + foreign_table + "(" + foreign_column + ") " +
                    foreign_expr);

        //query_str.append(", ");
        qDebug() << query_str.size();
        clear_w();
        emit closed(query_str);	//вызов сигнала и передача результативной строки
        this->destroy(); //закрытие окна
    } else
        QMessageBox::warning(this, "Ошибка", "Задайте имя поля");
}

void create_column::on_pushButton_3_clicked() {	//закрытие окна
    this->close();
}

void create_column::clear_w() { //метод очищения текстовых полей
    ui->lineEdit_type->clear();
    ui->columnname->clear();
}
