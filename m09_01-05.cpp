#include <iostream>
#include <iomanip>
#include <cmath>
#include <string>
#include <vector>
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
                  << "A sngle digit number must be preceded by a leading zero.\n";
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

std::vector<std::string> split(const std::string& sentence, const char delimiter = ' ') {
    std::vector<std::string> result;
    std::string word = "";
    for (const char & c : sentence) {
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

int countWords(const std::string & sentence, const char delimiter = ' ') {
    int result = 0;
    bool isWord = false;
    for (const char & c : sentence) {
        if (c == delimiter) {
            if (isWord) {
                ++result;
                isWord = false;
            }
        }
        else {
            isWord = true;
        }
    }
    if (isWord) {
        ++result;
    }
    return result;
}

bool getStringNumber(const std::string & prompt, std::string & number, int digits) {
    if (digits <= 0) {
        return false;
    }
    std::cout << prompt;
    while (std::getline(std::cin, number)) {
        if (number == "exit") {
            number = "";
            return false;
        }

        if (number.size() != digits) {
        	std::cout << "Incorrect number of digits entered.\n";
        }
        else {
            bool isCorrect = true;
            for (const char & c : number) {
                if (!isDigit(c)) {
                    isCorrect = false;
                    std::cout << "The entered string contains non-numeric characters.\n"; 
                    break;
                }
            }
            if (isCorrect) {
                return true;
            }
        }

        std::cout << "Enter " << digits << " digits.\n";
        std::cout << "For exit type \"exit\".\n";
    }
    return true;
}

std::pair<int, int> bullsAndCows(const std::string & number1, const std::string & number2) {
    std::pair<int, int> result = { 0, 0 };
    auto size1 = number1.size();
    auto size2 = number2.size();
    std::vector<bool> idx1 (size1, true);
    std::vector<bool> idx2 (size2, true);
    
    for (int i = 0; i < size1; ++i) {
        if (number1[i] == number2[i]) {
            idx1[i] = false;
            idx2[i] = false;
            ++result.first;
        }
    }

    for (int i = 0; i < size1; ++i) {
        if (!idx1[i]) {
            continue;
        }
        for (int j = 0; j < size2; ++j) {
            if (!idx2[j]) {
                continue;
            }
            if (number1[i] == number2[j]) {
                ++result.second;
                idx2[j] = false;
            }
        }
    }
    
    return result;
}

/*
    I - 1
    V - 5
    X - 10
    L - 50
    C - 100
    D - 500
    M - 1000
*/

std::string getRomanNumber(int & number) {
    char ten = '0';
    char five = '0';
    char unit = 'M';

    int intPart = number;
    if (number > 999) {
        intPart /= 1000;
        number -= intPart * 1000;
    }
    else if (number > 99) {
        intPart /= 100;
        number -= intPart * 100;
        ten = 'M';
        five = 'D';
        unit = 'C';
    }
    else if (number > 9) {
        intPart /= 10;
        number -= intPart * 10;
        ten = 'C';
        five = 'L';
        unit = 'X';
    }
    else {
        number = 0;
        ten = 'X';
        five = 'V';
        unit = 'I';
    }

    std::string out = "";
    switch (intPart) {
    case 9:
        return out + unit + ten;
    case 8:
        return out + five + unit + unit + unit;
    case 7:
        return out + five + unit + unit;
    case 6:
        return out + five + unit;
    case 5:
        return out + five;
    case 4:
        return out + unit + five;
    case 3:
        return out + unit + unit + unit;
    case 2:
        return out + unit + unit;
    case 1:
        return out + unit;
    default:
        break;
    }
    return "";
}

std::string to_roman(int number) {
    std::string out ="";

    while (number > 0) {
        out += getRomanNumber(number);
    }

    return out;
}

bool isLongNumber(const std::string & str, int maxDigits) {
    auto size = str.size();
    if (size == 0) {
        return false;
    }

    bool hasLeadingZero = false;
    bool signHasAlreadyBeen = false;
    size_t start = 0;
    char c = '0';
    for (size_t i = 0; i < size; ++i) {
        c = str[i];
        if (c == '0') {
            hasLeadingZero = true;
            ++start;
        }
        else if (c == ' ') {
            ++start;
        }
        else if (c == '-') {
            if (signHasAlreadyBeen) {
                std::cout << "Incorrect number format!\n";
                return false;
            }
            ++start;
            signHasAlreadyBeen = true;
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
            return false;
        }
    }

    return (digits > 0 || hasLeadingZero);

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

int main() {
//    std::cout.precision(16); 

//    std::cin.clear();
//    std::cin.ignore(std::cin.rdbuf()->in_avail(), '\n');

    std::cout << std::boolalpha;
    bool allCorrect = false;

    std::cout << "Task 1. Travel time.\n";
    int departure = 0, arrival = 0;
    allCorrect = getTimeMinutes("Enter the departure time (HH:MM): ", departure);
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
    std::vector<std::string> testStrings = { 
	"0", "0.", ".", ".0", "0123", "00.000", ".15",	"165.",
	"999999999999999999999999999999999.999999999999999999999", "-1.0", "-.35",
	"1.2.3", "-.", "11e-3", "+25"
    };
    for (const std::string & text : testStrings) {
        std::cout << text << " " << isLongNumber(text, 300) << "\n";
    }

    std::cout << "\nTask 3. Counting the number of words.\n";
    std::string sentence = "";
    std::cout << "Enter a sentence: ";
    std::getline(std::cin, sentence);
    auto wordSet = split(sentence);
    for (const std::string & word : wordSet) {
        std::cout << word << "\n";
    }
    std::cout << "There are " << wordSet.size() << " words in sentence \"" << sentence << "\"\n";
    
    auto words = countWords(sentence);
    std::cout << "There are " << words << " words in sentence \"" << sentence << "\"\n";

    std::cout << "\nTask 4. Bulls and cows.\n";
    const int digits = 4;
    std::string conceivedNumber = "";
    std::string attempt = "";
    
    allCorrect = getStringNumber("First player: Think of a " + std::to_string(digits) +
	    "-digits number and enter\nit: ", conceivedNumber, digits);
    allCorrect &= getStringNumber("Second player: Enter a " + std::to_string(digits) +
	    "-digits number to attempt\nmatch the conceived number: ", attempt, digits);
    if (allCorrect) {
        auto result = bullsAndCows(conceivedNumber, attempt);
        std::cout << "bulls: " << result.first << "\n";
        std::cout << "cows: " << result.second << "\n";
    }

    std::vector<std::string> nums1 = { "5671", "1234", "0023", "2013", "1111", "1222" };
    std::vector<std::string> nums2 = { "7251", "1234", "2013", "0023", "1111", "2234" };
    for (int i = 0; i < nums1.size(); ++i) {
        auto r = bullsAndCows(nums1[i], nums2[i]);
        std::cout << nums1[i] << " " << nums2[i];
        std::cout << " bulls: " << r.first;
        std::cout << " cows: " << r.second << "\n";
    }

    std::cout << "\nTask 5. Roman numbers.\n";
    int arabic = 0;
    allCorrect = getNumber("Enter a positive integer number no more than 3999: ", arabic);
    if (allCorrect) {
        if (arabic < 0) {
            std::cout << "The number must be positive.\n";
        }
        else if (arabic == 0) {
            std::cout << "There is no roman number representing zero.\n";
        }
        else if (arabic > 3999) {
            std::cout << "The number must be less than 3999.\n";
        }
        else {
            auto roman = to_roman(arabic);
            std::cout << "For " << arabic << " roman number is " << roman << "\n";
            std::vector<int> numbers = { 351, 449, 1313, 2020 };
            for (const int & n : numbers) {
                std::cout << n << " = " << to_roman(n) << "\n";
            }
        }
    }

    return 0;
}
