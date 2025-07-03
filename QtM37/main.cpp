#include <QApplication>
#include "./ui_caller.h"
#include "callermainwindow.h"

int main(int argc, char* argv[])
{
    QApplication a(argc, argv);
    CallerMainWindow window(nullptr);
    Ui::MainWindow caller;
    caller.setupUi(&window);
    window.lineEdit = caller.lineEdit;
    window.resize(480, 640);
    window.show();
    return QApplication::exec();
}