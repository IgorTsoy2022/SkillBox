#pragma once

#include <iostream>
#include <string>

namespace gpu {
    void display(const char& value);
    void display(const int& value);

    void output_line(const std::string& text);

    void prompt(const std::string& text);
} // namespace gpu