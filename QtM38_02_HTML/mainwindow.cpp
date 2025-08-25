#include "mainwindow.h"
#include <QPlainTextEdit>
#include <QWebEngineView>
#include <QSplitter>
#include <QVBoxLayout>
#include <QPushButton>
#include <QWidget>
#include <QTimer>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent) {
    auto *central = new QWidget(this);
    auto *layout = new QVBoxLayout(central);

    // Refresh button
    updateButton = new QPushButton("Refresh", this);
    layout->addWidget(updateButton);

    // Splitter
    auto *splitter = new QSplitter(this);

    editor = new QPlainTextEdit;
    preview = new QWebEngineView;

    splitter->addWidget(editor);
    splitter->addWidget(preview);
    splitter->setStretchFactor(0, 1);
    splitter->setStretchFactor(1, 1);

    layout->addWidget(splitter);
    setCentralWidget(central);

    // default HTML
    editor->setPlainText("<!DOCTYPE html><html><body><h1>Hello, Qt!</h1></body></html>");
    updatePreview();
    preview->setHtml(editor->toPlainText());

    updateTimer = new QTimer(this);
    updateTimer->setSingleShot(true);
    updateTimer->setInterval(300);

    connect(editor, &QPlainTextEdit::textChanged, this,
        [this] {
            updateTimer->start();
    });

    connect(updateTimer, &QTimer::timeout, this,
            &MainWindow::updatePreview);

    // Refresh button
    connect(updateButton, &QPushButton::clicked, this,
            &MainWindow::updatePreview);

    setWindowTitle("HTML Editor");
    resize(800, 600);
    splitter->setSizes({ width() / 2, width() / 2 });
}

void MainWindow::updatePreview() const {
    preview->setHtml(editor->toPlainText());
}
