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

    if (!hasFractions) {
        intDigits = digits;
    }

/*
    std::cout << "\nstart = " << start << " digits = " << digits << " symbols = " << symbols << "\n";
    std::cout << "substr(start, digits + symbols) = " << str.substr(start, digits + symbols) << "\n";
    std::cout << "intDigits      = " << intDigits << "\n";
    std::cout << "startExponenta = " << startExponenta << " e='" << str.substr(startExponenta, size - startExponenta) << "'\n";
    std::cout << "fractionZeros  = " << fractionZeros << "\n\n";
*/

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

/*
    std::cout << "exponenta = " << exponenta << "\n";
    std::cout << "maxDigits = " << maxDigits << "\n";
    std::cout << "powerNumber = " << powerNumber << "\n";
*/

    if (powerNumber > maxDigits) {
        std::cout << "Too big number (power)!\n";
    	return false;
    }

    if (digits > 0) {
        double doubleNumber = std::stod(str.substr(start, digits + symbols));
        if (exponenta != 0) {
            doubleNumber *= std::pow(10, exponenta);
        }
        double mantissa = doubleNumber / std::pow(10, powerNumber);

        //    std::cout << "maxMantissa = " << maxMantissa << " mantissa = " << mantissa << "\n";

        if (powerNumber == maxDigits && mantissa > maxMantissa) {
            std::cout << "Too big number (mantissa)!\n";
            return false;
        }

        number = numberIsNegative ? -doubleNumber : doubleNumber;

        return true;
    }

    return false;
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

    std::string str = "+000000001234567890#";
    std::cout << "str=" << str << "\n";
    getNumber("Enter value:", d);
    std::cout << "d=" << d << "\n";


    return 0;

    std::cout << "\nTask 2. Coffee machine.\n";

    std::cout << "\nTask 3. Red Mars.\n";

    std::cout << "\nTask 4. The frame.\n";

    std::cout << "\nTask 5. Coordinate axes.\n";
	
    std::cout << "\nTask 6. Important announcements.\n";

    return 0;
}
