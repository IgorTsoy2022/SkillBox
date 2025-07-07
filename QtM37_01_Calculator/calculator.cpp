#include "calculator.h"
#include "./ui_calculator.h"
#include <QDebug>
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::on_pushButton_AC_clicked() {
    QMessageBox::information(this, "Welcome!", "You have successfully logged in.");
}

void MainWindow::on_pushButton_BS_clicked() {

}

void MainWindow::on_pushButton_percent_clicked() {

}

void MainWindow::on_pushButton_divide_clicked() {

}

void MainWindow::on_pushButton_multiply_clicked() {

}

void MainWindow::on_pushButton_minus_clicked() {

}

void MainWindow::on_pushButton_plus_clicked() {

}

void MainWindow::on_pushButton_equal_clicked() {

}

void MainWindow::on_pushButton_sign_clicked() {

}

void MainWindow::on_pushButton_dot_clicked() {

}

void MainWindow::on_pushButton_0_clicked() {

}

void MainWindow::on_pushButton_1_clicked() {}
void MainWindow::on_pushButton_2_clicked() {}
void MainWindow::on_pushButton_3_clicked() {}
void MainWindow::on_pushButton_4_clicked() {}
void MainWindow::on_pushButton_5_clicked() {}
void MainWindow::on_pushButton_6_clicked() {}
void MainWindow::on_pushButton_7_clicked() {}
void MainWindow::on_pushButton_8_clicked() {}
void MainWindow::on_pushButton_9_clicked() {}

void MainWindow::on_lineInput_cursorPositionChanged(int arg1, int arg2) {
    qDebug() << "arg1 = " << arg1 << ", arg2 = " << arg2;
}

void MainWindow::on_lineInput_editingFinished() {

}

void MainWindow::on_lineInput_inputRejected() {

}

void MainWindow::on_lineInput_returnPressed() {

}

void MainWindow::on_lineInput_selectionChanged() {
    qDebug() << "on_lineInput_selectionChanged";
}

void MainWindow::on_lineInput_textEdited(const QString &arg1) {
    qDebug() << "on_lineInput_textEdited arg1 = " << arg1;
}

void MainWindow::on_lineInput_textChanged(const QString &arg1) {
    qDebug() << "on_lineInput_textChanged arg1 = " << arg1;
}