#include "mainwindow.h"

#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QSlider>
#include <QPushButton>

#include <QFileDialog>
#include <QPixmap>
#include <QGraphicsBlurEffect>
#include <QGraphicsScene>
#include <QGraphicsPixmapItem>
#include <QGraphicsView>
#include <QPainter>


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    auto *centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);

    // Layout
    auto *vbox = new QVBoxLayout(centralWidget);

    // Label
    label = new QLabel("Empty", this);
    label->setAlignment(Qt::AlignCenter);
    label->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    // Slider
    slider = new QSlider(Qt::Horizontal, this);
    slider->setRange(0, 10);
    slider->setValue(0);
    slider->setPageStep(1);
    slider->setSingleStep(1);
    slider->setTracking(true);
    slider->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);

    // Button
    openFileButton = new QPushButton("Open file", this);
    saveFileButton = new QPushButton("Save", this);

    auto *hbox = new QHBoxLayout();
    hbox->addWidget(openFileButton);
    hbox->addWidget(saveFileButton);

    // Layout
    vbox->addWidget(label);
    vbox->addWidget(slider);
    vbox->addLayout(hbox);

    connect(openFileButton, &QPushButton::clicked, this,
        &MainWindow::openImage);

    connect(saveFileButton, &QPushButton::clicked, this,
            &MainWindow::saveImage);

    connect(slider, &QSlider::valueChanged, this,
        &MainWindow::applyBlur);

    resize(600, 600);
}

void MainWindow::resizeEvent(QResizeEvent *event) {
    QMainWindow::resizeEvent(event);
    updateImage();
}

void MainWindow::openImage() {
    const QString fileName = QFileDialog::getOpenFileName(
        this,
        "Open image",
        QString(),
        "Images: (*.png *.jpg *.jpeg *.bmp *.gif)"
    );

    if (!fileName.isEmpty()) {
        originalPixmap.load(fileName);
        updateImage();
    }
}

void MainWindow::saveImage() {
    if (originalPixmap.isNull())
        return;

    const QString fileName = QFileDialog::getSaveFileName(
        this,
        "Save image",
        QString(),
        "PNG (*.png);;JPEG (*.jpg *.jpeg);;BMP (*.bmp)"
    );

    if (!fileName.isEmpty()) {
        const QImage img = makeBlurred(originalPixmap.toImage(), blurLevel * 2.5);
        img.save(fileName);
    }
}

void MainWindow::updateImage() const {
    if (!originalPixmap.isNull()) {
        const QImage image = makeBlurred(originalPixmap.toImage(), blurLevel * 2.5);

        const QPixmap scaled = QPixmap::fromImage(image).scaled(
            label->size(),
            Qt::KeepAspectRatio,
            Qt::SmoothTransformation
        );

        label->setPixmap(scaled);
    }
}

void MainWindow::applyBlur(const int value) {
    blurLevel = value;
    updateImage();
}

QImage MainWindow::makeBlurred(const QImage &image, const qreal radius) {
    if (radius <= 0.1)
        return image;

    QGraphicsScene scene;
    QGraphicsPixmapItem item;
    item.setPixmap(QPixmap::fromImage(image));
    auto *blur = new QGraphicsBlurEffect;
    blur->setBlurRadius(radius);
    item.setGraphicsEffect(blur);
    scene.addItem(&item);

    QImage result(image.size(), QImage::Format_ARGB32);
    result.fill(Qt::transparent);

    QPainter painter(&result);
    scene.render(&painter, QRectF(),
           QRectF(0, 0, image.width(), image.height()));

    return result;
}