#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QPixmap>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::on_horizontalSlider_valueChanged(int value) {
    QPixmap pix;
    int width = ui->image->width();
    int height = ui->image->height();

    if (value < 34) {
        pix = QPixmap(":/images/sphere_green.png");
    }
    else if (value < 67) {
        pix = QPixmap(":/images/sphere_yellow.png");
    }
    else {
        pix = QPixmap(":/images/sphere_red.png");
    }

    ui->image->setPixmap(pix.scaled(width, height, Qt::KeepAspectRatio));
}
