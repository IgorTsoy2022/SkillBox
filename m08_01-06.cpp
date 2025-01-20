#include <iostream>
#include <iomanip>
#include <cmath>
#include <string>
#include <typeinfo>

//  1.0 - double (8 bytes) 16 digits  (from -1.7E-308 to 1.7E+308)
//  1.0f - float (4 bytes) 7 digits   (from -3.4E-38 to 3.4E+38)
//  1.0l - long double (not less than double up to 18-19 digits)

template<typename T>
bool isNumber(const std::string & str, T & number) {
    auto size = str.size();
    if (size == 0) {
        return false;
    }

    int maxDigits = 0;
    int maxExponentaDigits = 0;
    float maxMantissa = 3.4;

    auto t = typeid(number).name();
    std::cout << *t << "\n";
    if (*t == 'i') {
        std::cout << "integer\n";
        maxDigits = 10;
    }
    else if (*t == 'l') {
        std::cout << "long\n";
        maxDigits = 19;
    }
    else if (*t == 'f') {
        std::cout << "float\n";
        maxExponentaDigits = 2;
        maxDigits = 38;
    }
    else if (*t == 'd') {
        std::cout << "double\n";
        maxMantissa = 1.7;
        maxExponentaDigits = 3;
        maxDigits = 308;
    }
    else {
        return false;
    }


    bool numberIsNegative = false;
    bool signHasAlreadyBeen = false;
    size_t start = 0;
    for (size_t i = 0; i < size; ++i) {
        char c = str[i];
        if (c == '0' || c == ' ') {
            ++start;
        }
        else if (c == '+' || c == '-') {
            if (signHasAlreadyBeen) {
                if (start > 0) {
        	        number = 0;
        	        return true;
        	    }
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
    int digits = 0;
    int intDigits = 0;
    int symbols = 0;
    int startExponenta = 0;
    for (size_t i = start; i < size; ++i) {
    	if (!std::isdigit(str[i])) {
    	    if (str[i] == '.') {
    	        if (hasFractions) {
                    break;
    		    }
    		    intDigits = digits;
    		    ++symbols;
                hasFractions = true;
    	    }
    	    else if (str[i] == 'e' || str[i] == 'E') {
    	        startExponenta = start + digits + symbols + 1;
                break;
            }
            else {
                break;
            }
    	}
    	else {
    	    ++digits;
    	}
    }

    std::cout << "start=" << start << " digits=" << digits << " intDigits=" << intDigits << " symbols=" << symbols << " startExponenta=" << startExponenta << "\n";
    std::cout << str.substr(start, digits + symbols) << "\n";

    bool exponentaIsNegative = false;
    int exponentaDigits = 0;
    int exponenta = 0;
    if (startExponenta > 0) {
        signHasAlreadyBeen = false;
        for (size_t i = startExponenta; i < size; ++i) {
            char c = str[i];
            if (c == '0' || c == ' ') {
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

        for (size_t i = startExponenta; i < size && exponentaDigits < maxExponentaDigits; ++i) {
            if (std::isdigit(str[i])) {
                ++exponentaDigits;
            }
            else {
                break;
            }
        }

        exponenta = std::stoi(str.substr(startExponenta, exponentaDigits));
        std::cout << str.substr(startExponenta, exponentaDigits) << " v=" << exponenta << "\n";
        if (exponentaIsNegative) {
            exponenta = -exponenta;
        }
    }
	
    if (intDigits > 1) {
        std::cout << "current digits = " << intDigits - 1 + exponenta << "\n";
    }
    else {
        std::cout << "1 / exponenta = " << 1/exponenta << "\n";
    }


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
    long long ll = 0;
    float f = 0.005;
    double d = 0.0000023;
    long double ld = 1.01234567890123456;

    std::string str;

    std::cout.precision(16);

    i = 1 / d;
    std::cout << i << "\n";

    str = " 0 +0.00023467e-1008. 05789123e.+5677";
    std::cout << "str=" << str << "\n";
    isNumber( str, d );


    std::cout << "\nTask 2. Coffee machine.\n";

    std::cout << "\nTask 3. Red Mars.\n";

    std::cout << "\nTask 4. The frame.\n";

    std::cout << "\nTask 5. Coordinate axes.\n";
	
    std::cout << "\nTask 6. Important announcements.\n";

    return 0;
}
