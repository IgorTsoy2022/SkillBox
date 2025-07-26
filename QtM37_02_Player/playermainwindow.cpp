#include "playermainwindow.h"

PlayerMainWindow::PlayerMainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    ui->pushButton_Rewind->setText(QString(QChar(0x000023EA)));
    ui->pushButton_Play->setText(QString(QChar(0x000023F5)));
    ui->pushButton_Pause->setText(QString(QChar(0x000023F8)));
    ui->pushButton_Stop->setText(QString(QChar(0x000023F9)));
    ui->pushButton_Forward->setText(QString(QChar(0x000023E9)));
}

PlayerMainWindow::~PlayerMainWindow() {
    delete ui;
}