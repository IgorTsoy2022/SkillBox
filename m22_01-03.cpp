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

std::vector<std::string> 
split(const std::string& str, const char delimiter = ' ') {
    std::vector<std::string> result;
    std::string word = "";
    for (const auto& c : str) {
        if (c == delimiter) {
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

// Task 3. Anagrams.

bool are_anagrams(const std::string& str1, const std::string& str2) {
    if ((str1.size() < 1) || (str1 == str2)) {
        return false;
    }

    std::map<char, int> letters;
    for (const auto& c : str1) {
        if (c == ' ') {
            continue;
        }

        char key = (c >= 'a' && c <= 'z') ? c - 32 : c;
        ++letters[key];
    }

    for (const auto& c : str2) {
        if (c == ' ') {
            continue;
        }

        char key = (c >= 'a' && c <= 'z') ? c - 32 : c;
        
        if (letters.count(key) == 0) {
            return false;
        }
        if (letters.at(key) == 1) {
            letters.erase(key);
        }
        else {
            --letters.at(key);
        }
    }

    return letters.empty();
}


bool are_weak_anagrams(const std::string& str1, const std::string& str2) {
    if ((str1.size() < 1) || (str1 == str2)) {
        return false;
    }

    std::set<char> letters;
    for (const auto& c : str1) {
        if (c == ' ') {
            continue;
        }

        char key = (c >= 'a' && c <= 'z') ? c - 32 : c;
        letters.insert(key);
    }

    for (const auto& c : str2) {
        if (c == ' ') {
            continue;
        }

        char key = (c >= 'a' && c <= 'z') ? c - 32 : c;

        if (letters.count(key) == 0) {
            return false;
        }
    }

    return true;
}


int main() {

    std::cout << "Task 1. Telephone directory.\n";
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
                            std::cout << "The subscriber with the specified"
                                      << " number was not found.\n";
                        }
                    }
                    else {
                        std::vector<long long> phone_numbers;
                        str1 = toupper(str1);
                        for (const auto& [num, name] : phonebook) {
                            if (toupper(name) == str1) {
                                phone_numbers.push_back(num);
                            }
                        }
                        if (phone_numbers.size() > 0) {
                            for (const auto& num : phone_numbers) {
                                std::cout << 
                                   ll_to_phone(num, "+# (###) ###-##-##")
                                   << " ";
                            }
                            std::cout << "\n";
                        }
                        else {
                            std::cout << "The subscriber with the specified name"
                                      << " was not found.\n";
                        }
                    }
                }
            }
        }

        for (const auto& [number, name] : phonebook) {
            std::cout << ll_to_phone(number, "+# (###) ###-####") << " "
                      << name << "\n";
        }
    }

    std::cout << "\nTask 2. Registry.\n";
    {
        std::map<std::string, int> registry;
        std::string input = "";
        std::cout << "1. Enter the last name to be placed on the waiting list.\n";
        std::cout << "2. Entry \"next\" to call the next patient.\n";
        std::cout << "3. Entry \"exit\" for quit.\n";
        std::cout << "<-";
        while (std::getline(std::cin, input)) {
            if (input == "exit") {
                break;
            }
            if (input == "next") {
                if (registry.empty()) {
                    std::cout << "There's no one in line.\n";
                }
                else {
                    auto it = registry.begin();
                    std::cout << "-> " << it->first << "\n";
                    if (it->second == 1) {
                        registry.erase(it);
                    }
                    else {
                        --it->second;
                    }
                }
            }
            else {
                ++registry[input];
            }
            std::cout << "<-";
        }
        if (registry.empty()) {
            std::cout << "The list of those left in line is empty.\n";
        }
        else {
            std::cout << "The list of those left in line:\n";
            print(registry);
        }
    }

    std::cout << "\nTask 3. Anagrams.\n";
    {
        // Eleven plus two = twelve plus one

        std::map<char, int> registry;
        std::string input = "";
        std::string str2 = "";
        std::cout << "Enter two strings to see if they are anagrams or type\n"
                  << "\"exit\" for quit.\n";
        while (true) {
            std::cout << "String 1 << ";
            std::getline(std::cin, input);

            if (input == "exit") {
                break;
            }

            std::cout << "String 2 << ";
            std::getline(std::cin, str2);

            if (are_anagrams(input, str2)) {
                std::cout << "Line \"" << input << "\" and\n";
                std::cout << "line \"" << str2 << "\" are\n";
                std::cout << "anagrams of each other in terms of characters\n";
                std::cout << "and number of characters.\n";
            }
            else {
                if (are_weak_anagrams(input, str2)) {
                    std::cout << "Line \"" << str2 << "\" consists of the letters of\n";
                    std::cout << "line \"" << input << "\".\n";
                }
                else {
                    std::cout << "The strings are not anagrams.\n";
                }
            }
        }
    }

    return 0;
}