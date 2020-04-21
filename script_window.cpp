#include "script_window.h"
#include "ui_script_window.h"

script_window::script_window(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::script_window)
{
    ui->setupUi(this);
}

script_window::~script_window()
{
    delete ui;
}

void script_window::prepare_window()
{
    ui->textEdit->clear();
    ui->tabWidget->setVisible(false);
    ui->label->setText("Успешно");
    ui->label->setVisible(false);
    QPalette palette = ui->label->palette();
    palette.setColor(ui->label->foregroundRole(), Qt::green);

    errors.clear();
    dir.clear();
}

void script_window::saveFile()
{
    dir = QFileDialog::getSaveFileName(0, "Сохранить файл", dir, "*.sql *.txt");
    if (!dir.isEmpty())
    {
        QFile file(dir);
        if (file.open(QIODevice::ReadWrite))
        {
            QTextStream stream (&file);
            stream << ui->textEdit->toPlainText();
            file.flush();
            file.close();
        }
        else
        {
            QMessageBox::critical(this, "Ошибка записи", "Ошибка записи файла");
        }
    }
}

void script_window::on_toolButton_create_clicked()
{
    if (!ui->textEdit->document()->isEmpty())
    {
        QMessageBox::StandardButton reply;
        reply = QMessageBox::question(this, "Предупреждение", "Сохранить изменения?",
                                      QMessageBox::Yes|QMessageBox::No);
        if (reply == QMessageBox::Yes)
            saveFile();

        ui->textEdit->clear();
    }
}

void script_window::resizeEvent(QResizeEvent *event)
{
    QSize s = event->size();
    this->resize(s);
    ui->tabWidget->resize(s);
    if (ui->tabWidget->count() < 1)
    {}
    else
    {
        for(int i=0;i<ui->tabWidget->count();i++)
               if(i!=ui->tabWidget->currentIndex())
                   ui->tabWidget->widget(i)->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);

        ui->tabWidget->widget(ui->tabWidget->currentIndex())->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
        ui->tabWidget->currentWidget()->resize(ui->tabWidget->size());
        ui->tabWidget->widget(ui->tabWidget->currentIndex())->adjustSize();
    }
}

void script_window::on_toolButton_open_clicked()
{
    ui->tabWidget->clear();
    ui->tabWidget->setVisible(false);
    dir = QFileDialog::getOpenFileName(0, "Открыть файл", dir, "*.sql *.txt");
    if (!dir.isEmpty())
    {
        QFile file(dir);
        if (file.open(QIODevice::ReadOnly))
        {
            QTextStream stream (&file);
            ui->textEdit->setText(stream.readAll());
            this->setWindowTitle(dir);
            file.flush();
            file.close();
        }
        else
        {
            QMessageBox::critical(this, "Ошибка чтения", "Ошибка чтения файла");
        }
    }
}

void script_window::on_toolButton_save_clicked()
{
    saveFile();
}

void script_window::on_toolButton_runscript_clicked()
{
    ui->tabWidget->clear();
    QSqlDatabase db = QSqlDatabase::addDatabase(config::user.db_driver);
    if (config::user.db_driver == "QSQLITE")
        db.setDatabaseName(config::user.dir_db_sqlite);
    else
    {
        db.setHostName(config::user.hostname);
        db.setDatabaseName(config::user.databasename);
        db.setUserName(config::user.db_username);
        db.setPassword(config::user.db_password);
    }

    if (db.open())
    {
        QSqlQuery q;
        QString query_str = ui->textEdit->document()->toPlainText();
        query_str.remove("\n");
        QStringList query_list = query_str.split(";");
        query_list.removeLast();
        foreach (QString s, query_list)
        {
            if (s.contains("select", Qt::CaseInsensitive))
            {
                q.clear();
                if (q.exec(s))
                {
                    QWidget* new_widget = new QWidget(ui->tabWidget);
                    QSqlQueryModel* model = new QSqlQueryModel;
                    model->setQuery(q);
                    QTableView* new_table = new QTableView(new_widget);
                    new_table->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
                    new_table->setModel(model);

                    ui->tabWidget->addTab(new_widget,s.mid(s.indexOf("from ", Qt::CaseInsensitive)+QString("from ").count()));
                }
                else
                    errors.append(q.lastError().text());

                ui->tabWidget->setVisible(true);
            }
            else
            {
                q.clear();
                if (!q.exec(s))
                    errors.append(q.lastError().text());
            }

            ui->label->setVisible(true);
        }

        if (!errors.isEmpty())
        {
            QVBoxLayout *l = new QVBoxLayout;
            QWidget* err_widget = new QWidget(ui->tabWidget);
            err_widget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
            err_widget->setLayout(l);
            QListView* list_view = new QListView(err_widget);
            list_view->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
            QStringListModel* err_model = new QStringListModel;
            err_model->setStringList(errors);

            err_widget->resize(ui->tabWidget->size());
            list_view->resize(ui->tabWidget->size());
            list_view->setModel(err_model);
            ui->tabWidget->addTab(err_widget, "Ошибки");
        }
    }
}

void script_window::on_textEdit_textChanged()
{
    ui->label->setVisible(false);
}

void script_window::closeEvent(QCloseEvent *event)
{
    QFile file(dir);
    if (file.open(QIODevice::ReadWrite))
    {
        QTextStream stream (&file);
        stream << ui->textEdit->toPlainText();
        file.flush();
        file.close();
    }
    prepare_window();
    emit closed();
    event->accept();
}
