#ifndef CALCULATOR_H
#define CALCULATOR_H

#include <QMainWindow>
#include <ui_calculator.h>
#include <QtWidgets/QLineEdit>

QT_BEGIN_NAMESPACE
namespace Ui {
    class MainWindow;
}
QT_END_NAMESPACE

class CalculatorMainWindow final : public QMainWindow {
    Q_OBJECT

public:
    explicit CalculatorMainWindow(QWidget *parent = nullptr);

    ~CalculatorMainWindow() override;

private:
    Ui::MainWindow* ui;

private slots:
    void on_pushButton_AC_clicked();


    /*
    void on_pushButton_C_clicked();
    void on_pushButton_BS_clicked();
    void on_pushButton_percent_clicked();
    void on_pushButton_divide_clicked();
    void on_pushButton_multiply_clicked();
    void on_pushButton_minus_clicked();
    void on_pushButton_plus_clicked();
    void on_pushButton_enter_clicked();
    void on_pushButton_sign_clicked();
    void on_pushButton_dot_clicked();
*/

/*
    void on_lineInput_cursorPositionChanged(int arg1, int arg2);
    void on_lineInput_editingFinished();
    void on_lineInput_inputRejected();
    void on_lineInput_returnPressed();
    void on_lineInput_selectionChanged();
    void on_lineInput_textEdited(const QString &arg1);
    void on_lineInput_textChanged(const QString &arg1);
*/

};

#endif // CALCULATOR_H
