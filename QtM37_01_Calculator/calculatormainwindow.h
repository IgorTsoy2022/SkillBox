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

    Q_PROPERTY(int previous_position_ current_position_ READ getPosition WRITE setPosition)
public:
    explicit CalculatorMainWindow(QWidget *parent = nullptr);

    [[nodiscard]] int getPosition(const int num) const;
    void setPosition(const int num, const int position);

    ~CalculatorMainWindow() override;

private:
    Ui::MainWindow* ui;

    int previous_position_ = 0;
    int current_position_ = 0;

private slots:

    void digits_numbers() const;

    void on_pushButton_AC_clicked();

    static void on_lineEdit_Input_cursorPositionChanged(int arg1, int arg2);
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

    void on_lineEdit_Input_editingFinished();
    void on_lineEdit_Input_inputRejected();
    void on_lineEdit_Input_returnPressed();
    void on_lineEdit_Input_selectionChanged();
    void on_lineEdit_Input_textEdited(const QString &arg1);
    void on_lineEdit_Input_textChanged(const QString &arg1);
*/

};

#endif // CALCULATOR_H
