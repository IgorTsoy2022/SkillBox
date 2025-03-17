#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <map>

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

// Task 1. Telephone directory.

long long phone_to_ll(const std::string & input) {
    long long number = 0;
    for (const char & c : input) {
        if (c >= '0' && c <= '9') {
            number *= 10;
            number += c - 48;
        }
    }
    
    return number;
}

std::string ll_to_phone(long long number, const std::string format) {
    std::string out = "";
    long long divisor = 0;
    for (const auto & c : format) {
        if (c == '#') {
            if (divisor == 0) {
            	divisor = 1;
            }
            else {
                divisor *= 10;
            }
        }
    }
    
    for (const auto & c : format) {
        if (c == '#') {
            auto curnum = number / divisor;
            out += std::to_string(curnum);
            number -= curnum * divisor;
            divisor /= 10;
        }
        else {
            out += c;
        }
    }

    return out;
}


int main() {

    std::cout << "Task 1. Telephone directory.\n";
    {
        std::map<long long, std::string> phonebook;
        std::string input = "";
        std::cout << "1. Entering a phone number and surname will add the entry to the directory.\n";
        std::cout << "2. Phone number query will output the surname of the subscriber.\n";
        std::cout << "3. Surname query will output phone numbers of subscribers with the specified surname.\n";
        std::cout << "Entry: ";
        while (std::getline(std::cin, input)) {
            if (input == "exit") {
                break;
            }
            std::string str1 = "";
            std::string str2 = "";
            std::stringstream input_stream(input);
            input_stream >> str1 >> str2;
            if (str1.size() > 0) {
                if (str2.size() > 0) {
                    if ((str1[0] >= '0' && str1[0] <= '9') ||
                         str1[0] == '+' || str1[0] == '(') {
                        phonebook[phone_to_ll(str1)] = str2;
                    }
                    else {
                        std::cout << "First string is not a phone number!\n";
                    }
                }
                else {
                     if ((str1[0] >= '0' && str1[0] <= '9') ||
                         str1[0] == '+' || str1[0] == '(') {
                        auto it = phonebook.find(phone_to_ll(str1));
                        if (it != phonebook.end()) {
                             std::cout << it->second << "\n";
                         }
                         else {
                             std::cout << "The subscriber with the specified number was not found.\n";
                         }
                     }
                     else {
                         bool found = false;
                         for (const auto& [num, name] : phonebook) {
                             if (name == str1) {
                                 found = true;
                                 std::cout << ll_to_phone(num, "+# (###) ###-##-##") << " ";
                             }
                         }
                         if (found) {
                             std::cout << "\n";
                         }
                         else {
                             std::cout << "The subscriber with the specified name was not found.\n";
                         }
                     }
                }
            }

        }

        for (const auto& [number, name] : phonebook) {
            std::cout << ll_to_phone(number, "+# (###) ###-####") << " " << name << "\n";
        }
    }

    std::cout << "\nTask 2. .\n";
    {
    }

    std::cout << "\nTask 3. .\n";
    {
    }

    return 0;
}