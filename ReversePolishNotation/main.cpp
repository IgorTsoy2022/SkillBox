#include <iomanip>
#include "ReversePolishNotation.h"

int main() {
    ReversePolishNotation<double> rpn;

    std::cout << std::setprecision(26) << std::boolalpha;

    float fPI_ = 3.141592653589793238462643;
    double dPI_ = 3.141592653589793238462643;
    long double lPI_ = 3.141592653589793238462643;
    std::cout << fPI_ << std::endl;
    std::cout << dPI_ << std::endl;
    std::cout << lPI_ << std::endl;

    std::string input = "";
    std::cout << "Calculator:\n";
    std::cout << "Input math expression (type \"exit\" for exit):\n";
    while (std::getline(std::cin, input)) {
        if (input == "exit") {
            break;
        }

        try {
            std::cout << "Postfix expression: \"" << rpn.to_postfix(input) << "\"" << std::endl;
            std::cout << "Result: " << rpn.calculate(input) << std::endl;
        }
        catch (const std::invalid_argument& e) {
            std::cerr << "Invalid argument: " << e.what() << std::endl;
        }
    }
}