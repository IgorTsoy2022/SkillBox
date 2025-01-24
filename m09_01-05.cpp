#include <iostream>
#include <iomanip>
#include <cmath>
#include <string>
#include <typeinfo>

bool isDigit(const char& c) {
    return (c >= '0') && (c <= '9');
}

int get2digits(const std::string& input) {
    int result;
    char c = input[0];

    if (!isDigit(c)) {
        return -1;
    }
    result = (c - 48) * 10;

    c = input[1];
    if (!isDigit(c)) {
        return -2;
    }

    return result + c - 48;
}

int getMinutes(const std::string& input) {
    auto size = input.size();
    if (size != 5) {
        return -6;
    }

    int hours = get2digits(input.substr(0, 2));

    if (hours > 23) {
        return -3;
    }

    if (input[2] != ':') {
        return -4;
    }

    int minutes = get2digits(input.substr(3, 2));

    if (minutes > 59) {
        return -5;
    }

    return hours * 60 + minutes;
}

static void wrongInputMessage(int msgNo) {
    switch (msgNo) {
    case -1:
    case -2:
        std::cout << "Must be double figures instead of characters!\n"
                  << "Single digit number must be preceded by a leading zero.\n";
        break;
    case -3:
        std::cout << "The hours must be less than 24.\n";
        break;
    case -4:
        std::cout << "The symbol that separates hours and minutes should be \":\".\n";
        break;
    case -5:
        std::cout << "The minutes must be less than 60.\n";
        break;
    default:
        std::cout << "Enter time in the following format: \"HH:MM\".\n";
    }
}

bool getTimeMinutes(const std::string& prompt, int& minutes) {
    std::string input = "";
    std::cout << prompt;
    while (std::getline(std::cin, input)) {
        if (input == "exit") {
            return false;
        }

        auto t = getMinutes(input);
        if (t >= 0) {
            minutes = t;
            break;
        }

        wrongInputMessage(t);
        std::cout << "For exit type \"exit\".\n";
    }
    return true;
}


template<typename T>
bool isNumber(const std::string& str, int maxDigits) {
    auto size = str.size();
    if (size == 0) {
        return false;
    }

    bool numberIsNegative = false;
    bool signHasAlreadyBeen = false;
    size_t start = 0;
    char c = '0';
    for (size_t i = 0; i < size; ++i) {
        c = str[i];
        if (c == '0' || c == ' ') {
            ++start;
        }
        else if (c == '-') {
            if (signHasAlreadyBeen) {
                std::cout << "Incorrect number format!\n";
                return false;
            }
            ++start;
            signHasAlreadyBeen = true;
            numberIsNegative = (c == '-');
        }
        else {
            break;
        }
    }

    bool hasFractions = false;
    bool completeFractionZeros = false;
    int digits = 0;
    int fractionZeros = 0;
    for (size_t i = start; i < size; ++i) {
        c = str[i];
        if (isDigit(c)) {
            ++digits;
            if (!hasFractions && digits > maxDigits) {
                std::cout << "Too big number (digits)!\n";
                return false;
            }
            if (hasFractions && !completeFractionZeros) {
                if (c == '0') {
                    ++fractionZeros;
                }
                else {
                    completeFractionZeros = true;
                }
            }
        }
        else if (c == '.') {
            if (hasFractions) {
                std::cout << "Too many decimal separators!\n";
                return false;
            }
            hasFractions = true;
        }
        else {
            break;
        }
    }

    if (digits == 0) {
        return false;
    }

    /*
        std::cout << "\nstart = " << start << " digits = " << digits << " symbols = " << symbols << "\n";
        std::cout << "substr(start, digits + symbols) = " << str.substr(start, digits + symbols) << "\n";
        std::cout << "intDigits      = " << intDigits << "\n";
        std::cout << "startExponenta = " << startExponenta << " e='" << str.substr(startExponenta, size - startExponenta) << "'\n";
        std::cout << "fractionZeros  = " << fractionZeros << "\n\n";
    */

    return true;
}


int main() {
//    std::cout.precision(16); 

//    std::cin.clear();
//    std::cin.ignore(std::cin.rdbuf()->in_avail(), '\n');

    std::cout << std::boolalpha;

    std::cout << "Task 1. Travel time.\n";
    int departure = 0, arrival = 0;
    bool allCorrect = getTimeMinutes("Enter the departure time (HH:MM): ", departure);
    allCorrect &= getTimeMinutes("Enter the arrival time (HH:MM): ", arrival);

    if (allCorrect) {
        if (departure < arrival) {
            int hours = 0, minutes = 0;
            hours = (arrival - departure) / 60;
            minutes = (arrival - departure) - hours * 60;
            std::cout << "The travel time was " << hours << " h. " << minutes << " m.\n";
        }
        else {
            std::cout << "The departure time is greater than the arrival time!\n";
        }
    }

    std::cout << "\nTask 2. Long real number.\n";


    std::cout << "\nTask 3. Counting the number of words.\n";

    std::cout << "\nTask 4. Bulls and cows.\n";

    std::cout << "\nTask 5. Roman numbers.\n";

    return 0;
}
