#include <iostream>
#include <fstream>
#include <vector>
#include <unordered_map>

int leap_year(int year) {
    if (year % 400 == 0) {
        return 1;
    }
    if (year % 100 == 0) {
        return 0;
    }
    if (year % 4 == 0) {
        return 1;
    }
    return 0;
}

bool is_date(int year, int month, int day) {
    if (month < 1 || month > 12 ||
         day < 1) {
        return false;
    }
    switch (month) {
    case 1:
    case 3:
    case 5:
    case 7:
    case 8:
    case 10:
    case 12:
        return day < 32;
    case 2:
        return day < 29 +
        leap_year(year);
    default:
        return day < 31;
    }
}

bool is_number(const std::string str) {
    for (const auto c : str) {
        if (c >= '0' && c <= '9') {
            continue;
        }
        if (c == '.') {
            continue;
        }
        return false;
    }
    return true;
}

int main() {
    std::cout << std::boolalpha;
    std::cout << is_number(".56712g3") << "\n";
    std::cout << std::stod(".6") << "\n";      return 0;
}