#include <algorithm>
#include <array>
#include <cmath>
#include <iostream>
#include <stdio.h>
#include <string>
#include <vector>

template<typename T, std::size_t N>
void print(const std::array<T, N>& arr, size_t from, size_t to) {
    std::cout << "[";
    if (from > to) {
        for (size_t i = from; i > to; --i) {
            std::cout << arr[i] << ", ";
        }
    }
    else {
        for (size_t i = from; i < to; ++i) {
            std::cout << arr[i] << ", ";
        }
    }
    std::cout << arr[to] << "]\n";
}

template<typename T>
void print(const std::vector<T> & arr, size_t from, size_t to) {
    std::cout << "[";
    if (from > to) {
        for (size_t i = from; i > to; --i) {
            std::cout << arr[i] << ", ";
        }
    } else {
        for (size_t i = from; i < to; ++i) {
            std::cout << arr[i] << ", ";
        }
    }
    std::cout << arr[to] << "]\n";
}

template<typename T, std::size_t ROWS, std::size_t COLS>
void print(const std::array<std::array<T, COLS>, ROWS>& arr) {
    for (const auto& arrow : arr) {
        for (size_t i = 0; i < COLS - 1; ++i) {
            std::cout << arrow[i] << " ";
        }
        std::cout << arrow[COLS - 1] << "\n";
    }
}

template<typename T>
void print(const std::vector<std::vector<T>>& arr) {
    for (const auto& arrow : arr) {
        for (size_t i = 0; i < arrow.size() - 1; ++i) {
            std::cout << arrow[i] << " ";
        }
        std::cout << arrow[arrow.size() - 1] << "\n";
    }
}

template<typename T>
bool isNumber(const std::string & str, T & number, int & maxDigits,
              double & maxMantissa) {
    auto size = str.size();
    if (size == 0) {
        return false;
    }

    int maxExponentaDigits = 1;

    switch (*typeid(number).name()) {
    case 'i':
        // std::cout << "integer\n";
        maxMantissa = 2.1;
        maxDigits = 10;
        maxExponentaDigits = 1;
        break;
    case 'l':
        // std::cout << "long\n";
        maxMantissa = 2.1; // 9.2;
        maxDigits = 10; // 19;
        maxExponentaDigits = 1; // 2;
        break;
    case 'f':
        // std::cout << "float\n";
        maxMantissa = 3.4;
        maxDigits = 38;
        maxExponentaDigits = 2;
        break;
    case 'd':
        // std::cout << "double\n";
        maxMantissa = 1.7;
        maxDigits = 308;
        maxExponentaDigits = 3;
        break;
    default :
        maxDigits = 0;
        maxMantissa = 0;
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
        else if (c == '+' || c == '-') {
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
    int intDigits = 0;
    int fractionZeros = 0;
    int symbols = 0;
    int startExponenta = 0;
    for (size_t i = start; i < size; ++i) {
    	c = str[i];
        if (std::isdigit(c)) {
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
            intDigits = digits;
            ++symbols;
            hasFractions = true;
        }
        else if (c == 'e' || c == 'E') {
            ++symbols;
            startExponenta = start + digits + symbols;
            break;
        }
        else {
            break;
        }
    }

    if (digits == 0) {
        if (start > 0 || symbols > 0) {
            number = 0;
            return true;
        }
        return false;
    }

    if (!hasFractions) {
        intDigits = digits;
    }

    bool exponentaIsNegative = false;
    int exponentaDigits = 0;
    int exponenta = 0;
    if (startExponenta > 0) {
        signHasAlreadyBeen = false;
        for (size_t i = startExponenta; i < size; ++i) {
            char c = str[i];
            if (c == '0') {
                ++startExponenta;
            }
            else if (c == '+' || c == '-') {
                if (signHasAlreadyBeen) {
                    break;
                }
                ++startExponenta;
                signHasAlreadyBeen = true;
                exponentaIsNegative = (c == '-');
            }
            else {
                break;
            }
        }

        for (size_t i = startExponenta; i < size; ++i) {
            if (std::isdigit(str[i])) {
                if (exponentaDigits > maxExponentaDigits) {
                    if (exponentaIsNegative) {
                        break;
                    }
                    std::cout << "Too big the power of the number!\n";
                    return false;
                }
                ++exponentaDigits;
            }
            else if (str[i] == '.') {
                std::cout << "Incorrect power of number!\n";
                return false;
            } 
            else {
                break;
            }
        }

        if (exponentaDigits > 0) {
            exponenta = std::stoi(str.substr(startExponenta, exponentaDigits));
            exponenta = exponentaIsNegative ? -exponenta : exponenta;
        }
    }

    int powerNumber = 0;
    if (intDigits > 0) {
        powerNumber = intDigits + exponenta - 1;
    }
    else {
        powerNumber = exponenta - fractionZeros - 1;
    }

    if (powerNumber > maxDigits) {
        std::cout << "Too big number (power)!\n";
    	return false;
    }

    double doubleNumber = std::stod(str.substr(start, digits + symbols));
    if (exponenta != 0) {
        doubleNumber *= std::pow(10, exponenta);
    }
    double mantissa = doubleNumber / std::pow(10, powerNumber);

    if (powerNumber == maxDigits && mantissa > maxMantissa) {
        std::cout << "Too big number (mantissa)!\n";
        return false;
    }

    number = numberIsNegative ? -doubleNumber : doubleNumber;

    return true;
}

template<typename T>
bool getNumber(const std::string & prompt, T & number) {
    std::string input = "";
    std::cout << prompt;
    while (std::getline(std::cin, input)) {
        if (input == "exit") {
            return false;
        }

        int maxDigits = 0;
        double maxMantissa = 0;
        if (isNumber(input, number, maxDigits, maxMantissa)) {
            break;
        }

        std::string range = "from -" + std::to_string(maxMantissa) + "*10^"
                          + std::to_string(maxDigits) + " to "
						  + std::to_string(maxMantissa) + "*10^"
						  + std::to_string(maxDigits);
        std::cout << "Incorrect value!\n";
        std::cout << "Input a number in the range:\n";
        std::cout << range + ".\n";
        std::cout << "Or type 'exit' for exit.\n";
    }
    return true;
}

template<typename T, std::size_t N>
bool splitIntoNumbers(const std::string & sentence, std::array<T, N> & nums,
                      const char delimeter = ' ') {
    bool result = true;
    int maxDigits = 0;
    double maxMantissa = 0;
    size_t index = 0;
    T number;
    std::string word = "";
    for (const char & c : sentence) {
        if (c == delimeter) {
            if (word.size() > 0) {
                if (!isNumber(word, number, maxDigits, maxMantissa)) {
                    result = false;
                    break;
                }
                word.clear();
                nums[index++] = number;
                if (index == N) {
                    return true;
                }
            }
    	}
    	else {
            word += c;
    	}
    }

    if (result && word.size() > 0) {
        if (isNumber(word, number, maxDigits, maxMantissa)) {
            nums[index] = number;
        }
        else {
            result = false;
        }
    }

    if (!result) {
        std::string range = "from -"
                          + std::to_string(maxMantissa) + "*10^"
   	                      + std::to_string(maxDigits) + " to "
						  + std::to_string(maxMantissa) + "*10^"
						  + std::to_string(maxDigits);
        std::cout << "Incorrect value!\n";
        std::cout << "Input a number in the range:\n";
        std::cout << range + ".\n";
    }

    if (index < N - 1) {
        std::cout << "The container is not fully initialized!\n";
        return false;
    }

    return result;
}

template<typename T, std::size_t N>
bool getRowNumbers(const std::string & prompt, std::array<T, N> & arr) {
    std::string input = "";
    std::cout << prompt;
    while (std::getline(std::cin, input)) {
        if (input == "exit") {
            return false;
        }
        if (splitIntoNumbers(input, arr)) {
            break;
        }
    }
    return true;
}

template<typename T>
bool splitIntoNumbers(const std::string& sentence, std::vector<T>& nums,
    const char delimeter = ' ') {
    bool result = true;
    auto size = nums.size();
    int maxDigits = 0;
    double maxMantissa = 0;
    size_t index = 0;
    T number;
    std::string word = "";
    for (const char& c : sentence) {
        if (c == delimeter) {
            if (word.size() > 0) {
                if (!isNumber(word, number, maxDigits, maxMantissa)) {
                    result = false;
                    break;
                }
                word.clear();
                nums[index++] = number;
                if (index == size) {
                    return true;
                }
            }
        }
        else {
            word += c;
        }
    }

    if (result && word.size() > 0) {
        if (isNumber(word, number, maxDigits, maxMantissa)) {
            nums[index] = number;
        }
        else {
            result = false;
        }
    }

    if (!result) {
        std::string range = "from -"
            + std::to_string(maxMantissa) + "*10^"
            + std::to_string(maxDigits) + " to "
            + std::to_string(maxMantissa) + "*10^"
            + std::to_string(maxDigits);
        std::cout << "Incorrect value!\n";
        std::cout << "Input a number in the range:\n";
        std::cout << range + ".\n";
    }

    if (index < size - 1) {
        std::cout << "The container is not fully initialized!\n";
        return false;
    }

    return result;
}

template<typename T>
bool getRowNumbers(const std::string& prompt, std::vector<T>& arr) {
    std::string input = "";
    std::cout << prompt;
    while (std::getline(std::cin, input)) {
        if (input == "exit") {
            return false;
        }
        if (splitIntoNumbers(input, arr)) {
            break;
        }
    }
    return true;
}

////////////////////////////////////////////////
// Task 1. Maximum sum of consecutive numbers
//         (maximum subarray problem).
////////////////////////////////////////////////



////////////////////////////////////////////////
// Task 2. Find summands.
////////////////////////////////////////////////


////////////////////////////////////////////////
// Task 3. Find ascending number.
////////////////////////////////////////////////

////////////////////////////////////////////////
// Task 4. Modulo sorting.
////////////////////////////////////////////////


int main () {
    std::cout << "Task 1. Speedometer.\n";
    
    float speed = 0,  speed_value = 0;
    float epsilon = 0.01f;
    char speed_txt[6];
    std::cout << "Enter \"exit\" for exit.\n";
    while (true) {
        if (getNumber("Speed delta: ", speed_value)) {
            speed += speed_value;
            speed = speed > 150 ? 150 : speed;
            sprintf_s(speed_txt, "%.1f", speed);
            std::cout << "Speed: " << speed_txt << "\n";
            if (speed < 0 || std::abs(speed) < epsilon) {
                break;
            }
            continue;
        }
        break;
    }

    std::cout << "\nTask 2. Find summands.\n";


    std::cout << "\nTask 3. Find ascending number.\n";


    std::cout << "\nTask 4. Modulo sorting.\n";
    std::cout << "Given an array of numbers sorted in ascending order\n";
    std::cout << "Need to sort by module.\n";


    return 0;
}
