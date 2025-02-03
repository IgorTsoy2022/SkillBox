#include <iostream>
#include <cmath>
#include <string>
#include <cassert>
#include <vector>
#include <map>
#include <windows.h>

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

void goto_xy(unsigned short x, unsigned short y) {
    HANDLE hStdOut = ::GetStdHandle(STD_OUTPUT_HANDLE);
    COORD position = { x, y };
    ::SetConsoleCursorPosition(hStdOut, position);
}

/*
COORD GetConsoleCursorPosition(HANDLE hConsoleOutput) {
    CONSOLE_SCREEN_BUFFER_INFO cbsi;
    if (GetConsoleScreenBufferInfo(hConsoleOutput, &cbsi)) {
        return cbsi.dwCursorPosition;
    }
    else {
        // The function failed. Call GetLastError() for details.
        COORD invalid = { 0, 0 };
        return invalid;
    }
}
*/

void draw_table(const char (&arr)[3][3]) {
    std::cout << "   | A | B | C |\n";
    for (int i {}; i < 3; ++i) {
        std::cout << "---+---+---+---+\n";
        std::cout << " " << i + 1 << " ";
        for (int j {}; j < 3; ++j) {
            std::cout << "| " << arr[i][j] << " ";
        }
        std::cout << "|\n";
    }
    std::cout << "---+---+---+---+\n";
}

struct tblCoord {
    unsigned short x = 0;
    unsigned short y = 0;
};

bool point_is_correct(const std::string & input, const char (&tbl)[3][3], tblCoord & point) {
    if (input.size() > 5) {
        return false;
    }
    unsigned short x = 0;
    unsigned short y = 0;
    for (const char & c : input) {
        if (c == ' ') {
            continue;
        }
        if ((c >= 'A' && c < 'D' ||
             c >= 'a' && c < 'd') && x == 0) {
            x = c - (c > 'D' ? 'a' : 'A') + 1;
            if (y > 0) {
                break;
            }
            continue;
        }
        if (c > '0' && c < '4' && y == 0) {
            y = c - '0';
            if (x > 0) {
                break;
            }
            continue;
        }
        return false;
    }

    if (x > 0 && y > 0) {
        if (tbl[y - 1][x - 1]  != ' ') {
            return false;
        }
        point.x = x;
        point.y = y;
        return true;
    }

    return false;
}

bool get_point(const std::string & prompt, const char (&tbl)[3][3], tblCoord & point) {
    std::string input = "";
    std::cout << prompt;
    while (std::getline(std::cin, input)) {
        if (input == "exit") {
            return false;
        }
        if (point_is_correct(input, tbl, point)) {
            return true;
        }
        gotoxy(prompt.size() + 1, 10);
        std::string blank(input.size(), ' ');
        std::cout << blank;
        gotoxy(prompt.size() + 1, 10);
    }

    return true;
}



int main () {
//    std::cin.clear();
//    std::cin.ignore(std::cin.rdbuf()->in_avail(), '\n');

    std::cout << std::boolalpha;
    bool allCorrect = false;

    std::cout << "Task 1. The banquet table.\n";
    int cutleries[2][6] = {
        { 3, 3, 3, 3, 3, 3 },
        { 3, 3, 3, 3, 3, 3 }
    };
    int dessert_spoons[2][6] = {
        { 1, 0, 0, 0, 0, 0 },
        { 1, 0, 0, 0, 0, 0 }
    };
    int plates[2][6] = {
        { 2, 2, 2, 2, 2, 2 },
        { 2, 2, 2, 2, 2, 2 }
    };
    int dessert_plates[2][6] = {
        { 1, 0, 0, 0, 0, 0 },
        { 1, 0, 0, 0, 0, 0 }
    };
    int chairs[2][6] = {
        { 1, 1, 1, 1, 1, 1 },
        { 1, 1, 1, 1, 1, 1 }
    };

    std::cout << "1-st row, seat 5: add 1 chair.\n";
    ++chairs[0][5];
    
    std::cout << "2-nd row, seat 3: the spoon is missing.\n";
    --cutleries[1][3];

    std::cout << "2-nd row, VIP seat: handed his spoon to 2-nd row, seat 3.\n";
    --cutleries[1][0];
    ++cutleries[1][3];

    std::cout << "2-nd row, VIP seat: the waiter took the dessert plate away.\n";
    --dessert_plates[1][0];

    std::cout << "The current situation on the table:\n";
    std::cout <<
    "seats     |VIP| 1 | 2 | 3 | 4 | 5 \n" <<
    "----------+---+---+---+---+---+---\n" <<
    "         ";
    for(int i = 0; i < 6; ++i) {
        std::cout << " | " << cutleries[0][i];
    }
    std::cout << "\n";
    std::cout <<
    "cutleries |---|---|---|---|---|---\n" <<
    "         ";
    for(int i = 0; i < 6; ++i) {
        std::cout << " | " << cutleries[1][i];
    }
    std::cout << "\n";

    std::cout <<
    "----------+---+---+---+---+---+---\n" <<
    "dessert  ";
    for(int i = 0; i < 6; ++i) {
        std::cout << " | " << dessert_spoons[0][i];
    }
    std::cout << "\n";
    std::cout <<
    "spoons    |---|---|---|---|---|---\n" <<
    "         ";
    for(int i = 0; i < 6; ++i) {
        std::cout << " | " << dessert_spoons[1][i];
    }
    std::cout << "\n";

    std::cout <<
    "----------+---+---+---+---+---+---\n" <<
    "         ";
    for(int i = 0; i < 6; ++i) {
        std::cout << " | " << plates[0][i];
    }
    std::cout << "\n";
    std::cout <<
    "plates    |---|---|---|---|---|---\n" <<
    "         ";
    for(int i = 0; i < 6; ++i) {
        std::cout << " | " << plates[1][i];
    }
    std::cout << "\n";

    std::cout <<
    "----------+---+---+---+---+---+---\n" <<
    "dessert  ";
    for(int i = 0; i < 6; ++i) {
        std::cout << " | " << dessert_plates[0][i];
    }
    std::cout << "\n";
    std::cout <<
    "plates    |---|---|---|---|---|---\n" <<
    "         ";
    for(int i = 0; i < 6; ++i) {
        std::cout << " | " << dessert_plates[1][i];
    }
    std::cout << "\n";

    std::cout <<
    "----------+---+---+---+---+---+---\n" <<
    "         ";
    for(int i = 0; i < 6; ++i) {
        std::cout << " | " << chairs[0][i];
    }
    std::cout << "\n";
    std::cout <<
    "chairs    |---|---|---|---|---|---\n" <<
    "         ";
    for(int i = 0; i < 6; ++i) {
        std::cout << " | " << chairs[1][i];
    }
    std::cout << "\n";

    std::cout << "\nPress any key to continue...";
    std::cin.get();

    ::system("clear");
    std::cout << "Task 2. Tic-tac-toe.\n";
    char tbl[3][3] = {
        { ' ', ' ', ' ' },
        { ' ', ' ', ' ' },
        { ' ', ' ', ' ' }
    } ;
    tblCoord point {};
    bool next_is_X = true;
    draw_table(tbl);
    std::string prompt = "";
    for (int i = 0; i < 9; ++i) {
    	gotoxy(0, 10);
    	std::string blank(prompt.size() + 5, ' ');
    	std::cout << blank;
    	gotoxy(0, 10);
    	if (next_is_X) {
    	    prompt = "The X's move:";
    	}
    	else {
    		prompt = "The 0's move:";
    	}
 
        if (!get_point(prompt, tbl, point)) {
            break;
        }

        tbl[point.y -1][point.x - 1] = next_is_X ? 'X' : '0';
        gotoxy(0, 2);
        draw_table(tbl);

        next_is_X = !next_is_X;
    }
    
    

    

return 0;

 //   cursor(0, 0);

    std::cout << "\nTask 3. The storage.\n";
 
    std::cout << "\nTask 4. The storage.\n";

    std::cout << "\nTask 5. The storage.\n";

    std::cout << "\nTask 6. The storage.\n";

    std::cout << "\nTask 7. The storage.\n";

    std::cout << "\nTask 8. The storage.\n";

    return 0;
}
