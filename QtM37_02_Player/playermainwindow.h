#ifndef QTM37_02_PLAYER_PLAYERMAINWINDOW_H
#define QTM37_02_PLAYER_PLAYERMAINWINDOW_H

#include <QMainWindow>
#include "./ui_player.h"

QT_BEGIN_NAMESPACE
namespace Ui {
    class MainWindow;
}
QT_END_NAMESPACE

class PlayerMainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit PlayerMainWindow(QWidget* parent = nullptr);

    ~PlayerMainWindow() override;

private:
    Ui::MainWindow *ui{};
};

#endif //QTM37_02_PLAYER_PLAYERMAINWINDOW_H
