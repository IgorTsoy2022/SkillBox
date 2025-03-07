#include <cstdlib>
#include <ctime>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <unordered_map>

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

void toupper(std::string& text) {
    for (auto& c : text) {
        if (c >= 'a' && c <= 'z') {
            c = c - 32;
        }
    }
}

void tolower(std::string& text) {
    for (auto& c : text) {
        if (c >= 'A' && c <= 'Z') {
            c = c + 32;
        }
    }
}

bool is_number(const std::string str) {
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

struct date {
    int year = 0;
    int month = 0;
    int day = 0;
};

bool is_date(const date & d) {
    if (d.month < 1 || d.month > 12 ||
         d.day < 1) {
        return false;
    }
    switch (d.month) {
    case 1:
    case 3:
    case 5:
    case 7:
    case 8:
    case 10:
    case 12:
        return d.day < 32;
    case 2:
        return d.day < 29 +
        leap_year(d.year);
    default:
        return d.day < 31;
    }
}

std::vector<std::string> 
split(const std::string& str, const char delimeter = ' ') {
    std::vector<std::string> result;
    std::string word = "";
    for (const auto& c : str) {
        if (c == delimeter) {
            if (word.size() > 0) {
                result.push_back(word);
                word.clear();
            }
        }
        else {
            word += c;
        }
    }
    if (word.size() > 0) {
        result.push_back(word);
    }
    return result;
}

bool is_ddmmyyyy(const std::string& ddmmyyyy, const char delimeter,
                 date & d) {
    auto size = ddmmyyyy.size();
    if (size < 6) {
        return false;
    }
    auto nums = split(ddmmyyyy, delimeter);
    if (nums.size() != 3) {
        return false;
    }

    int year = 0, month = 0, day = 0;
    if (!is_number(nums[0])) {
       return false;
    }
    d.day = std::stoi(nums[0]);
    if (!is_number(nums[1])) {
        return false;
    }
    d.month = std::stoi(nums[1]);
    if (!is_number(nums[2])) {
        return false;
    }
    d.year = std::stoi(nums[2]);

    return is_date(d);
}


int main() {

    std::cout << std::boolalpha;
/*
    std::ofstream file;
    file.open("test.txt");
    file.setf(std::ios::boolalpha);
    file.setf(std::ios::showpos);
    file.setf(std::ios::showpoint);
    file.setf(std::ios::fixed);
    file.precision(3);
    file.width(10);

    int number = 8;
    file.write((char*)&number, sizeof(number));
    file.write("\n", 1);

    file.close();
 */

/*
    std::cout << "Task 1. Entry in the statement of accounts.\n";
    {
        std::cout << "Consecutively enter the rows with the entries : Name and\n";
        std::cout << "Surname of the payee, date and amount of payment. The\n";
        std::cout << "data must be separated by spaces (type \"exit\" for exit):\n";

        std::ofstream file;
        file.open("payroll_test.txt", std::ios::app);
        if (file.is_open()) {
            date d{};
            std::string input;
            std::string gap = " ";
            std::string fname = "";
            std::string lname = "";
            std::string dt = "";
            std::string amount = "";
            while (std::getline(std::cin, input)) {
                if (input == "exit") {
                    break;
                }
                std::stringstream input_stream(input);
                input_stream >> fname >> lname >> dt >> amount;
                if (is_ddmmyyyy(dt, '.', d)) {
                    if (is_number(amount)) {
                        file << fname << gap << lname << gap
                            << dt << gap << amount << "\n";
                    }
                    else {
                        std::cout << "Incorrect amount!\n";
                    }
                }
                else {
                    std::cout << "Incorrect date!\n";
                }
            }
        }
        file.close();
    }

    std::cout << "\nTask 2. Drawing random pictures.\n";
    {
        std::cout << "Enter the dimensions of the picture: width and height: ";
        std::string input;
        std::string strwidth = "";
        std::string strheight = "";
        char byte = 0;
        char lf = '\n';
        int width = 0, height = 0;
        while (std::getline(std::cin, input)) {
            if (input == "exit") {
                break;
            }
            std::stringstream input_stream(input);
            input_stream >> strwidth >> strheight;
            if (is_number(strwidth) && is_number(strheight)) {
                width = std::stoi(strwidth);
                height = std::stoi(strheight);
                std::ofstream file;
                file.open("binary_pic.txt", std::ios::binary);
                if (file.is_open()) {
                    std::srand(std::time(nullptr));
                    for (int i = 0; i < height; ++i) {
                        for (int j = 0; j < width; ++j) {
                            byte = std::rand() % 2;
                            std::cout << (int)byte;
                            file.write(&byte, 1);
                        }
                        std::cout << lf;
                        file.write(&lf, 1);
                    }
                }

                file.close();
                break;
            }
            else {
                std::cout << "Non-numeric data entered!\n";
            }
        }
    }
//*/

    std::cout << "\nTask 3. Fishing.\n";
    {
        std::ofstream file;
        file.open("river.txt");
        file << "sunfish\n";
        file << "shad\n";
        file << "carp\n";
        file << "bass\n";
        file << "bullhead\n";
        file << "carp\n";
        file << "walleye\n";
        file << "catfish\n";
        file << "carp\n";
        file.close();
    }
    {
        std::string fish = "";
        std::ofstream file;
        file.open("basket.txt", std::ios::app);
        file.close();
    
    
    }
    
    
    return 0;
}