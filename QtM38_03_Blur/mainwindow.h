#pragma once

#include <QMainWindow>

class QLabel;
class QSlider;
class QPushButton;
class QPixmap;
class QGraphicsBlurEffect;

class MainWindow final : public QMainWindow {
    Q_OBJECT
public:
    explicit MainWindow(QWidget *parent = nullptr);

protected:
    void resizeEvent(QResizeEvent *event) override;

private slots:
    void openImage();
    void saveImage();
    void updateImage() const;
    void applyBlur(int value);

private:
    QLabel *label;
    QSlider *slider;
    QPushButton *openFileButton;
    QPushButton *saveFileButton;

    QPixmap originalPixmap;

    int blurLevel = 0;
    static QImage makeBlurred(const QImage &image, qreal radius);
};