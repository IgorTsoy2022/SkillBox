#ifndef CALCULATOR_H
#define CALCULATOR_H

#include <QMainWindow>
#include <QtWidgets/QLineEdit>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr)
        : QMainWindow(parent)
    {}

    ~MainWindow();

private:
    Ui::MainWindow *ui;
};
#endif // CALCULATOR_H
