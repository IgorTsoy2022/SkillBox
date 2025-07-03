#include <QApplication>

#include <QDir>
#include <QDebug>

#include <QSlider>
#include <QSpinBox>

#include <QPushButton>
#include <QPixmap>
#include <QPainter>
#include <QPaintEvent>
#include <QTimer>

#include <iostream>

class ImageButton : public QPushButton {
    Q_OBJECT
public:
    ImageButton() = default;
    explicit ImageButton(QWidget *parent);
    void paintEvent(QPaintEvent *e) override;
    [[nodiscard]] QSize sizeHint() const override;
    [[nodiscard]] QSize minimumSizeHint() const override;
    void keyPressEvent(QKeyEvent *e) override;
public slots:
    void setUp();
    void setDown();
private:
    QPixmap mCurrentButtonPixmap;
    QPixmap mButtonDownPixmap;
    QPixmap mButtonUpPixmap;
    bool isDown = false;
};


ImageButton::ImageButton(QWidget *parent) {
    setParent(parent);
    setToolTip("Stop");
    setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);

    mButtonUpPixmap = QPixmap(":/images/button_up.png");
    mButtonDownPixmap = QPixmap(":/images/button_down.png");
    mCurrentButtonPixmap = mButtonUpPixmap;
    setGeometry(mCurrentButtonPixmap.rect());
    connect(this, &QPushButton::clicked, this, &ImageButton::setDown);
}

void ImageButton::paintEvent(QPaintEvent *e) {
    QPainter p(this);
    p.drawPixmap(e->rect(), mCurrentButtonPixmap);
}

QSize ImageButton::sizeHint() const {
    return QSize(100, 100);
}

QSize ImageButton::minimumSizeHint() const {
    return sizeHint();
}

void ImageButton::keyPressEvent(QKeyEvent *e) {
    setDown();
}

void ImageButton::setDown() {
    mCurrentButtonPixmap = mButtonDownPixmap;
    update();
    QTimer::singleShot(100, this, &ImageButton::setUp);
}

void ImageButton::setUp() {
    mCurrentButtonPixmap = mButtonUpPixmap;
    update();
}

int main(int argc, char *argv[]) {

    QApplication app(argc, argv);

    const QDir currentDir;
    qDebug() << currentDir.absolutePath();

    auto *slider = new QSlider(Qt::Horizontal);
    auto *spinbox = new QSpinBox();

    slider->setMinimum(0);
    slider->setMaximum(100);
    spinbox->setMinimum(0);
    spinbox->setMaximum(100);

    QObject::connect(slider, &QSlider::valueChanged, spinbox, &QSpinBox::setValue);
    QObject::connect(spinbox, QOverload<int>::of(&QSpinBox::valueChanged), slider, &QSlider::setValue);

    slider->resize(200, 50);
    spinbox->resize(100, 50);

    slider->move(800, 500);
    spinbox->move(1200, 500);

    slider->show();
    spinbox->show();

    auto *button = new ImageButton(nullptr);
    button->setFixedSize(360, 120);
    button->move(1000, 400);
    QObject::connect(button, &QPushButton::clicked, [](){
        std::cout << "clicked\n";
    });

    button->show();

    const auto exec = QApplication::exec();

    delete slider;
    delete spinbox;
    delete button;

    return exec;
}

#include <QtM36.moc>
