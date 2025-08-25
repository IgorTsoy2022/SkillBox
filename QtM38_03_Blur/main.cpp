#include "mainwindow.h"

#include <QApplication>

int main(int argc, char* argv[])
{
    QApplication application(argc, argv);

    MainWindow wnd;
    wnd.show();

    return QApplication::exec();
}