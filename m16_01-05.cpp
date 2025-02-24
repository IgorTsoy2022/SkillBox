#include <algorithm>
#include <array>
#include <cmath>
#include <iostream>
#include <stdio.h>
#include <sstream>
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
// Task 1.
////////////////////////////////////////////////

////////////////////////////////////////////////
// Task 2. Fractional number stitcher
////////////////////////////////////////////////

double stitch_numbers(int integer_part, int fraction_part) {
    double result = 0;
    std::string fraction = '.' + std::to_string(fraction_part);
    result = integer_part + std::stod(fraction);
    return result;
}

////////////////////////////////////////////////
// Task 3. 
////////////////////////////////////////////////

////////////////////////////////////////////////
// Task 4. Mechanical piano.
////////////////////////////////////////////////

std::string dectobin(int num) {
    num = std::abs(num);
    std::string result;
    while (num != 0) {
        result.push_back(num % 2 + '0');
        num /= 2;
    }

    auto size = result.size();
    for (int i = 0; i < size / 2; ++i) {
        std::swap(result[i], result[size -1 - i]);
    }
 
    return result;
}

int bintodec(const std::string & str) {
	auto size = str.size();
	int result = 0;
	int factor = 1;
	for (int i = size - 1; i >= 0; --i) {
	    result += (str[i] - '0') * factor;
	    factor *= 2;
	}
	return result;
}

enum note {
    DO = 1,
    RE = 2,
    MI = 4,
    FA = 8,
    SOL = 16,
    LA = 32,
    SI = 64
};

////////////////////////////////////////////////
// Task 5. Smart home.
////////////////////////////////////////////////

enum switches {
    POWER = 1,
    SOCKETS = 2,
    LIGHTS_INSIDE = 4,
    LIGHTS_OUTSIDE = 8,
    HEATERS = 16,
    WATER_PIPE_HEATING = 32,
    CONDITIONER = 64
};


int main () {
    std::cout << std::boolalpha;
/*
    std::cout << "Task 1. Speedometer.\n";
    {
        float speed = 0,  speed_value = 0;
        float epsilon = 0.01f;
        char speed_txt[6];
        std::cout << "Enter \"exit\" for exit.\n";
        while (true) {
            if (getNumber("Speed delta: ", speed_value)) {
                speed += speed_value;
                speed = speed > 150 ? 150 : speed;
                std::sprintf(speed_txt, "%.1f", speed);
                std::cout << "Speed: " << speed_txt << "\n";
                if (speed < 0 || std::abs(speed) < epsilon) {
                    break;
                }
                continue;
             }
            break;
        }
    }

    std::cout << "\nTask 2. Fractional number stitcher.\n";
    {
        int integer_part = 0, fraction_part = 0;
        bool allCorrect = getNumber("Enter integer part: ", integer_part);
        if (allCorrect) {
            allCorrect = getNumber("Enter fractional part:", fraction_part);
        }
        if (allCorrect) {
            std::cout << "Result: " << stitch_numbers(integer_part, fraction_part) << "\n";
        }
    }

    std::cout << "\nTask 3. Calculator.\n";
    {
        std::string buffer = "";
        std::cin >> buffer;
        std::stringstream buffer_stream(buffer);
        char c ='0';
        double a = 0, b = 0;
        buffer_stream >> a >> c >> b;
        double result = 0;
        switch (c) {
        case '+' :
            result = a + b;
            break;
        case '-' :
            result = a - b;
            break;
        case '*' :
            result = a * b;
            break;
        case '/':
            result = a / b;;
            break;
        }
        std::cout << "Result: " << result << "\n";
    }

    std::cout << "\nTask 4. Mechanical piano.\n";
    {
        bool correct = false;
        int accord = 0;
        std::string buffer = "";
        std::cin >> buffer;
        for (auto & c : buffer) {
            correct = (c > '0' && c < '8');
            if (!correct) {
                break;
            }
            accord |= 1 << (c - '0') - 1;
        }
        
        if (correct) {
        	if (accord & DO) {
        	    std::cout << "DO";
        	}
        	if (accord & RE) {
        	    std::cout << "RE";
        	}
        	if (accord & MI) {
        	    std::cout << "MI";
        	}
        	if (accord & FA) {
        	    std::cout << "FA";
        	}
            if (accord & SOL) {
        	    std::cout << "SOL";
        	} 
            if (accord & LA) {
        	    std::cout << "LA";
        	} 
        	if (accord & SI) {
        	    std::cout << "SI";
        	}
        }
        else {
            std::cout << "Incorrect input!\n";
        }
    }
//*/

    std::cout << "\nTask 5. Smart home.\n";
    short time = 0;
    short temperature_inside = 0;
    short temperature_outside = 0;
    bool movenent = false;
    short color_temperature = 5000;
    const short color_temperature_step = (5000 - 2700) / (20 - 16);

    /*
    POWER = 1,
    SOCKETS = 2,
    LIGHTS_INSIDE = 4,
    LIGHTS_OUTSIDE = 8,
    HEATERS = 16,
    WATER_PIPE_HEATING = 32,
    CONDITIONER = 64
    */

    // temperature_inside, temperature_outside, movenent, lights
    std::vector<std::string> hourly_sensor_readings {
        /* 00:00 */ "23, 8, yes, on",
        /* 01:00 */ "22, 5, no, off",
        /* 02:00 */ "20, 0, no, off",    // < 22 HEATERS ON
        /* 03:00 */ "21, -3, no, off",       // < 0 WATER_PIPE_HEATING ON
        /* 04:00 */ "22, -5, no, off",
        /* 05:00 */ "23, -5, no, off",                            // LIGHTS_OUTSIDE OFF
        /* 06:00 */ "24, 0, yes, on",        // > 5 WATER_PIPE_HEATING OFF
        /* 07:00 */ "25, 8, yes, off",
        /* 08:00 */ "26, 10, yes, off",  // > 25 HEATERS OFF
        /* 09:00 */ "26, 15, yes, off",
        /* 10:00 */ "25, 20, no, off",
        /* 11:00 */ "25, 21, no, off",
        /* 12:00 */ "25, 25, yes, off",
        /* 13:00 */ "27, 30, yes, off",
        /* 14:00 */ "30, 32, yes, off",      // 30 CONDITIONER ON
        /* 15:00 */ "27, 30, no, off",
        /* 16:00 */ "26, 29, no, off",                            // LIGHTS_OUTSIDE ON
        /* 17:00 */ "25, 25, yes, on",       // 25 CONDITIONER OFF
        /* 18:00 */ "24, 20, yes, on",
        /* 19:00 */ "23, 18, yes, on",
        /* 20:00 */ "22, 15, yes, on",
        /* 21:00 */ "21, 12, yes, on",  // < 22 HEATERS ON
        /* 22:00 */ "22, 10, yes, on",
        /* 23:00 */ "23, 10, no, on",
        /* 00:00 */ "26, 11, no, on",   // > 25 HEATERS OFF

    };

    std::cout << "Since the readings are taken from sensors, we will exclude checking\n"
              << "for incorrect input data.\n";
    int switches_state = 0;
    switches_state |= POWER;   // Power on.
    switches_state |= SOCKETS; // Sockets on.

    char c = '0';
    char buff1[3], buff2[3];

    for (const std::string & readings : hourly_sensor_readings) {
        std::cout << (time < 10 ? "0" : "") << time << ":00 h " << readings << "\n";
        std::stringstream buffer_stream(readings);
        buffer_stream >> temperature_inside >> temperature_outside >> buff1 >> buff2;
        std::cout << "buff1 = " << buff1 << "\n";
        movenent = (buff1 == "yes") ? true : false;
        std::cout << "movenent=" << movenent << "\n";

        if (time == 0) {
            color_temperature = 5000;
        }

        time = time > 22 ? 0 : time + 1;
    }

    return 0;
}
