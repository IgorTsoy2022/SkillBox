#include "mainwindow.h"

#include <QApplication>
#include <QLabel>

int main(int argc, char* argv[]) {
    QApplication a(argc, argv);

    MainWindow wnd;
    wnd.show();

    return QApplication::exec();
}