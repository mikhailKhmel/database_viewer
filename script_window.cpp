#include "script_window.h"
#include "ui_script_window.h"

script_window::script_window(QWidget *parent) :
        QWidget(parent),
        ui(new Ui::script_window) {
    ui->setupUi(this);
    this->setWindowFlag(Qt::FramelessWindowHint);

    if (config::user.lightmode == 1)
        ui->textEdit_rows->setStyleSheet("background: #404040");
    else
        ui->textEdit_rows->setStyleSheet("background: white");

    ui->splitter->setHandleWidth(1);
    ui->splitter->setStretchFactor(0, 1);
    ui->splitter->setStretchFactor(1, 15);
    ui->textEdit_rows->verticalScrollBar()->setDisabled(true);
    connect(ui->textEdit->verticalScrollBar(), SIGNAL(valueChanged(int)), this, SLOT(setV()));

    ui->textEdit->append("");
    show_rows();
}

script_window::~script_window() {
    delete ui;
}

void script_window::show_rows() {
    ui->textEdit_rows->clear();
    script_window::rows = ui->textEdit->document()->lineCount();

    for (int i = 1; i <= script_window::rows; i++) {
        ui->textEdit_rows->append(QString::number(i));
    }

    ui->textEdit_rows->verticalScrollBar()->setValue(ui->textEdit->verticalScrollBar()->value());
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
    show_rows();
}

void script_window::setV() {
    ui->textEdit_rows->verticalScrollBar()->setValue(ui->textEdit->verticalScrollBar()->value());

}
