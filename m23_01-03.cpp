#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <map>
#include <set>

template<typename A>
void print(const A& arr, size_t from, size_t to) {
    std::cout << "[";
    if (from > to) {
        for (size_t i = from; i > to; --i) {
            std::cout << arr[i] << " ";
        }
    }
    else {
        for (size_t i = from; i < to; ++i) {
            std::cout << arr[i] << " ";
        }
    }
    std::cout << arr[to] << "]\n";
}

template<typename M>
void print(const M& container) {
    for (const auto& [key, value] : container) {
        std::cout << key << " " << value << "\n";
    }
}

std::string toupper(const std::string& text) {
    std::string result = "";
    for (const auto& c : text) {
        if (c >= 'a' && c <= 'z') {
            result += (c - 32);
        }
        else {
            result += c;
        }
    }
    return result;
}

std::string tolower(std::string& text) {
    std::string result = "";
    for (const auto& c : text) {
        if (c >= 'A' && c <= 'Z') {
            result += (c + 32);
        }
        else {
            result += c;
        }
    }
    return result;
}

bool is_number(const std::string str) {
    if (str.size() < 1) {
        return false;
    }
    for (const auto& c : str) {
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
    if (month < 1 || month > 12 || day < 1) {
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

// Task 1. .


// Task 3. .



int main() {

    std::cout << "Task 1. Days of week.\n";
    {
        std::map<long long, std::string> phonebook;
        std::string input = "";
        std::cout << "1. Entering a phone number and surname will add the\n"
                  << "   entry to the directory.\n";
        std::cout << "2. Phone number query will output the surname of the\n"
                  << "   subscriber.\n";
        std::cout << "3. Surname query will output phone numbers of subscribers\n"
                  << "   with the specified surname.\n";
        std::cout << "4. Entry \"exit\" for quit.\n";
        while (std::getline(std::cin, input)) {
            if (input == "exit") {
                break;
            }
        }
    }

    std::cout << "\nTask 2. Registry.\n";
    {
    }

    std::cout << "\nTask 3. Anagrams.\n";
    {
    }

    return 0;
}