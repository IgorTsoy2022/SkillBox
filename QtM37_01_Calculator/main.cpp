#include <QApplication>
#include "./ui_calculator.h"
#include "calculatormainwindow.h"

int main(int argc, char* argv[])
{
    QApplication a(argc, argv);
    CalculatorMainWindow window(nullptr);

    window.resize(480, 640);
    window.show();
    return QApplication::exec();
}