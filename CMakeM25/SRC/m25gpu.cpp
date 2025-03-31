#include "m25gpu.h"

namespace gpu {
    void display(const char& value) {
        std::cout << value;
    }
    void display(const int& value) {
        std::cout << value;
    }

    void output_line(const std::string& text) {
        std::cout << text << std::endl;
    }

    void prompt(const std::string& text) {
        std::cout << text;
    }
} // namespace gpu