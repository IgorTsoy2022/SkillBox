#ifndef CALCULATOR_H
#define CALCULATOR_H

#include <QMainWindow>
#include <QtWidgets/QLineEdit>

QT_BEGIN_NAMESPACE
namespace Ui {
    class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_pushButton_AC_clicked();
    void on_pushButton_BS_clicked();
    void on_pushButton_percent_clicked();
    void on_pushButton_divide_clicked();
    void on_pushButton_multiply_clicked();
    void on_pushButton_minus_clicked();
    void on_pushButton_plus_clicked();
    void on_pushButton_equal_clicked();
    void on_pushButton_sign_clicked();
    void on_pushButton_dot_clicked();
    void on_pushButton_0_clicked();
    void on_pushButton_1_clicked();
    void on_pushButton_2_clicked();
    void on_pushButton_3_clicked();
    void on_pushButton_4_clicked();
    void on_pushButton_5_clicked();
    void on_pushButton_6_clicked();
    void on_pushButton_7_clicked();
    void on_pushButton_8_clicked();
    void on_pushButton_9_clicked();

    void on_lineInput_cursorPositionChanged(int arg1, int arg2);
    void on_lineInput_editingFinished();
    void on_lineInput_inputRejected();
    void on_lineInput_returnPressed();
    void on_lineInput_selectionChanged();
    void on_lineInput_textEdited(const QString &arg1);
    void on_lineInput_textChanged(const QString &arg1);


private:
    Ui::MainWindow *ui;
};

#endif // CALCULATOR_H
