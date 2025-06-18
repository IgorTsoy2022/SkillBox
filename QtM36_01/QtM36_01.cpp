#include <QApplication>

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
    ImageButton(QWidget *parent);
    void paintEvent(QPaintEvent *e) override;
    QSize sizeHint() const override;
    QSize minimumSizeHint() const override;
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

    mButtonUpPixmap = QPixmap(":img/button_up.png");
    if (!mButtonUpPixmap) {
        std::cout << "No image\n";
    }
    else {
        std::cout  << "w=" << mButtonUpPixmap.width() << " h=" << mButtonUpPixmap.height() << "\n";
    }
    mButtonDownPixmap = QPixmap("C:/CPP/GIT/SkillBox-main/QtM36_01/img/button_down.png");
    if (!mButtonDownPixmap) {
        std::cout << "No image\n";
    }
    else {
        std::cout  << "w=" << mButtonDownPixmap.width() << " h=" << mButtonDownPixmap.height() << "\n";
    }

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

    {
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
    }


    {
        ImageButton redButton(nullptr);
        std::cout << "button\n";
        redButton.setFixedSize(100, 100);
        redButton.move(1000, 400);

        QObject::connect(&redButton, &QPushButton::clicked, [](){
            std::cout << "clicked\n";
        });
        std::cout << "button\n";
        redButton.show();
        std::cout << "button show\n";
    }
    return app.exec();
}

#include <QtM36_01.moc>
