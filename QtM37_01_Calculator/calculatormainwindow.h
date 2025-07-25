#ifndef CALCULATOR_H
#define CALCULATOR_H

#include <QMainWindow>
#include <ui_calculator.h>
#include <QtWidgets/QLineEdit>
#include "ReversePolishNotation.h"

QT_BEGIN_NAMESPACE
namespace Ui {
    class MainWindow;
}
QT_END_NAMESPACE

class CalculatorMainWindow final : public QMainWindow {
    Q_OBJECT

    Q_PROPERTY(bool executed READ getExecuted WRITE setExecuted)
    Q_PROPERTY(QString result READ getResult WRITE setResult)

public:
    explicit CalculatorMainWindow(QWidget *parent = nullptr);

    bool getExecuted() const;
    void setExecuted(bool current);
    QString getResult() const;
    void setResult(const QString& current);

    ~CalculatorMainWindow() override;

private:
    Ui::MainWindow* ui;
    bool executed = false;
    QString result;

private slots:
    void digits();
    void functions();

    void on_pushButton_AC_clicked();
    void on_pushButton_C_clicked() const;
    void on_pushButton_BS_clicked() const;
    void on_pushButton_equal_clicked();

};

#endif // CALCULATOR_H
