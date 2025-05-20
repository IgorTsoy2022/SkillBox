#define _USE_MATH_DEFINES
#include <iostream>
#include <cmath>
#include <unordered_set>

struct Box {
    double width = 0.0;
    double height = 0.0;

    Box() {};

    Box(const double w, const double h)
        : width(w)
        , height(h)
    {};

    Box operator=(const Box& right) {
        width = right.width;
        height = right.height;
        return *this;
    }
};

class Shape {
public:
    Shape() {};

    Shape(const Box& frame, const std::string& type)
        : frame_(frame)
        , type_(type)
    {};

    virtual const double square() const = 0;
    virtual const Box& dimensions() const = 0;
    virtual const std::string_view type() const = 0;

    virtual ~Shape() {
        std::cout << "~Shape;\n";
    };
protected:
    Box frame_{};
    std::string type_ = "unknown";
};

class Circle : public Shape {
public:
    Circle() {};

    Circle(const double radius, const std::string& type = "Circle")
        : Shape(Box(2 * radius, 2 * radius), type)
        , radius_(radius)
    {};

    void set_radius(const double radius) {
        radius_ = radius;
        frame_ = Box(2 * radius_, 2 * radius_);
    }

    const double square() const override {
        return M_PI * radius_ * radius_;
    }

    const Box& dimensions() const override {
        return frame_;
    }

    const std::string_view type() const override {
        return type_;
    }

    ~Circle() {
        std::cout << "~Circle;\n";
    };
private:
    double radius_ = 0;
};

class Rectangle : public Shape {
public:
    Rectangle() {};

    Rectangle(const Box& size, const std::string& type = "Rectangle")
        : Shape(size, type)
        , size_(size)
    {};

    void set_size(const Box& size) {
        size_ = size;
        frame_ = size_;
    }

    const double square() const override {
        return size_.width * size_.height;
    }

    const Box& dimensions() const override {
        return frame_;
    }

    const std::string_view type() const override {
        return type_;
    }

    ~Rectangle() {
        std::cout << "~Rectangle;\n";
    }
private:
    Box size_{};
};

class Triangle : public Shape {
public:
    Triangle() {};

    Triangle(const double a, const double b, const double c,
             const std::string& type = "Triangle")
        : Shape(frame_size(a, b, c), type)
        , a_(a)
        , b_(b)
        , c_(c)
    {};

    void set_size(const double a, const double b, const double c) {
        a_ = a;
        b_ = b;
        c_ = c;
        frame_ = frame_size(a, b, c);
    }

    const double square() const override {
        auto semi_perimeter = (a_ + b_ + c_) / 2.0;
        return std::sqrt(semi_perimeter * (semi_perimeter - a_)
               * (semi_perimeter - b_) * (semi_perimeter - c_));
    }

    const Box& dimensions() const override {
        return frame_;
    }

    const std::string_view type() const override {
        return type_;
    }

    ~Triangle() {
        std::cout << "~Triangle; \n";
    }
private:
    double a_ = 0.0;
    double b_ = 0.0;
    double c_ = 0.0;
    const Box frame_size(const double a, const double b, const double c) const {
        auto semi_perimeter = (a + b + c) / 2.0;
        auto triangle_area = std::sqrt(semi_perimeter * (semi_perimeter - a) *
                                      (semi_perimeter - b) * (semi_perimeter - c));
        auto circumferential_radius = (a * b * c) / (4 * triangle_area);
        if (triangle_area > 0) {
            return Box(2 * circumferential_radius, 2 * circumferential_radius);
        }
        else {
            return Box{};
        }
    }
};

void printParams(Shape* shape) {
    std::cout << "Type:" << shape->type() << "\n";
    std::cout << "Square:" << shape->square() << "\n";
    std::cout << "Width:" << shape->dimensions().width << "\n";
    std::cout << "Height:" << shape->dimensions().height << "\n";
}

int main() {
    Shape* circle = new Circle(8);
    Shape* rectangle = new Rectangle(Box(5, 6));
    Shape* triangle = new Triangle(2, 3, 4);

    printParams(circle);
    printParams(rectangle);
    printParams(triangle);

    return 0;
}