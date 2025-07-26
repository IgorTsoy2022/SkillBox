#include <QApplication>
#include "playermainwindow.h"

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);
    PlayerMainWindow window(nullptr);

    window.resize(350, 300);
    window.show();
    return QApplication::exec();
}
