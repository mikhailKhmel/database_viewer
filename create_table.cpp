#include "create_table.h"
#include "ui_create_table.h"

create_table::create_table(QWidget *parent) :
        QWidget(parent),
        ui(new Ui::create_table) {
    ui->setupUi(this);
    this->setWindowFlag(Qt::FramelessWindowHint);
    //create_table::create_table_query.append("");
    l = new create_column;

    connect(l, SIGNAL(closed(
    const QString&)), this, SLOT(save_new_column(
    const QString&)));

    create_table::create_table_query.clear();
    create_table::create_table_query.append("");
    create_table::create_table_query.append(");");
}

create_table::~create_table() {
    delete ui;
}

void create_table::mousePressEvent(QMouseEvent *event) {
    m_nMouseClick_X_Coordinate = event->x();
    m_nMouseClick_Y_Coordinate = event->y();
}

void create_table::mouseMoveEvent(QMouseEvent *event) {
    move(event->globalX() - m_nMouseClick_X_Coordinate, event->globalY() - m_nMouseClick_Y_Coordinate);
}

void create_table::prepare_window() {
    ui->tablename->clear();
    ui->textEdit->clear();
    create_table::create_table_query.clear();
    create_table::create_table_query.append("");
    create_table::create_table_query.append(");");
}


void create_table::update_query() {
    ui->textEdit->clear();
    QString result = create_table_query.join("\n");

    ui->textEdit->setText(result);

}

void create_table::save_new_column(const QString &str) {
    qDebug() << create_table::create_table_query.size();
    create_table::create_table_query[create_table::create_table_query.size() - 2].append(", ");
    create_table::create_table_query.append(str);
    create_table::create_table_query.swap(create_table::create_table_query.size() - 1,
                                          create_table::create_table_query.size() - 2);
    create_table::update_query();
}

void create_table::on_toolButton_clicked() {
    l->show();
}

void create_table::on_tablename_textChanged(const QString &arg1) {
    create_table::create_table_query[0] = "CREATE TABLE " + arg1 + " (";
    create_table::update_query();
}

void create_table::on_pushButton_clicked() {
    if (!ui->tablename->text().isEmpty()) {
        create_table::create_table_query.append(");");
        QSqlQuery qry;
        QString q = ui->textEdit->toPlainText();
        q.remove("\n");
        q.replace("(,", "(");
        //qry.prepare(q);
        if (qry.exec(q)) {
            emit closed();
            this->destroy();
        } else
            QMessageBox::warning(this, "Error", qry.lastError().text());
    }

}

void create_table::on_pushButton_2_clicked() {
    this->close();
}
