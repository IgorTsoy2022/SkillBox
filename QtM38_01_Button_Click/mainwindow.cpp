#include "mainwindow.h"

#include <QVBoxLayout>
#include <QPixmap>
#include <QMediaPlayer>
#include <QAudioOutput>
#include <QSoundEffect>
#include <QFile>

#include <QGraphicsOpacityEffect>
#include <QPropertyAnimation>
#include <QEasingCurve>


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    // Container
    auto *container = new QWidget(this);
    setCentralWidget(container);
    auto *layout = new QVBoxLayout(container);
    layout->setAlignment(Qt::AlignCenter);

    const auto pixmap = QPixmap(":/images/sphere_red.png");
    qDebug() << "Pixmap loaded:" << !pixmap.isNull();

    // Button
    button = new QPushButton(container);
    button->setFixedSize(pixmap.size());
    button->setText("");
    button->setMask(pixmap.mask());

    button->setStyleSheet(
        "QPushButton { "
            "border: none;"
            "background-image: url(:/images/sphere_red.png);"
            "background-repeat: no-repeat;"
            "background-position: center;"
        " } "
        "QPushButton:pressed { "
            "border: none;"
            "background-image: url(:/images/sphere_red_pressed.png);"
            "background-repeat: no-repeat;"
            "background-position: center;"
        " }"
    );

    layout->addWidget(button);

    auto *clickSound = new QSoundEffect(this);
    clickSound->setSource(QUrl("qrc:/sounds/click.wav"));
    clickSound->setVolume(0.8f);

    connect(button, &QPushButton::clicked, this,
            [=] {
        clickSound->play();
    });
}