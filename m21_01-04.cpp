#include <cstdlib>
#include <ctime>
#include <iostream>
#include <fstream>
#include <sstream>
#include <array>
#include <string>
#include <vector>
#include <map>
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

// Task 1. Entry in the statement of accounts.

struct DATE {
    int year = 0;
    int month = 0;
    int day = 0;
};

struct PAYROLL {
    std::string name = "";
    DATE date;
    double amount = 0.0;
};

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

bool is_date(const DATE& date) {
    if (date.month < 1 || date.month > 12 ||
        date.day < 1) {
        return false;
    }
    switch (date.month) {
    case 1:
    case 3:
    case 5:
    case 7:
    case 8:
    case 10:
    case 12:
        return date.day < 32;
    case 2:
        return date.day < 29 +
        leap_year(date.year);
    default:
        return date.day < 31;
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

bool ddmmyyyy_to_date(const std::string& ddmmyyyy, const char delimeter,
                      DATE& date) {
    auto size = ddmmyyyy.size();
    if (size < 6) {
        return false;
    }
    auto nums = split(ddmmyyyy, delimeter);
    if (nums.size() != 3) {
        return false;
    }

    if (!is_number(nums[0])) {
       return false;
    }
    date.day = std::stoi(nums[0]);
    if (!is_number(nums[1])) {
        return false;
    }
    date.month = std::stoi(nums[1]);
    if (!is_number(nums[2])) {
        return false;
    }
    date.year = std::stoi(nums[2]);

    return is_date(date);
}

std::string date_to_ddmmyyyy(const DATE& date, const char delimeter = '.') {
    std::string ddmmyyyy = "";

    if (date.day < 10) {
        ddmmyyyy = "0";
    }
    ddmmyyyy += std::to_string(date.day) + delimeter;
    if (date.month < 10) {
        ddmmyyyy += "0";
    }
    ddmmyyyy += std::to_string(date.month) + delimeter;
    ddmmyyyy += std::to_string(date.year);

    return ddmmyyyy;
}

void write(std::ofstream& file, PAYROLL& p) {
    int size = p.name.size();
    file.write((char*)&size, sizeof(size));
    file.write((char*)p.name.c_str(), size);
    std::cout << "write size = " << size << "\n";
    std::cout << "write p.name = " << p.name << "\n";

    auto strdate = date_to_ddmmyyyy(p.date);
    size = strdate.size();
    file.write((char*)&size, sizeof(size));
    file.write(strdate.c_str(), size);

    file.write((char*)&p.amount, sizeof(p.amount));
    char c = '\n';
    file.write(&c, 1);
}

void read(std::ifstream& file, PAYROLL& p) {
    int size = 0;
    file.read((char*)&size, sizeof(size));
    std::cout << "read size = " << size << "\n";
    p.name.resize(size);
    file.read((char*)p.name.c_str(), size);
    std::cout << "read p.name = " << p.name << "\n";

    file.read((char*)&size, sizeof(size));
    std::string strdate = "";
    strdate.resize(size);
    file.read((char*)strdate.c_str(), size);
    std::cout << "read strdate = " << strdate << "\n";

    ddmmyyyy_to_date(strdate, '.', p.date);

    file.read((char*)&p.amount, sizeof(p.amount));
    std::cout << "read p.amount = " << p.amount << "\n";

}

void print(const std::vector<PAYROLL>& plist) {
    for (const auto& p : plist) {
        std::cout << p.name << " " << date_to_ddmmyyyy(p.date)
                  << " " << p.amount << "\n";
    }
}

bool text_view(const std::string& filename) {
    bool isOk = false;
    std::ifstream fs;
    fs.open(filename, std::ios::binary);
    if (fs.is_open()) {
        char buffer[33];
        while (!fs.eof()) {
            fs.read(buffer, 32);
            buffer[fs.gcount()] = 0;
            std::cout << buffer;
        }
        isOk = true;
    }
    fs.close();
    return isOk;
}

// Task 4. ATM 

int main() {

    std::cout << std::boolalpha;

    std::cout << "Task 1. Entry in the payroll.\n";
    {
        std::cout << "Consecutively enter the rows with the entries : Name and\n";
        std::cout << "Surname of the payee, date and amount of payment. The\n";
        std::cout << "data must be separated by spaces (type \"exit\" for exit):\n";

        std::vector<PAYROLL> paylist;
        std::string filename = "payroll_test.bin";
        std::ofstream file;
        file.open(filename, /*std::ios::app |*/ std::ios::binary);
        if (file.is_open()) {
            std::string input;
            std::string gap = " ";
            std::string fname = "";
            std::string lname = "";
            std::string strdate = "";
            std::string amount = "";
            while (std::getline(std::cin, input)) {
                if (input == "exit") {
                    break;
                }

                std::stringstream input_stream(input);
                input_stream >> fname >> lname >> strdate >> amount;

                DATE date{};
                PAYROLL p{};
                if (ddmmyyyy_to_date(strdate, '.', date)) {
                    if (is_number(amount)) {
                        p.name = fname + " " + lname;
                        p.date = date;
                        p.amount = std::stod(amount);
                        paylist.push_back(p);

                        write(file, p);
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
        print(paylist);
    }
    {
        std::vector<PAYROLL> paylist;
        std::string filename = "payroll_test.bin";
        std::ifstream file;
        file.open(filename, std::ios::binary);
        if (file.is_open()) {
            PAYROLL p{};
            while (true) {
                read(file, p);
                paylist.push_back(p);

                if (file.eof()) {
                    break;
                }
            }
        }

        file.close();
        std::cout << "Read:\n";
        print(paylist);
    }

    std::cout << "\nTask 2. Drawing random pictures.\n";
    {
    }

    std::cout << "\nTask 3. Fishing.\n";
    {
    }

    std::cout << "\nTask 4. ATM machine.\n";
    {
    }

    return 0;
}