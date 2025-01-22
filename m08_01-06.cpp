#include <iostream>
#include <iomanip>
#include <cmath>
#include <string>
#include <typeinfo>

//  1    - integer (4 bytes) 10 digits (-2 147 483 648 to  +2 147 483 647)
//  1    - long    (4 bytes) 19 digits (-9 223 372 036 854 775 808 to +9 223 372 036 854 775 807)
//  1.0  - double  (8 bytes) 16 digits (-1.7E-308      to 1.7E+308)
//  1.0f - float   (4 bytes)  7 digits (-3.4E-38       to 3.4E+38)
//  1.0l - long double (not less than double up to 18-19 digits)

template<typename T>
bool isNumber(const std::string & str, T & number) {
    auto size = str.size();
    if (size == 0) {
        return false;
    }

    int typeNo = 0;
    int maxDigits = 10;
    int maxExponentaDigits = 1;
    float maxMantissa = 2.1;

    switch (*typeid(number).name()) {
    case 'i':
        std::cout << "integer\n";
        typeNo = 1;
        maxMantissa = 2.1;
        maxExponentaDigits = 1;
        maxDigits = 10;
        break;
    case 'l':
        std::cout << "long\n";
        typeNo = 2;
        maxMantissa = 9.2;
        maxExponentaDigits = 2;
        maxDigits = 19;
        break;
    case 'f':
        std::cout << "float\n";
        typeNo = 3;
        maxMantissa = 3.4;
        maxExponentaDigits = 2;
        maxDigits = 38;
        break;
    case 'd':
        std::cout << "double\n";
        typeNo = 4;
        maxMantissa = 1.7;
        maxExponentaDigits = 3;
        maxDigits = 308;
        break;
    default :
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
            if (!hasFractions && digits == maxDigits) {
                return false;
            }
            ++digits;
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
                return false;
//                break;
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

    std::cout << "start = " << start << " digits = " << digits << " symbols = " << symbols << "\n";
    std::cout << "substr(start, digits + symbols) = " << str.substr(start, digits + symbols) << "\n";
    std::cout << "intDigits      = " << intDigits << "\n";
    std::cout << "startExponenta = " << startExponenta << " e='" << str.substr(startExponenta, size - startExponenta) << "'\n";
    std::cout << "fractionZeros  = " << fractionZeros << "\n";

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

        std::cout << "00startExponenta=" << startExponenta << " e='" << str.substr(startExponenta, size - startExponenta) << "'\n";

        for (size_t i = startExponenta; i < size; ++i) {
            if (std::isdigit(str[i])) {
                if (exponentaDigits == maxExponentaDigits) {
                    return false;
                }
                ++exponentaDigits;
                std::cout << "exponentaDigits=" << exponentaDigits << "\n";
                std::cout << "str[i]=" << str[i] << " exponentaDigits=" << exponentaDigits << " maxExponentaDigits=" << maxExponentaDigits << "\n";
            }
//            else if (str[i] == ' ' && exponentaDigits == maxExponentaDigits) {
//                return false;
//            } 
            else {
                break;
            }
        }

        if (exponentaDigits > 0) {
            exponenta = std::stoi(str.substr(startExponenta, exponentaDigits));
        }

        std::cout << "'" << str.substr(startExponenta, exponentaDigits) << "' exponenta=" << exponenta << "\n";
        if (exponentaIsNegative) {
            exponenta = -exponenta;
        }
    }


    int actualDigits = 0;
    int actualExponenta = 0;
    if (intDigits > 0) {
        actualDigits = digits;
        actualExponenta = intDigits + exponenta;
        std::cout << "str.substr(start, digits + 1) = " << str.substr(start, digits + 1) << "\n";
    }
    else {
        actualDigits = digits - fractionZeros;
        actualExponenta = exponenta - fractionZeros;
        std::cout << "digits - fractionZeros = " << digits - fractionZeros << "\n";
        std::cout << "str.substr(start + fractionZeros + 1, digits - fractionZeros) = " << str.substr(start + fractionZeros + 1, digits - fractionZeros) << "\n";

    }

    std::cout << "actualDigits = " << actualDigits << "\n";
    std::cout << "exponenta = " << exponenta << "\n";
    std::cout << "actualExponenta = " << actualExponenta << "\n";

    if (actualExponenta > maxDigits) {
    	return false;
    }

std::cout << "str.substr(start, digits + symbols - 1) = " << str.substr(start, digits + symbols - 1) << "\n";
    float mantissa = std::stof(str.substr(start, digits + symbols - 1));
    std::cout << "mantissa  * pow(10, exponenta)= " << mantissa * std::pow(10, exponenta) << "\n";
  


}

bool isInteger(const std::string& str, int& number) {
    if (str.empty()) {
        return false;
    }

    auto size = str.size();

    size_t start = (str[0] == '-' || str[0] == '+') ? 1 : 0;
    size_t leadingZeros = 0;

    for (size_t i = start; i < size; ++i) {
        if (str[i] != '0') {
            break;
        }
        ++leadingZeros;
    }

    start += leadingZeros;

    size_t length = size > start + 9 ? start + 9 : size;

    if (!std::isdigit(str[start])) {
        if (leadingZeros > 0) {
            number = 0;
            return true;
        }
        return false;
    }

    ++start;

    for (size_t i = start; i < length; ++i) {
        if (!std::isdigit(str[i])) {
            number = std::stoi(str.substr(0, i - 1));
            return true;
        }
    }

    number = std::stoi(str.substr(0, length));
    return true;
}

bool getInteger(const std::string& prompt, int& number) {
    std::string input = "";
    std::cout << prompt;
    while (std::getline(std::cin, input)) {
        if (input == "exit") {
            return false;
        }

        if (isInteger(input, number)) {
            break;
        }

        std::cout << "Incorrect value!\n";
        std::cout << "Input a number in the range:\n";
        std::cout << "from -999 999 999 to 999 999 999.\n";
        std::cout << "Or type 'exit' for exit.\n";
    }
    return true;
}

int main() {
    std::cout << "Task 1. Space simulator.\n";
    int i = 1;
    long l = 0;

    float f = 0.005;
    double d = 0.023e30;

    long long ll = 0;
    long double ld = 1.01234567890123456;

    std::cout.precision(16);

//    std::cout << d << "\n";

//    std::string str = "+000000001234567890#";
//    std::cout << "str=" << str << "\n";
//    isNumber( str, i );

    std::string str1 = "+00000.000000012901e0009# .0";
    std::cout << "str=" << str1 << "\n";
    isNumber(str1, l);

    return 0;

    std::cout << "\nTask 2. Coffee machine.\n";

    std::cout << "\nTask 3. Red Mars.\n";

    std::cout << "\nTask 4. The frame.\n";

    std::cout << "\nTask 5. Coordinate axes.\n";
	
    std::cout << "\nTask 6. Important announcements.\n";

    return 0;
}
