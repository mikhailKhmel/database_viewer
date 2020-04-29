#include "script_window.h"
#include "ui_script_window.h"

script_window::script_window(QWidget *parent) :
        QWidget(parent),
        ui(new Ui::script_window) {
    ui->setupUi(this);
    this->setWindowFlag(Qt::FramelessWindowHint);
    connect(ui->textEdit->verticalScrollBar(), SIGNAL(valueChanged(int)), this, SLOT(sliderMoved(int)));

    ui->splitter->setHandleWidth(1);
    ui->splitter->setStretchFactor(0, 1);
    ui->splitter->setStretchFactor(1, 15);

    if (config::user.lightmode == 0) {
        ui->textEdit->setStyleSheet("color: #111111; background: #736f6f");
    }
}

script_window::~script_window() {
    delete ui;
}

void script_window::mousePressEvent(QMouseEvent *event) {
    m_nMouseClick_X_Coordinate = event->x();
    m_nMouseClick_Y_Coordinate = event->y();
}

void script_window::mouseMoveEvent(QMouseEvent *event) {
    move(event->globalX() - m_nMouseClick_X_Coordinate, event->globalY() - m_nMouseClick_Y_Coordinate);
}

void script_window::prepare_window() {
    ui->textEdit->clear();
    ui->tabWidget->setVisible(false);
    ui->label->setText("Успешно");
    ui->label->setVisible(false);
    QPalette palette = ui->label->palette();
    palette.setColor(ui->label->foregroundRole(), Qt::green);

    errors.clear();
    dir.clear();

    script_window::show_rows();
}

void script_window::saveFile() {
    dir = QFileDialog::getSaveFileName(0, "Сохранить файл", dir, "*.sql *.txt");
    if (!dir.isEmpty()) {
        QFile file(dir);
        if (file.open(QIODevice::ReadWrite)) {
            QTextStream stream(&file);
            stream << ui->textEdit->toPlainText();
            file.flush();
            file.close();
        } else {
            QMessageBox::critical(this, "Ошибка записи", "Ошибка записи файла");
        }
    }
}

void script_window::on_toolButton_create_clicked() {
    if (!ui->textEdit->document()->isEmpty()) {
        QMessageBox::StandardButton reply;
        reply = QMessageBox::question(this, "Предупреждение", "Сохранить изменения?",
                                      QMessageBox::Yes | QMessageBox::No);
        if (reply == QMessageBox::Yes)
            saveFile();

        ui->textEdit->clear();
    }
}

void script_window::resizeEvent(QResizeEvent *event) {
    QSize s = event->size();
    this->resize(s);
    ui->tabWidget->resize(s);
    if (ui->tabWidget->count() < 1) {}
    else {
        for (int i = 0; i < ui->tabWidget->count(); i++)
            if (i != ui->tabWidget->currentIndex())
                ui->tabWidget->widget(i)->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);

        ui->tabWidget->widget(ui->tabWidget->currentIndex())->setSizePolicy(QSizePolicy::Preferred,
                                                                            QSizePolicy::Preferred);
        ui->tabWidget->currentWidget()->resize(ui->tabWidget->size());
        ui->tabWidget->widget(ui->tabWidget->currentIndex())->adjustSize();
    }
}

void script_window::on_toolButton_open_clicked() {
    ui->tabWidget->clear();
    ui->tabWidget->setVisible(false);
    dir = QFileDialog::getOpenFileName(0, "Открыть файл", dir, "*.sql *.txt");
    if (!dir.isEmpty()) {
        QFile file(dir);
        if (file.open(QIODevice::ReadOnly)) {
            QTextStream stream(&file);
            ui->textEdit->document()->setPlainText(stream.readAll());
            //ui->textEdit->setText(stream.readAll());
            this->setWindowTitle(dir);
            file.flush();
            file.close();

            show_rows();
        } else {
            QMessageBox::critical(this, "Ошибка чтения", "Ошибка чтения файла");
        }
    }
}

void script_window::on_toolButton_save_clicked() {
    saveFile();
}

void script_window::on_toolButton_runscript_clicked() {
    for (int i = 0; i < ui->tabWidget->count(); ++i) {
        ui->tabWidget->removeTab(i);
    }
    QSqlDatabase db = config::set_current_db();

    if (db.open()) {
        QSqlQuery q;
        QString query_str = ui->textEdit->document()->toPlainText();
        query_str.remove("\n");
        QStringList query_list = query_str.split(";");
        query_list.removeLast();
        int curr_line = 1;
        foreach(QString
        s, query_list)
        {
            if (s.contains("select", Qt::CaseInsensitive)) {
                q.clear();
                if (q.exec(s)) {
                    QWidget *new_widget = new QWidget(ui->tabWidget);
                    QSqlQueryModel *model = new QSqlQueryModel;
                    model->setQuery(q);
                    QTableView *new_table = new QTableView(new_widget);
                    new_table->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
                    new_table->setModel(model);

                    ui->tabWidget->addTab(new_widget,
                                          s.mid(s.indexOf("from ", Qt::CaseInsensitive) + QString("from ").count()));
                } else
                    errors.append("Line " + QString::number(curr_line) + ": " + q.lastError().text());

                ui->tabWidget->setVisible(true);
            } else {
                q.clear();
                if (!q.exec(s))
                    errors.append(q.lastError().text());
            }

            ui->label->setVisible(true);
            curr_line++;
        }

        if (!errors.isEmpty()) {
            QVBoxLayout *l = new QVBoxLayout;
            QWidget *err_widget = new QWidget(ui->tabWidget);
            err_widget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
            err_widget->setLayout(l);
            QListView *list_view = new QListView(err_widget);
            list_view->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
            QStringListModel *err_model = new QStringListModel;
            err_model->setStringList(errors);

            err_widget->resize(ui->tabWidget->size());
            list_view->resize(ui->tabWidget->size());
            list_view->setModel(err_model);
            ui->tabWidget->addTab(err_widget, "Ошибки");
        }
    }
    QSqlDatabase::removeDatabase(config::curr_database_name);
}

void script_window::closeEvent(QCloseEvent *event) {
    QFile file(dir);
    if (file.open(QIODevice::ReadWrite)) {
        QTextStream stream(&file);
        stream << ui->textEdit->toPlainText();
        file.flush();
        file.close();
    }
    prepare_window();
    emit closed();
    event->accept();
}


void script_window::show_rows() {
    ui->textEdit_rows->clear();
    script_window::rows = ui->textEdit->document()->lineCount();

    QStringList list_rows;
    for (int i = 1; i <= script_window::rows; i++) {
        list_rows.append(QString::number(i));
        ui->textEdit_rows->append(QString::number(i));
    }
}

void script_window::on_textEdit_textChanged() {
    ui->label->setVisible(false);

    show_rows();
}

void script_window::sliderMoved(int a) {
    ui->textEdit_rows->verticalScrollBar()->setValue(a);
}

void script_window::on_textEdit_cursorPositionChanged() {
    QList <QTextEdit::ExtraSelection> extraSelections_rows;

    QTextEdit::ExtraSelection selection;

    QColor lineColor = QColor(Qt::yellow).lighter(160);

    selection.format.setBackground(lineColor);
    selection.format.setProperty(QTextFormat::FullWidthSelection, true);
    selection.cursor = ui->textEdit->textCursor();
    selection.cursor.clearSelection();
    extraSelections_rows.append(selection);

    ui->textEdit->setExtraSelections(extraSelections_rows);
}

void script_window::on_toolButton_lightmode_clicked() {
    //lightmode 0 - темная тема; 1 - светлая

    if (config::user.lightmode == 0) {
        QFile styleF;
        styleF.setFileName(":/light.css");
        styleF.open(QFile::ReadOnly);
        QString qssStr = styleF.readAll();
        config::user.lightmode = 1;
        qApp->setStyleSheet(qssStr);
    } else {
        QFile styleF;
        styleF.setFileName(":/dark.css");
        styleF.open(QFile::ReadOnly);
        QString qssStr = styleF.readAll();
        config::user.lightmode = 0;
        qApp->setStyleSheet(qssStr);
    }
}

void script_window::on_pushButton_clicked() {
    this->close();
}
