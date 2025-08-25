#pragma once

#include <QMainWindow>

class QPlainTextEdit;
class QWebEngineView;
class QTimer;
class QPushButton;

class MainWindow final : public QMainWindow {
    Q_OBJECT
public:
    explicit MainWindow(QWidget *parent = nullptr);

private slots:
    void updatePreview() const;

private:
    QPlainTextEdit *editor;
    QWebEngineView *preview;
    QTimer *updateTimer;
    QPushButton *updateButton;
};
