#include <QApplication>
//#include "./ui_calculator.h"
#include "calculator.h"




int main(int argc, char* argv[])
{
    QApplication a(argc, argv);
    MainWindow window;
//    QMainWindow window(nullptr);
//    Ui::MainWindow calculator;
//    calculator.setupUi(&window);
//    window.lineEdit = calculator.lineEdit;
    window.resize(480, 640);
    window.show();
    return QApplication::exec();
}