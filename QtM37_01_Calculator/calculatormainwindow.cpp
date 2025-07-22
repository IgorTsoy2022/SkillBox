#include "calculatormainwindow.h"
#include "./ui_calculator.h"
#include <QDebug>
#include <QMessageBox>


CalculatorMainWindow::CalculatorMainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
 /*
    connect(ui->pushButton_plus, SIGNAL(clicked()), this, SLOT(arithmetic_operations()));
    connect(ui->pushButton_minus, SIGNAL(clicked()), this, SLOT(arithmetic_operations()));
    connect(ui->pushButton_multiply, SIGNAL(clicked()), this, SLOT(arithmetic_operations()));
    connect(ui->pushButton_divide, SIGNAL(clicked()), this, SLOT(arithmetic_operations()));
*/

}

CalculatorMainWindow::~CalculatorMainWindow() {
    delete ui;
}


void CalculatorMainWindow::on_pushButton_AC_clicked() {
    ui->lineEdit_Input->setText("0");
    QMessageBox::information(this, "Welcome!", "You have successfully logged in.");
}

/*
void CalculatorMainWindow::on_pushButton_C_clicked() {
    QMessageBox::information(this, "Welcome!", "You have successfully logged in.");
}

void CalculatorMainWindow::on_pushButton_BS_clicked() {

}

void CalculatorMainWindow::on_pushButton_percent_clicked() {

}

void CalculatorMainWindow::on_pushButton_divide_clicked() {

}

void CalculatorMainWindow::on_pushButton_multiply_clicked() {

}

void CalculatorMainWindow::on_pushButton_minus_clicked() {

}

void CalculatorMainWindow::on_pushButton_plus_clicked() {

}

void CalculatorMainWindow::on_pushButton_enter_clicked() {

}

void CalculatorMainWindow::on_pushButton_sign_clicked() {

}

void CalculatorMainWindow::on_pushButton_dot_clicked() {

}
*/
/*
void CalculatorMainWindow::on_lineInput_cursorPositionChanged(int arg1, int arg2) {
    qDebug() << "arg1 = " << arg1 << ", arg2 = " << arg2;
}

void CalculatorMainWindow::on_lineInput_editingFinished() {

}

void CalculatorMainWindow::on_lineInput_inputRejected() {

}

void CalculatorMainWindow::on_lineInput_returnPressed() {

}

void CalculatorMainWindow::on_lineInput_selectionChanged() {
    qDebug() << "on_lineInput_selectionChanged";
}

void CalculatorMainWindow::on_lineInput_textEdited(const QString &arg1) {
    qDebug() << "on_lineInput_textEdited arg1 = " << arg1;
}

void CalculatorMainWindow::on_lineInput_textChanged(const QString &arg1) {
    qDebug() << "on_lineInput_textChanged arg1 = " << arg1;
}
*/