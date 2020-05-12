//виджет, которым заполняется каждая вкладка окна MainWindow
#include "script_window.h"
#include "ui_script_window.h"

script_window::script_window(QWidget *parent) :
        QWidget(parent),
        ui(new Ui::script_window) {
    ui->setupUi(this);
    this->setWindowFlag(Qt::FramelessWindowHint);

    //применение стилей
    if (config::user.lightmode == 1)
        ui->textEdit_rows->setStyleSheet("QTextEdit: {background: white;}");
    else
        ui->textEdit_rows->setStyleSheet("QTextEdit: {background: #404040;}");

    //разделение окна
    ui->splitter->setHandleWidth(1);
    ui->splitter->setStretchFactor(0, 1);
    ui->splitter->setStretchFactor(1, 15);
    ui->textEdit_rows->verticalScrollBar()->setDisabled(true); //отключения скроллбара
    connect(ui->textEdit->verticalScrollBar(), SIGNAL(valueChanged(int)), this, SLOT(setV())); //обработка сигнала прокрутки скроллбара текстовго поля

    ui->textEdit->append("");   //очистка текстового поля
    show_rows();    //отобразить номера строк
}

script_window::~script_window() {
    delete ui;
}

//отобразить номера строк
void script_window::show_rows() {
    ui->textEdit_rows->clear();
    script_window::rows = ui->textEdit->document()->lineCount();    //количество строк текущего текста

    for (int i = 1; i <= script_window::rows; i++) {
        ui->textEdit_rows->append(QString::number(i));  //заполнение номерами строк
    }

    //установить позицию скроллбара, как у основного текстового поля
    ui->textEdit_rows->verticalScrollBar()->setValue(ui->textEdit->verticalScrollBar()->value());
}

//подсветка текущей строки
void script_window::on_textEdit_cursorPositionChanged() {
    QList <QTextEdit::ExtraSelection> extraSelections_rows; //список выделенных строк

    QTextEdit::ExtraSelection selection;    //конкретная выделенная строка

    QColor lineColor = QColor(Qt::yellow).lighter(160); //цвет заднего фона
    QColor textColor = QColor(Qt::black);   //цвет текста

    //применение изменений
    selection.format.setForeground(textColor);
    selection.format.setBackground(lineColor);
    selection.format.setProperty(QTextFormat::FullWidthSelection, true);
    selection.cursor = ui->textEdit->textCursor();
    selection.cursor.clearSelection();
    extraSelections_rows.append(selection);

    ui->textEdit->setExtraSelections(extraSelections_rows);
    //обновить список строк
    show_rows();
}

//установка скроллбара строк
void script_window::setV() {
    ui->textEdit_rows->verticalScrollBar()->setValue(ui->textEdit->verticalScrollBar()->value());

}
