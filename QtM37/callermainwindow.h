//
// Created by User on 02.07.2025.
//

#ifndef CALLERMAINWINDOW_H
#define CALLERMAINWINDOW_H

#include <QMainWindow>
#include <QtWidgets/QLineEdit>

class CallerMainWindow : public QMainWindow {
    Q_OBJECT
public:
    QLineEdit* lineEdit = nullptr;
    explicit CallerMainWindow(QWidget *parent = nullptr)
        : QMainWindow(parent)
    {}

public slots:
    void add0() { lineEdit->setText(lineEdit->text() + "0"); };
    void add1() { lineEdit->setText(lineEdit->text() + "1"); };
    void add2() { lineEdit->setText(lineEdit->text() + "2"); };
    void add3() { lineEdit->setText(lineEdit->text() + "3"); };
    void add4() { lineEdit->setText(lineEdit->text() + "4"); };
    void add5() { lineEdit->setText(lineEdit->text() + "5"); };
    void add6() { lineEdit->setText(lineEdit->text() + "6"); };
    void add7() { lineEdit->setText(lineEdit->text() + "7"); };
    void add8() { lineEdit->setText(lineEdit->text() + "8"); };
    void add9() { lineEdit->setText(lineEdit->text() + "9"); };
    void call() { lineEdit->setText("Call"); };
};

#endif //CALLERMAINWINDOW_H
