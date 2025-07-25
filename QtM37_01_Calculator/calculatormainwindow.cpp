#include "calculatormainwindow.h"
#include "./ui_calculator.h"
#include <QDebug>
#include <QMessageBox>
#include <QLineEdit>

class LineEdit final : public QLineEdit {
public:
    explicit LineEdit(QWidget* parent=nullptr)
        : QLineEdit(parent)
    {
        connect(this, &QLineEdit::selectionChanged, [this]{
            if(selectionStart() > -1) {
                start_=selectionStart();
                length_=selectionLength();
            }
            if(out_) setSelection(start_, length_);
        });
    }

private:
    int start_{0}, length_{0};
    bool out_{false};

protected:
    void focusOutEvent(QFocusEvent *e) override{
        QLineEdit::focusOutEvent(e);
        out_ = true;
        setSelection(start_, length_);
    }

    void focusInEvent(QFocusEvent *e) override{
        QLineEdit::focusInEvent(e);
        out_ = false;
    }
};

CalculatorMainWindow::CalculatorMainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    connect(ui->pushButton_0, SIGNAL(clicked()), this, SLOT(digits()));
    connect(ui->pushButton_1, SIGNAL(clicked()), this, SLOT(digits()));
    connect(ui->pushButton_2, SIGNAL(clicked()), this, SLOT(digits()));
    connect(ui->pushButton_3, SIGNAL(clicked()), this, SLOT(digits()));
    connect(ui->pushButton_4, SIGNAL(clicked()), this, SLOT(digits()));
    connect(ui->pushButton_5, SIGNAL(clicked()), this, SLOT(digits()));
    connect(ui->pushButton_6, SIGNAL(clicked()), this, SLOT(digits()));
    connect(ui->pushButton_7, SIGNAL(clicked()), this, SLOT(digits()));
    connect(ui->pushButton_8, SIGNAL(clicked()), this, SLOT(digits()));
    connect(ui->pushButton_9, SIGNAL(clicked()), this, SLOT(digits()));

    connect(ui->pushButton_pi, SIGNAL(clicked()), this, SLOT(digits()));
    connect(ui->pushButton_e, SIGNAL(clicked()), this, SLOT(digits()));

    connect(ui->pushButton_comma, SIGNAL(clicked()), this, SLOT(digits()));
    connect(ui->pushButton_leftB, SIGNAL(clicked()), this, SLOT(digits()));
    connect(ui->pushButton_rightB, SIGNAL(clicked()), this, SLOT(digits()));
    connect(ui->pushButton_power, SIGNAL(clicked()), this, SLOT(digits()));
    connect(ui->pushButton_percent, SIGNAL(clicked()), this, SLOT(digits()));
    connect(ui->pushButton_divide, SIGNAL(clicked()), this, SLOT(digits()));
    connect(ui->pushButton_multiply, SIGNAL(clicked()), this, SLOT(digits()));
    connect(ui->pushButton_minus, SIGNAL(clicked()), this, SLOT(digits()));
    connect(ui->pushButton_plus, SIGNAL(clicked()), this, SLOT(digits()));
    connect(ui->pushButton_dot, SIGNAL(clicked()), this, SLOT(digits()));

    connect(ui->pushButton_sin, SIGNAL(clicked()), this, SLOT(functions()));
    connect(ui->pushButton_cos, SIGNAL(clicked()), this, SLOT(functions()));
    connect(ui->pushButton_tan, SIGNAL(clicked()), this, SLOT(functions()));
    connect(ui->pushButton_asin, SIGNAL(clicked()), this, SLOT(functions()));
    connect(ui->pushButton_acos, SIGNAL(clicked()), this, SLOT(functions()));
    connect(ui->pushButton_atan, SIGNAL(clicked()), this, SLOT(functions()));
    connect(ui->pushButton_mod, SIGNAL(clicked()), this, SLOT(functions()));
    connect(ui->pushButton_exp, SIGNAL(clicked()), this, SLOT(functions()));
    connect(ui->pushButton_log, SIGNAL(clicked()), this, SLOT(functions()));
    connect(ui->pushButton_ln, SIGNAL(clicked()), this, SLOT(functions()));

//    ui->pushButton_equal->setCheckable(true);
}

CalculatorMainWindow::~CalculatorMainWindow() {
    delete ui;
}

bool CalculatorMainWindow::getExecuted() const {
    return executed;
}

void CalculatorMainWindow::setExecuted(const bool current) {
    executed = current;
}

QString CalculatorMainWindow::getResult() const {
    return result;
}

void CalculatorMainWindow::setResult(const QString& current) {
    result = current;
}

void CalculatorMainWindow::digits() {
    const auto position = ui->lineEdit_Input->cursorPosition();
    const auto length = ui->lineEdit_Input->text().length();
    auto *button = dynamic_cast<QPushButton*>(sender());
    QString new_text;
    if (getExecuted()) {
        setExecuted(false);
        new_text = getResult();
    }
    else {
        new_text = ui->lineEdit_Input->text();
    }
    QString text = button->text();

    if (position < length) {
        new_text.insert(position, text);
    }
    else {
        new_text += text;
    }
    ui->lineEdit_Input->setText(new_text);
    ui->lineEdit_Input->setCursorPosition(position + text.length());

}

void CalculatorMainWindow::functions() {
    const auto position = ui->lineEdit_Input->cursorPosition();
    const auto length = ui->lineEdit_Input->text().length();
    auto *button = dynamic_cast<QPushButton*>(sender());
    QString new_text;
    if (getExecuted()) {
        setExecuted(false);
        new_text = getResult();
    }
    else {
        new_text = ui->lineEdit_Input->text();
    }
    QString text = button->text() + '(';

    if (position < length) {
        new_text.insert(position, text);
    }
    else {
        new_text += text;
    }
    ui->lineEdit_Input->setText(new_text);
    ui->lineEdit_Input->setCursorPosition(position + text.length());

}

void CalculatorMainWindow::on_pushButton_AC_clicked() {
    ui->lineEdit_Input->setText("");
    ui->textEdit_Result->setText("");
    setExecuted(false);
}

void CalculatorMainWindow::on_pushButton_C_clicked() const {
    const auto position = ui->lineEdit_Input->cursorPosition();
    QString new_text = ui->lineEdit_Input->text();

    new_text.remove(position, 1);
    ui->lineEdit_Input->setText(new_text);
    ui->lineEdit_Input->setCursorPosition(position);
    //    QMessageBox::information(this, "Welcome!", "You have successfully logged in.");
}

void CalculatorMainWindow::on_pushButton_BS_clicked() const {
    const auto position = ui->lineEdit_Input->cursorPosition() - 1;
    QString new_text = ui->lineEdit_Input->text();

    new_text.remove(position, 1);
    ui->lineEdit_Input->setText(new_text);
    ui->lineEdit_Input->setCursorPosition(position);
}

void CalculatorMainWindow::on_pushButton_equal_clicked() {
    if (getExecuted()) {
        return;
    }
    QString new_text = ui->lineEdit_Input->text();
    QString result;
    std::string text = new_text.toStdString();
    try {
        std::stringstream ss;
        ss << std::setprecision(25) << ReversePolishNotation<long double>::calculate(text);
        result = QString::fromStdString(ss.str());
        new_text += "=" + result;
        setResult(result);
    }
    catch (const std::invalid_argument& e) {
        result = QString::fromStdString(e.what());
        setResult("");
    }
    ui->lineEdit_Input->setText(new_text);
    ui->textEdit_Result->setText(result);
    setExecuted(true);
}