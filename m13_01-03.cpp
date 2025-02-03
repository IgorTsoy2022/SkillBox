#include <iostream>
#include <cmath>
#include <string>
#include <cassert>
#include <vector>
#include <map>

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
    std::cout << arr[to] << "]";
}

template<typename T>
bool isNumber(const std::string & str, T & number, int & maxDigits, double & maxMantissa) {
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
bool getNumber(const std::string& prompt, T& number) {
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

        std::string range = "from -" + std::to_string(maxMantissa) + "*10^" + std::to_string(maxDigits);
                    range += " to " + std::to_string(maxMantissa) + "*10^" + std::to_string(maxDigits);
        std::cout << "Incorrect value!\n";
        std::cout << "Input a number in the range:\n";
        std::cout << range + ".\n";
        std::cout << "Or type 'exit' for exit.\n";
    }
    return true;
}

template<typename T>
bool get_valid_index(const std::vector<T> & arr, size_t & index, const T & excluded) {
    for (size_t i = index; i < arr.size(); ++i) {
        if (arr[i] != excluded) {
            index = i;
            return true;
        }
    }
    return false;
}

template<typename T>
void remove_item(std::vector<T> & arr, const T & item) {
    auto size = arr.size();
    size_t index = 0;
    for (size_t i = 0; i < size; ++i) {
        if (get_valid_index(arr, index, item)) {
            arr[i] = arr[index++];
        }
        else {
            arr.pop_back();
        }
    }
}

int main () {
//    std::cout.precision(16); 

//    std::cin.clear();
//    std::cin.ignore(std::cin.rdbuf()->in_avail(), '\n');

    std::cout << std::boolalpha;
    bool allCorrect = false;

    std::cout << "Task 1. Delete elements from container.\n";
    int number_of_elements = 0;
    allCorrect = getNumber("Enter the number of elements: ", number_of_elements);
    if (allCorrect && number_of_elements > 0) {
        std::vector<int> nums(number_of_elements, 0);
        for (int & element : nums) {
            if (!getNumber("Enter an element:", element)) {
                break;
            }
        }
        std::cout << "Entered elements:\n";
        print(nums, 0, nums.size() - 1);
        std::cout << "\n";

        int element_being_deleted = 0;
        if (getNumber("Enter the element to be deleted: ", element_being_deleted)) {

            remove_item(nums, element_being_deleted);

            std::cout << "Remaining elements:\n";
            if (nums.size() > 0) {
                print(nums, 0, nums.size() - 1);
                std::cout << "\n";
            }
            else {
                std::cout << "The container is empty.\n";
            }
        }
    }
        
    std::vector<std::string> words = {
        "The", "sentence", "has",
        "unnecessary", "extra",
        "extra", "words"
    };
    for (const auto & word : words) {
        std::cout << word << " ";
    }
    std::cout << "\n";
    std::cout << "Let's delete the word \"extra\".\n";
    std::string word = "extra";
    remove_item(words, word);
    std::cout << "Result:\n";
   for (const auto & word : words) {
        std::cout << word << " ";
    }
    std::cout << "\n";

    std::cout << "\nTask 2. Purchases.\n";
    std::vector<float> prices {
        2.5, 4.25, 3.0, 10.0
    };
    std::vector<int> items {
        1, 1, 0, 3, 6
    };
    std::cout << "Prices:\n";
    for (const auto & p : prices) {
        std::cout << p << " ";
    }
    std::cout << "\n";
    std::cout << "Items:\n";
    for (const auto & i : items) {
        std::cout << i << " ";
    }
    std::cout << "\n";

    auto price_count = prices.size();
    float purchases = 0.0;
    for (const auto & i : items) {
        if (i < price_count) {
            purchases += prices[i];
        }
        else {
            std::cout << "There is no product with index " << i << "\n";
        }
    }
    std::cout << "The total cost of purchases is going to be " << purchases << " credits.\n";

    std::cout << "\nTask 3. The storage.\n";
    const int maxSize = 20;
    std::vector<int> db(maxSize, 0);
    std::cout << "Start (for exit type \"exit\"):\n";
    int num = 0;
    int index = 0;
    while (true) {
        if (!getNumber("Input number:", num)) {
            break;
        }
        if (num == -1) {
        	for (const int & item : db) {
        	    std::cout << item << " ";
        	}
        	std::cout << "\n";
        	continue;
        }
        if (index < maxSize) {
            db[index++] = num;
        }
        else {
            db.erase(db.begin());
            db.push_back(num);
        }
    }

    return 0;
}
