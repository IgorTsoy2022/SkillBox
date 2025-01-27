#include <iostream>
#include <cmath>

int main() {
    const float zero = 0.00001;
    float a, b, c;
    std::cout << "a, b, c:";
    std::cin >> a >> b >> c;

    if (std::abs(a) < zero) { 
        std::cout << "Not a quadratic equation!" << std::endl;
        std::cout << "Solution of a linear equation " << b << " * x"
                  << ((c > 0) ? " + " : " - ") << std::abs(c) << " = 0"
                  << " is x = " << -c/b << std::endl;
    }
    else {
        float discriminant = b * b - 4 * a * c;
        std::cout << "discriminant = " << discriminant << "\n";
        if (discriminant > 0) {
            float x1 = (-b + std::sqrt(discriminant)) / (2 * a);
            float x2 = (-b - std::sqrt(discriminant)) / (2 * a);
            std::cout  << "Root 1, 2: " << x1 << ", " << x2 << std::endl;
        }
        else if (std::abs(discriminant) < zero) {
            std::cout << "Root: " << -b / (2 * a) << std::endl;
        }
        else {
            std::cout << "Complex scenario is not supported!" << std::endl;
        }
    }

    return 0;
}