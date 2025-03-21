#include <algorithm>
#include <array>
#include <cmath>
#include <iostream>
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
                      const char delimiter = ' ') {
    bool result = true;
    int maxDigits = 0;
    double maxMantissa = 0;
    size_t index = 0;
    T number;
    std::string word = "";
    for (const char & c : sentence) {
        if (c == delimiter) {
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
    const char delimiter = ' ') {
    bool result = true;
    auto size = nums.size();
    int maxDigits = 0;
    double maxMantissa = 0;
    size_t index = 0;
    T number;
    std::string word = "";
    for (const char& c : sentence) {
        if (c == delimiter) {
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

int getMaxSum(const std::vector<int>& arr) {
    int result = arr[0];
    int sum = 0;
    int minSum = 0;
    for (const auto& value : arr) {
        sum += value;
        result = result > sum - minSum ? result : sum - minSum;
        minSum = minSum < sum ? minSum : sum;
    }
    return result;
}

int getMaxSum(const std::vector<int>& arr, 
              size_t& left, size_t& right) {
    size_t index = 0;
    left = 0;
    right = 0;
    int minus_pos = -1;
    int sum = 0;
    int minSum = 0;
    int result = arr[0];
    for (const auto& value : arr) {
        sum += value;
        int diff = sum - minSum;
        if (diff > result) {
            result = diff;
            left = minus_pos + 1;
            right = index;
        }
        if (sum < minSum) {
            minSum = sum;
            minus_pos = index;
        }
        ++index;
    }
    return result;
}

int JayKadane84(const std::vector<int>& arr) {
    int result = arr[0];
    int sum = 0;
    for (const auto& value : arr) {
        sum += value;
        result = result > sum ? result : sum;
        sum = sum > 0 ? sum : 0;
    }
    return result;
}

int JayKadane84(const std::vector<int>& arr,
                size_t& left, size_t& right) {
    size_t index = 0;
    left = 0;
    right = 0;
    int minus_pos = -1;
    int sum = 0;
    int result = arr[0];
    for (const auto& value : arr) {
        sum += value;
        if (sum > result) {
            result = sum;
            left = minus_pos + 1;
            right = index;
        }
        if (sum < 0) {
            sum = 0;
            minus_pos = index;
        }
        ++index;
    }
    return result;
}

////////////////////////////////////////////////
// Task 2. Find summands.
////////////////////////////////////////////////

template<typename T>
std::pair<size_t, size_t> summands(const std::vector<T> & arr, T sum) {
    auto size = arr.size();
    if (size > 1) {
        size_t left = 0;
        for (const auto& value : arr) {
            for (size_t i = left; i < size; ++i) {
                if (value + arr[i] == sum) {
                    return { left, i };
                }
            }
            ++left;
        }
    }

    return {};
}

////////////////////////////////////////////////
// Task 3. Find ascending number.
////////////////////////////////////////////////

////////////////////////////////////////////////
// Task 4. Modulo sorting.
////////////////////////////////////////////////

template<typename T>
void modulo_sort(std::vector<T>& arr) {
    auto size = arr.size();
    if (size < 2) {
        return;
    }

    std::sort(arr.begin(), arr.end());

    size_t first_positive_value_id = 0;
    for (const auto& value : arr) {
        if (value >= 0 || first_positive_value_id == size - 1) {
            break;
        }
        ++first_positive_value_id;
    }

    if (first_positive_value_id < 1) {
        return;
    }

    auto right = arr[first_positive_value_id] < 0 ?
                     first_positive_value_id : first_positive_value_id - 1;
    for (size_t i = 0; i < first_positive_value_id / 2; ++i) {
        std::swap(arr[i], arr[right - i]);
    }

    if (arr[first_positive_value_id] < 0) {
        return;
    }

    for (size_t i = first_positive_value_id; i < size; ++i) {
        for (size_t j = i; j > 0; --j) {
            if ((arr[j] < 0 ? -arr[j] : arr[j]) < 
                (arr[j - 1] < 0 ? -arr[j - 1] : arr[j - 1])) {
                std::swap(arr[j], arr[j - 1]);
            }
        }
    }

}

int main () {
    std::cout << "Task 1. Maximum sum of consecutive numbers (maximum subarray problem).\n";
    std::vector<int> arr = { -2, 1, -3, 1, -1, 2, 1, 3, 5, 4 };
    std::cout << "Given an array:\n";
    print(arr, 0, arr.size() - 1);
    size_t left = 0, right = 0;
    auto result = JayKadane84(arr, left, right);
    std::cout << "The maximum sum of consecutive numbers is " << result
              << " from i = " << left << " to " << right << "\n";

    std::cout << "\nTask 2. Find summands.\n";
    std::cout << "Given an array of integers and a number that is the sum of two numbers\n";
    std::cout << "from the array. You need to find these two numbers in the array.\n";
    std::cout << "An array:\n";
    std::vector<int> arr1 = { 2, 7, 11, 15 };
    print(arr1, 0, arr1.size() - 1);
    int num;
    while (true) {
        if (getNumber("Enter a number or \"exit\" for exit: ", num)) {
            auto res = summands(arr1, num);
            if (res.first == res.second) {
                std::cout << "There are no two numbers in the array whose sum is "
                    << num << "\n";
            }
            else {
                std::cout << "The sum of " << arr1[res.first]
                    << " (arr[" << res.first << "]) and "
                    << arr1[res.second] << " (arr[" << res.second
                    << "]) add up to " << num << "\n";
            }
            continue;
        }
        break;
    }

    std::cout << "\nTask 3. Find ascending number.\n";
    std::cout << "Numbers are entered from the keyboard. If the entered number is -1 the\n";
    std::cout << "fifth ascending number of the entered numbers must be displayed on the screen\n";
    std::cout << "If the entered number is -2, the program terminates.\n";
    std::vector<int> arr2(5, 0);
    int inits = 0;
    int num2 = 0;
    while (true) {
        if (getNumber("Enter a number or \"-2\" for exit: ", num2)) {
            if (num2 == -2) {
                break;
            }
            if (num2 == -1) {
                if (inits > 4) {
                    std::cout << "The 5th ascending number is " << arr2[4] << ".\n";
                }
                else {
                	std::cout << "The entered numbers are less than 5.\n";
                }
                continue;
            }

            if (inits < 5) {
                arr2[inits] = num2;
            }
            else {
                if (arr2[4] > num2) {
                    arr2[4] = num2;
                }
            }

            for (int i = (inits > 4 ? 4 : inits); i > 0; --i) {
                if (arr2[i - 1] > arr2[i]) {
                    std::swap(arr2[i -1], arr2[i]);
                }
                else {
                    break;
                }
            }
            ++inits;
        }
        else {
            std::cout << "For exit enter -2.\n";
        }
    }
    print(arr2, 0, arr2.size() - 1);

    std::cout << "\nTask 4. Modulo sorting.\n";
    std::cout << "Given an array of numbers sorted in ascending order\n";
    std::cout << "Need to sort by module.\n";

    std::vector<int> arr3 = {
        -100, -50, -30, -5, -4, -3, -2, 1, 5, 10, 15
    };

    auto size = arr3.size();

    if (size > 0) {
        std::cout << "Source array:\n";
        print(arr3, 0, size - 1);

        modulo_sort(arr3);

        std::cout << "Array sorted by module:\n";
        print(arr3, 0, size - 1);
    }
    return 0;
}
