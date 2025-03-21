#include <array>
#include <chrono>
#include <cmath>
#include <iostream>
#include <string>
#include <thread>
#include <vector>
#include <windows.h>

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

template<typename T>
bool get_valid_index(const std::vector<T> & arr, size_t & index,
                     const T & excluded) {
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
*/

/*
void clearRow(int row) {
    HANDLE hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);

    COORD coord = { 0, row - 1 };

    CONSOLE_SCREEN_BUFFER_INFO csbi;
    GetConsoleScreenBufferInfo(hStdOut, &csbi);
 FillConsoleOutputCharacter(hStdOut, ' ', 80, coord, NULL);
    SetConsoleCursorPosition(hStdOut, csbi.dwCursorPosition);
}
*/

void goto_xy(SHORT x, SHORT y) {
    HANDLE hStdOut = ::GetStdHandle(STD_OUTPUT_HANDLE);
    COORD position = { x, y };
    ::SetConsoleCursorPosition(hStdOut, position);
}

//----------------------------------------------------//
//    Tic-tac-toe
//----------------------------------------------------//

void clear_table(char (&tbl)[3][3]) {
    for (short i = 0; i < 3; ++i) {
        for (short j = 0; j < 3; ++j) {
           tbl[i][j] = ' ';
        }
    }
}

void draw_table(const char (&tbl)[3][3], SHORT row) {
    goto_xy(0, row);
    std::cout << "   | A | B | C |\n";
    for (short i {}; i < 3; ++i) {
        std::cout << "---+---+---+---+\n";
        std::cout << " " << i + 1 << " ";
        for (short j {}; j < 3; ++j) {
            std::cout << "| " << tbl[i][j] << " ";
        }
        std::cout << "|\n";
    }
    std::cout << "---+---+---+---+\n";
}

struct tblCoord {
    SHORT x = 0;
    SHORT y = 0;
};

void draw_cell(const tblCoord & point, SHORT row, const char c) {
    goto_xy(point.x * 4 + 1, point.y * 2 + row);
    std::cout << c;
}

short point_is_correct(const std::string & input, const char (&tbl)[3][3],
                      tblCoord & point) {
    if (input.size() > 5) {
        return -1;
    }
    SHORT x = 0;
    SHORT y = 0;
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
        return -1;
    }

    if (x > 0 && y > 0) {
        if (tbl[y - 1][x - 1]  != ' ') {
            return 0;
        }
        point.x = x;
        point.y = y;
        return 1;
    }

    return 2;
}

bool get_point(const std::string & prompt, SHORT row, const char (&tbl)[3][3],
               tblCoord & point) {
    std::string input = "";
    std::cout << prompt;
    while (std::getline(std::cin, input)) {
        if (input == "exit") {
            return false;
        }

        std::string blank80(80, ' ');
        switch (point_is_correct(input, tbl, point)) {
        case -1:
            goto_xy(0, row + 1);
            std::cout << blank80;
            goto_xy(0, row + 1);
            std::cout << "Incorrect input!\n";
            break;
        case 0:
            goto_xy(0, row + 1);
            std::cout << blank80;
            goto_xy(0, row + 1);
            std::cout << "The cell has already been occupied!\n";
            break;
        case 1:
            goto_xy(0, row +1);
            std::cout << blank80;
            goto_xy(0, row + 1);
            return true;
        case 2:
            break;
        }
 
        goto_xy(prompt.size(), row);
        std::string blank(input.size(), ' ');
        std::cout << blank;
        goto_xy(prompt.size(), row);
    }
}

void clear_status(SHORT(&horizontals)[3], SHORT(&verticals)[3],
    SHORT(&diagonals)[3]) {
    for (short i = 0; i < 3; ++i) {
        horizontals[i] = 0;
        verticals[i] = 0;
        diagonals[i] = 0;
    }
}

SHORT update_status(tblCoord& point, bool current_move_is_X,
    SHORT(&horizontals)[3], SHORT(&verticals)[3], SHORT(&diagonals)[3]) {
    horizontals[point.y - 1] += current_move_is_X ? 1 : -1;
    verticals[point.x - 1] += current_move_is_X ? 1 : -1;

    if (point.x == point.y) {
        diagonals[0] += current_move_is_X ? 1 : -1;
        if (point.x == 2) {
            diagonals[1] += current_move_is_X ? 1 : -1;
        }
    } 
    else if (point.x == 3 && point.y == 1 ||
        point.x == 1 && point.y == 3) {
        diagonals[1] += current_move_is_X ? 1 : -1;
    }

    ++diagonals[2];

    for (short i = 0; i < 3; ++i) {
        if (horizontals[i] == 3 || verticals[i] == 3) {
            return 1;
        }
        if (horizontals[i] == -3 || verticals[i] == -3) {
            return 2;
        }
        if (i < 2) {
            if (diagonals[i] == 3) {
                return 1;
            }
            if (diagonals[i] == -3) {
                return 2;
            }
        }
    }

    if (diagonals[2] > 7) {
        for (short i = 0; i < 3; ++i) {
            if (horizontals[i] < -1 || verticals[i] < -1) {
                return current_move_is_X ? 5 : 6;
            }
            if (horizontals[i] > 1 || verticals[i] > 1) {
                return current_move_is_X ? 6 : 4;
            }
            if (i < 2) {
                if (diagonals[i] < -1) {
                    return current_move_is_X ? 5 : 6;
                }
                if (diagonals[i] > 1) {
                    return current_move_is_X ? 6 : 4;
                }
            }
        }
        return 3;
    }

    return 0;
}

void tic_tac_toe() {
    ::system("cls");

//    ::system("clrscr");
//    ::system("clear");

    goto_xy(0, 0);
    std::cout << "Let's play the game \"Tic-tac-toe\".\n";
    std::cout << "Enter the coords of the cell in the\n";
    std::cout << "format: A1 or 1a or type \"exit\" for exit.\n";
    char tbl[3][3] = {
        { ' ', ' ', ' ' },
        { ' ', ' ', ' ' },
        { ' ', ' ', ' ' }
    };

    SHORT horizontals[3] = { 0, 0, 0 };
    SHORT verticals[3] = { 0, 0, 0 };
    SHORT diagonals[3] = { 0, 0, 0 };

    tblCoord point{};
    std::string blank(80, ' ');
    std::string prompt = "";
    char choice = '0';
    std::cout << "Press 'X' if 'X' starts the game: ";
    std::cin >> choice;
    bool current_move_is_X = (choice != 'X' && choice != 'x') ? false : true;

    goto_xy(0, 4);
    std::cout << blank;

    std::cin.ignore();

    while (true) {
        draw_table(tbl, 4);

        short status = 0;

        for (short i = 0; i < 9; ++i) {
            goto_xy(0, 12);
            std::cout << blank;
            goto_xy(0, 12);

            if (current_move_is_X) {
                prompt = "The X's move:";
            }
            else {
                prompt = "The 0's move:";
            }

            if (!get_point(prompt, 12, tbl, point)) {
                status = -1;
                break;
            }

            choice = current_move_is_X ? 'X' : '0';
            tbl[point.y - 1][point.x - 1] = choice;
            status = update_status(point, current_move_is_X,
                horizontals, verticals, diagonals);

            // draw_table(tbl, 4);
            draw_cell(point, 4, choice);

            current_move_is_X = !current_move_is_X;

            if (status > 0) {
                break;
            }
        }

        goto_xy(0, 12);
        switch (status) {
        case -1:
            std::cout << "The game is interrupted.\n";
            break;
        case 1:
            std::cout << "The crosses won!\n";
            break;
        case 2:
            std::cout << "The noughts won!\n";
            break;
        case 3:
            std::cout << "We have a draw.\n";
            break;
        case 4:
            std::cout << "Next move is X, so the crosses win!\n";
            break;
        case 5:
            std::cout << "Next move is 0, so the noughts win!\n";
            break;
        case 6:
            if (current_move_is_X) {
                std::cout << "Next move is X, so we have a draw!\n";
            }
            else {
                std::cout << "Next move is 0, so we have a draw!\n";
            }
            break;
        }

        std::cout << "Press 'y' if you want to play again: ";
        std::cin >> choice;

        goto_xy(0, 13);
        std::cout << blank,
        goto_xy(0, 14);
        std::cout << blank;
        if (choice != 'y') {
            break;
        }

        clear_table(tbl);
        clear_status(horizontals, verticals, diagonals);
    }

    goto_xy(0, 13);
    std::cout << "Game over!\n";
}

//----------------------------------------------------//
//    Matrix
//----------------------------------------------------//

template<typename T, size_t ROWS, size_t COLS>
bool equal_matrix(const std::array<std::array<T, COLS>, ROWS>& m1,
                  const std::array<std::array<T, COLS>, ROWS>& m2) {
    for (size_t i = 0; i < ROWS; ++i) {
        for (size_t j = 0; j < COLS; ++j) {
            if (m1[i][j] != m2[i][j]) {
                return false;
            }
        }
    }
    return true;
}

template<typename T, size_t N>
void to_diagonal_matrix(std::array<std::array<T, N>, N>& m) {
    for (size_t i = 0; i < N; ++i) {
        for (size_t j = 0; j < N; ++j) {
            if (i != j) {
                m[i][j] = 0;
            }
        }
    }
}

template<typename T, size_t ROWS, size_t COLS>
std::array<T, ROWS> matrix_x_vector(const std::array<std::array<T, COLS>, ROWS>& a,
    const std::array<T, COLS>& v/*, std::array<T, ROWS>& axv*/) {
    std::array<T, ROWS> axv{};
    size_t index = 0;
    for (const auto& arrow : a) {
        T accum = {};
        for (size_t i = 0; i < COLS; ++i) {
            accum += arrow[i] * v[i];
        }
        axv[index++] = accum;
    }
    return axv;
}

//----------------------------------------------------//
//    Pimples
//----------------------------------------------------//

template<size_t ROWS, size_t COLS>
void clear_cells(std::array<std::array<bool, COLS>, ROWS>& pimples) {
    for (auto& arrows : pimples) {
        for (auto& cell : arrows) {
            cell = true;
        }
    }
}

template<size_t ROWS>
void draw_field(std::array<std::array<bool, 12>, ROWS>& pimples, SHORT row) {
    short index = 1;
    goto_xy(0, row);
    std::cout << "    | A | B | C | D | E | F | G | H | I | J | K | L |\n";
    for (auto& arrows : pimples) {
        std::cout << "----+---+---+---+---+---+---+---+---+---+---+---+---+\n";
        std::cout << (index < 10 ? "  " : " ") << index++ << " ";
        for (auto& cell : arrows) {
            std::cout << "| " << (cell ? "O" : "X") << " ";
        }
        std::cout << "|\n";
    }
    std::cout << "----+---+---+---+---+---+---+---+---+---+---+---+---+\n";
}

bool check_coords(const std::string& input, tblCoord& left, tblCoord& right) {
    auto size = input.size();
    if (size < 4 || size > 12) {
        return false;
    }

    SHORT x1 = 0, x2 = 0, y1 = 0, y2 = 0;
    short index = 0;
    while (index < size) {
        char c = input[index++];
        if (c == ' ') {
            continue;
        }

        if (c >= 'A' && c < 'M' ||
            c >= 'a' && c < 'm') {
            SHORT x = c - (c > 'L' ? 'a' : 'A') + 1;
            if (x1 == 0) {
                x1 = x;
            }
            else if (x2 == 0) {
                x2 = x;
            }
            else {
                return false;
            }
            continue;
        }

        if (c >= '0' && c <= '9') {
            SHORT y = c - '0';
            if (index < size) {
                c = input[index];
                if (c >= '0' && c <= '9') {
                    y = y * 10 + c - '0';
                    ++index;
                }
            }
            if (y > 12) {
                return false;
            }
            if (y1 == 0) {
                y1 = y;
            }
            else if (y2 == 0) {
                y2 = y;
            }
            else {
                return false;
            }
            continue;
        }

        return false;
    }

    if (x1 > 0 && y1 > 0 &&
        x2 > 0 && y2 > 0) {
        if (x1 > x2) {
            left.x = x2;
            left.y = y2;
            right.x = x1;
            right.y = y1;
        }
        else {
            left.x = x1;
            left.y = y1;
            right.x = x2;
            right.y = y2;
        }
        return true;
    }

    return false;
}

bool get_coords(const std::string & prompt, SHORT row, tblCoord & left,
tblCoord & right) {
    std::string blank80(80, ' ');
    std::string input = "";
    std::cout << prompt;
    while (std::getline(std::cin, input)) {
        if (input == "exit") {
            return false;
        }
        if (check_coords(input, left, right)) {
            return true;
        }
        else {
            goto_xy(0, row + 1);
            std::cout << blank80;
            goto_xy(0, row + 1);
            std::cout << "Incorrect input!\n";
        }
        goto_xy(prompt.size(), row);
        std::string blank(input.size(), ' ');
        std::cout << blank;
        goto_xy(prompt.size(), row);
    }

    return true;
}

template<size_t ROWS, size_t COLS>
void draw_region(std::array<std::array<bool, COLS>, ROWS>& pimples,
    const tblCoord& left, const tblCoord& right, SHORT row) {
    SHORT minX = 0, maxX = 0, minY = 0, maxY = 0;
    if (left.x < right.x) {
        minX = left.x;
        maxX = right.x;
    }
    else {
        minX = right.x;
        maxX = left.x;
    }
    if (left.y < right.y) {
        minY = left.y;
        maxY = right.y;
    }
    else {
        minY = right.y;
        maxY = left.y;
    }
    SHORT startX = minX * 4 + 1;

    for (SHORT i = minY; i < maxY + 1; ++i) {
        goto_xy(startX, i * 2 + row);
        for (SHORT j = minX; j < maxX + 1; ++j) {
            std::cout << " " << (pimples[i - 1][j - 1] ? "O" : "X") << " |";
        }
    }
}

template<size_t ROWS, size_t COLS>
void burst_region(std::array<std::array<bool, COLS>, ROWS> & pimples,
                  const tblCoord& left, const tblCoord& right,
                  short & burst_count, SHORT row) {
    SHORT minX = 0, maxX = 0, minY = 0, maxY = 0;
    if (left.x < right.x) {
        minX = left.x;
        maxX = right.x;
    } 
    else {
        minX = right.x;
        maxX = left.x;
    }
    if (left.y < right.y) {
        minY = left.y;
        maxY = right.y;
    }
    else {
        minY = right.y;
        maxY = left.y;
    }
    SHORT startX = minX * 4 + 1;

    std::chrono::milliseconds duration(100);
    for (SHORT i = minY; i < maxY + 1; ++i) {
        SHORT currentX = startX;
        SHORT currentY = i * 2 + row;
        goto_xy(startX, currentY);
        for (SHORT j = minX; j < maxX + 1; ++j) {
            currentX += 4;
            if (pimples[i - 1][j - 1]) {
                std::cout << "Pop!";
                pimples[i - 1][j - 1] = false;
                ++burst_count;
                std::this_thread::sleep_for(duration);
            }
            else {
                goto_xy(currentX, currentY);
            }
        }
    }
}

void pimples() {
    ::system("cls");

    const short Rows = 12, Cols = 12;
    const short Cells = Rows * Cols;
    std::array<std::array<bool, Cols>, Rows> pimples{};
    tblCoord left{}, right{};

    goto_xy(0, 0);
    std::cout << "Let's play the game \"Pimple\".\n";
    std::cout << "Enter the coordinates of one corner (in any order:\n";
    std::cout << "A1, 1A), then the coordinates of the second corner of\n";
    std::cout << "the rectangle. Adjacent numerical coordinates should\n";
    std::cout << "be separated by a space (A1 12L, C10 1E). Letter and\n";
    std::cout << "alternating numerical coordinates can be entered\n";
    std::cout << "without spaces between them (A1L12, 10CE1). The\n";
    std::cout << "pimples inside the rectangle selected by the\n";
    std::cout << "corners will burst.\n";

    std::string blank80(80, ' ');
    while (true) {
        short burst_count = 0;
        clear_cells(pimples);
        draw_field(pimples, 9);

        while (burst_count < Cells) {
            if (!get_coords("Enter rectangle's corners coords:", 35, left, right)) {
                break;
            }
            burst_region(pimples, left, right, burst_count, 9);
            draw_region(pimples, left, right, 9);
            goto_xy(0, 35);
            std::cout << blank80;
            goto_xy(0, 35);
            if (burst_count == Cells) {
                std::cout << "All pimples have been burst.\n";
            }
        }

        char choice = 'N';
        std::cout << "Press 'y' if you want to play again: ";
        std::cin >> choice;

        goto_xy(0, 36);
        std::cout << blank80 << "\n";
        std::cout << blank80;
        if (choice != 'y') {
            break;
        }
        std::cin.ignore();
    }

    goto_xy(0, 36);
    std::cout << "Good bye!\n";
}

//----------------------------------------------------//
//    Sea battle
//----------------------------------------------------//

const short CELLX = 4;
const short CELLY = 2;
const short SEAXY = 10;

void clear_sea(std::vector<std::vector<short>>& sea) {
    for (short i = 0; i < SEAXY; ++i) {
        for (short j = 0; j < SEAXY; ++j) {
            sea[i][j] = 0;
        }
    }
}

void place_ship_at_sea(std::vector<std::vector<short>>& sea,
                       const tblCoord& coord1, const tblCoord& coord2,
                       const SHORT decks, const SHORT shipNo) {
    SHORT col1 = coord1.x - 1;
    SHORT row1 = coord1.y - 1;

    SHORT left = col1 > 0 ? col1 - 1 : col1;
    SHORT right = col1 < SEAXY - 1 ? col1 + 1 : col1;
    SHORT top = row1 > 0 ? row1 - 1 : row1;
    SHORT bottom = row1 < SEAXY - 1 ? row1 + 1 : row1;

    if (decks == 1) {
        for (auto i = top; i < bottom + 1; ++i) {
            for (auto j = left; j < right + 1; ++j) {
                sea[i][j] = (i == row1 && j == col1) ? 1 : 2;
            }
        }
        return;
    }

    SHORT col2 = coord2.x - 1;
    SHORT row2 = coord2.y - 1;
    if (col1 == col2) {
        // Vertical ship
        SHORT minY = row1, maxY = row2;
        if (row1 > row2) {
            minY = row2;
            maxY = row1;
        }

        for (auto i = minY; i < maxY + 1; ++i) {
            sea[i][col1] = decks * 10 + shipNo;
        }

        top = minY > 0 ? minY - 1 : minY;
        bottom = maxY < SEAXY - 1 ? maxY + 1 : maxY;
        for (auto i = top; i < bottom + 1; ++i) {
            if (left < col1) {
                sea[i][left] = 2;
            }
            if (right > col1) {
                sea[i][right] = 2;
            }
        }
        if (top < minY) {
            sea[top][col1] = 2;
        }
        if (bottom > maxY) {
            sea[bottom][col1] = 2;
        }

        return;
    }

    if (row1 == row2) {
        // Horizontal ship
        SHORT minX = col1, maxX = col2;
        if (col1 > col2) {
            minX = col2;
            maxX = col1;
        }

        for (auto j = minX; j < maxX + 1; ++j) {
            sea[row1][j] = decks * 10 + shipNo;
        }

        left = minX > 0 ? minX - 1 : minX;
        right = maxX < SEAXY - 1 ? maxX + 1 : maxX;
        for (auto j = left; j < right + 1; ++j) {
            if (top < row1) {
                sea[top][j] = 2;
            }
            if (bottom > row1) {
                sea[bottom][j] = 2;
            }
        }
        if (left < minX) {
            sea[row1][left] = 2;
        }
        if (right > maxX) {
            sea[row1][right] = 2;
        }

        return;
    }
}

short set_move(std::vector<std::vector<short>>& sea, 
               const tblCoord& coord, short& total_decks,
               const short rest_decks = 4) {
    auto row = coord.y - 1;
    auto col = coord.x - 1;
    auto & cell = sea[row][col];

    if (cell == 0 || cell == 2) {
        cell = 5;
    }
    else if (cell == 1) {
        cell = 4;
        --total_decks;
    }
    else if (rest_decks < 2) {
        auto left = col;
        while (left > 0) {
            if (sea[row][left - 1] != 3) {
                break;
            }
            --left;
        }

        auto right = col;
        while (right < SEAXY - 1) {
            if (sea[row][right + 1] != 3) {
                break;
            }
            ++right;
        }

        if (left != right) {
            for (auto i = left; i < right + 1; ++i) {
                sea[row][i] = 4;
            }
            --total_decks;
        }
        else {
            auto top = row;
            while (top > 0) {
                if (sea[top - 1][col] != 3) {
                    break;
                }
                --top;
            }

            auto bottom = row;
            while (bottom < SEAXY - 1) {
                if (sea[bottom + 1][col] != 3) {
                    break;
                }
                ++bottom;
            }

            for (auto i = top; i < bottom + 1; ++i) {
                sea[i][col] = 4;
            }
            --total_decks;
        }
    }
    else {
        cell = 3;
        --total_decks;
    }

    return cell;
}

char get_symbol(const short num, const bool hidden = false) {
    switch (num) {
    case 0:
        return ' ';
    case 1:
        return hidden ? ' ' : 'S';
    case 2:
        return hidden ? ' ' : '.';
    case 3:
        return '!';
    case 4:
        return 'X';
    case 5:
        return '*';
    }

    if (num > 9) {
        return hidden ? ' ' : 'S';
    }

    return ' ';
}

void draw_sea(const tblCoord& sea_pos, 
              const std::vector<std::vector<short>>& sea,
              const bool hidden = false) {
    short index = 1;
    SHORT row = 1;
    goto_xy(sea_pos.x, sea_pos.y);

    std::cout << "    | A | B | C | D | E | F | G | H | I | J |";
    for (const auto& arrow : sea) {
        goto_xy(sea_pos.x, sea_pos.y + row++);
        std::cout << "----+---+---+---+---+---+---+---+---+---+---+";
        goto_xy(sea_pos.x, sea_pos.y + row++);
        std::cout << (index < 10 ? "  " : " ") << index++ << " |";
        for (const auto & cell : arrow) {
            std::cout << " " << get_symbol(cell, hidden) << " |";
        }
    }
    goto_xy(sea_pos.x, sea_pos.y + row);
    std::cout << "----+---+---+---+---+---+---+---+---+---+---+" << "\n";
}

void draw_sea(const tblCoord& sea_pos, 
              const std::vector<std::vector<short>>& sea,
              const tblCoord& from, const tblCoord& to,
              const bool hidden = false) {
    SHORT minX = 0, maxX = 0, minY = 0, maxY = 0;
    if (from.x < to.x) {
        minX = from.x;
        maxX = to.x;
    }
    else {
        minX = to.x;
        maxX = from.x;
    }
    if (from.y < to.y) {
        minY = from.y;
        maxY = to.y;
    }
    else {
        minY = to.y;
        maxY = from.y;
    }

    if (minX > 1) {
        --minX;
    }
    if (maxX < SEAXY) {
        ++maxX;
    }

    if (minY > 1) {
        --minY;
    }
    if (maxY < SEAXY) {
        ++maxY;
    }

    SHORT startX = minX * CELLX + 1 + sea_pos.x;

    for (SHORT i = minY; i < maxY + 1; ++i) {
        goto_xy(startX, i * CELLY + sea_pos.y);
        for (SHORT j = minX; j < maxX + 1; ++j) {
            std::cout << " " << get_symbol(sea[i - 1][j - 1], hidden) << " |";
        }
    }
}

void draw_sea(const tblCoord& sea_pos,
    std::vector<std::vector<short>>& sea,
    const tblCoord& coord, const bool hidden = false) {
    SHORT minX = coord.x, maxX = coord.x;
    SHORT minY = coord.y, maxY = coord.y;

    if (minX > 1) {
        --minX;
    }
    if (maxX < SEAXY) {
        ++maxX;
    }

    if (minY > 1) {
        --minY;
    }
    if (maxY < SEAXY) {
        ++maxY;
    }

    SHORT startX = minX * CELLX + 1 + sea_pos.x;

    for (SHORT i = minY; i < maxY + 1; ++i) {
        goto_xy(startX, i * CELLY + sea_pos.y);
        for (SHORT j = minX; j < maxX + 1; ++j) {
            std::cout << " " << get_symbol(sea[i - 1][j - 1], hidden) << " |";
        }
    }
}

short check_point(const std::string& input, tblCoord& point) {
    auto size = input.size();
    if (size < 2 || size > 10) {
        return -1;
    }

    SHORT x = 0, y = 0, x_tmp = 0, y_tmp = 0;
    short index = 0;
    char c = '0';
    while (index < size) {
        c = input[index++];
        if (c == ' ') {
            continue;
        }
        if (c >= 'A' && c < 'K' ||
            c >= 'a' && c < 'k') {
            x_tmp = c - (c > 'K' ? 'a' : 'A') + 1;
            if (x == 0) {
                x = x_tmp;
            }
            else {
                return -1;
            }
            continue;
        }
        if (c >= '0' && c <= '9') {
            y_tmp = c - '0';
            if (index < size) {
                c = input[index];
                if (c >= '0' && c <= '9') {
                    y_tmp = y_tmp * 10 + c - '0';
                    ++index;
                }
                if (y_tmp > 10) {
                    return -1;
                }
            }
            if (y == 0) {
                y = y_tmp;
            }
            else {
                return -1;
            }
            continue;
        }
        return -1;
    }

    if (x > 0 && y > 0) {
        point.x = x;
        point.y = y;
        return 1;
    }

    return 0;
}

short check_points(const std::string & input,
                   tblCoord& point1, tblCoord& point2) {
    auto size = input.size();
    if (size < 2 || size > 10) {
        return -1;
    }

    SHORT x1 = 0, x2 = 0, y1 = 0, y2 = 0;
    SHORT x_tmp = 0, y_tmp = 0;
    short index = 0;
    char c = '0';
    while (index < size) {
        c = input[index++];
        if (c == ' ') {
            continue;
        }
        if (c >= 'A' && c < 'K' ||
            c >= 'a' && c < 'k') {
            x_tmp = c - (c > 'K' ? 'a' : 'A') + 1;
            if (x1 == 0) {
                x1 = x_tmp;
            }
            else if (x2 == 0) {
                x2 = x_tmp;
            }
            else {
                return -1;
            }
            continue;
        }
        if (c >= '0' && c <= '9') {
            y_tmp = c - '0';
            if (index < size) {
                c = input[index];
                if (c >= '0' && c <= '9') {
                    y_tmp = y_tmp * 10 + c - '0';
                    ++index;
                }
                if (y_tmp > 10) {
                    return -1;
                }
            }
            if (y1 == 0) {
                y1 = y_tmp;
            }
            else if (y2 == 0) {
                y2 = y_tmp;
            }
            else {
                return -1;
            }
            continue;
        }
        return -1;
    }

    if (x1 > 0 && y1 > 0) {
        point1.x = x1;
        point1.y = y1;
        short coords_count = 1;
        if (x2 > 0 && y2 > 0) {
            point2.x = x2;
            point2.y = y2;
            ++coords_count;
        }
        return coords_count;
    }

    return 0;
}

short check_placement(const std::vector<std::vector<short>>& sea,
                      const tblCoord& coord1, const tblCoord& coord2,
                      short coords_count, short& decks) {
    SHORT col1 = coord1.x - 1;
    SHORT row1 = coord1.y - 1;
    if (coord1.x == coord2.x &&
        coord1.y == coord2.y) {
        coords_count = 1;
    }
    if (coords_count == 1) {
        decks = 1;
        auto cell = sea[row1][col1];
        return cell > 9 ? 1 : cell;
    }

    SHORT col2 = coord2.x - 1;
    SHORT row2 = coord2.y - 1;
    if (col1 == col2) {
        // Vertical ship
        SHORT minY = row1, maxY = row2;
        if (row1 > row2) {
            minY = row2;
            maxY = row1;
        }
        decks = maxY - minY + 1;
        for (SHORT i = minY; i < maxY + 1; ++i) {
            auto cell = sea[i][col1];
            if (cell == 1 || cell == 2 || cell > 9) {
                return cell > 9 ? 1 : cell;
            }
        }
        return 0;
    }

    if (row1 == row2) {
        // Horizontal ship
        SHORT minX = col1, maxX = col2;
        if (col1 > col2) {
            minX = col2;
            maxX = col1;
        }
        decks = maxX - minX + 1;
        for (SHORT j = minX; j < maxX + 1; ++j) {
            auto cell = sea[row1][j];
            if (cell == 1 || cell == 2 || cell > 9) {
                return cell > 9 ? 1 : cell;
            }
        }
        return 0;
    }

    // Diagonal ship
    return 6;
}

    // [0] - 21, [1] - 22, [2] - 23
    // [3] - 31, [4] - 32, [5] - 41
short check_move(const std::vector<std::vector<short>>& sea,
                 const tblCoord& coord, short& shipID) {
    auto cell = sea[coord.y - 1][coord.x - 1];
    if (cell > 20 && cell < 24) {
        shipID = cell - 21;
    }
    else if (cell > 30 && cell < 33) {
        shipID = cell - 28;
    }
    else if (cell > 40) {
        shipID = 5;
    }
    else {
        shipID = -1;
    }
    return cell;
}

bool place_ships(const tblCoord& sea_pos, 
                 std::vector<std::vector<short>> & sea) {
    short total_ships = 10;
    tblCoord coord1{}, coord2{};
    std::vector<short> ships = { 4, 3, 2, 1 };
    std::string input = "";
    std::string blank90(90, ' ');
    std::string prompt = "Enter the coordinates of the ships: ";
    SHORT prompt_col = sea_pos.x;
    SHORT prompt_row = sea_pos.y + 11 * CELLY;

    goto_xy(prompt_col, prompt_row);
    std::cout << prompt;
    while (std::getline(std::cin, input) && total_ships > 0) {
        if (input == "exit") {
            return false;
        }

        std::string err = "";
        auto result = check_points(input, coord1, coord2);
        if (result == -1) {
            err = "Incorrect input. Try again.";
        }
        else if (result == 0) {
            err = "Nothing to do.";
        }
        else {
            short decks = 0;
            result = check_placement(sea, coord1, coord2,
                                     result, decks);
            if (result == 1) {
                err = "Space is occupied by another ship!";
            }
            else if (result == 2) {
                err = "Ships are too close together!";
            }
            else if (result == 6) {
            	err = "Diagonal placement is not allowed!";
            }
            else if (decks > 4) {
                err = "Too many decs on the ship!";
            }
            else if (ships[decks - 1] < 1) {
                switch (decks) {
                case 1:
                    err = "Four single-deck";
                    break;
                case 2:
                    err = "Three double-deck";
                    break;
                case 3:
                    err = "Two triple-deck";
                    break;
                case 4:
                    err = "One four-deck";
                    break;
                }
                err += " ships have already been commissioned!";
            }
            else if (result == 0) {
                --total_ships;
                place_ship_at_sea(sea, coord1, coord2,
                                  decks, ships[decks - 1]--);

                if (decks == 1) {
                    draw_sea(sea_pos, sea, coord1, false);
                }
                else {
                    draw_sea(sea_pos, sea, coord1, coord2, false);
                }

                goto_xy(0, prompt_row + 1);
                std::cout << blank90;

                if (total_ships < 1) {
                    goto_xy(prompt_col, prompt_row);
                    std::cout << std::string(45, ' ');
                    goto_xy(prompt_col, prompt_row);
                    std::cout << "The fleet is fully staffed!";
                    break;
                }

                goto_xy(prompt_col + prompt.length(), prompt_row);
                std::cout << std::string(input.length(), ' ');
                goto_xy(prompt_col + prompt.length(), prompt_row);
                continue;
            }
        }

        goto_xy(prompt_col, prompt_row + 1);
        std::cout << blank90;
        goto_xy(prompt_col, prompt_row + 1);
        std::cout << err;

        goto_xy(prompt_col + prompt.length(), prompt_row);
        std::cout << std::string(input.length(), ' ');
        goto_xy(prompt_col + prompt.length(), prompt_row);
    }

    return true;
}

short shoot(const tblCoord& sea_pos, const std::string & prompt, 
            std::vector<std::vector<short>>& sea,
            std::vector<short>& fleet, short & total_decks) {
    tblCoord coord{};
    std::string input = "";
    std::string blank90(90, ' ');
    SHORT prompt_col = sea_pos.x;
    SHORT prompt_row = sea_pos.y + 11 * CELLY;

    goto_xy(prompt_col, prompt_row);
    std::cout << blank90;
    goto_xy(prompt_col, prompt_row);
    std::cout << prompt;
    while (std::getline(std::cin, input)) {
        if (input == "exit") {
            return -1;
        }

        std::string err = "";
        auto result = check_point(input, coord);
        if (result == -1) {
            err = "Incorrect input. Try again.";
        }
        else if (result == 0) {
            err = "Nothing to do.";
        }
        else {
            short shipID = 0;
            result = check_move(sea, coord, shipID);

            if (result > 2 && result < 6) {
                err = "This point has already been shot at.";
            }
            else {
                if (shipID >= 0) {
                    result = set_move(sea, coord, total_decks, fleet[shipID]--);
                }
                else {
                    result = set_move(sea, coord, total_decks);
                }
                draw_sea(sea_pos, sea, { 1, 1 }, { 10, 10 }, true);

                if (total_decks < 1) {
                    goto_xy(prompt_col, prompt_row);
                    std::cout << std::string(45, ' ');
                    goto_xy(prompt_col, prompt_row);
                    std::cout << "Enemy fleet completely sunk! ";
                    return 1;
                }
                
                if(result == 5) {
                    return 0;
                }

                goto_xy(0, prompt_row + 1);
                std::cout << blank90;
                goto_xy(prompt_col + prompt.length(), prompt_row);
                std::cout << std::string(input.length(), ' ');
                goto_xy(prompt_col + prompt.length(), prompt_row);
                continue;
            }
        }

        goto_xy(prompt_col, prompt_row + 1);
        std::cout << blank90;
        goto_xy(prompt_col, prompt_row + 1);
        std::cout << err;

        goto_xy(prompt_col + prompt.length(), prompt_row);
        std::cout << std::string(input.length(), ' ');
        goto_xy(prompt_col + prompt.length(), prompt_row);
    }
}

void sea_battle() {
    ::system("cls");

    goto_xy(0, 0);

    std::cout << "Let's play the game \"Sea battle\".\n";
    std::cout << "Each of the 2 players has 4 single-deck, 3 double-deck, 2 triple-deck and 1 four-deck ships.\n";
    std::cout << "Ships can only be positioned vertically or horizontally, diagonal placement is not allowed.\n";
    std::cout << "Each player takes turns placing their ships by indicating the coordinates of the bow and\n";
    std::cout << "stern of the ship (single-deck ships can be indicated by the coordinates of a point). The\n";
    std::cout << "coordinates can be in any order (A8a10, 10Aa8). Adjacent numerical coordinates should be\n";
    std::cout << "separated by a space (aa10 8, a10 8a).\n";
    std::cout << "                     S E A  1                                     S E A  2";

    std::vector<std::vector<short>> sea1(SEAXY, std::vector<short>(SEAXY, 0));
    std::vector<std::vector<short>> sea2(SEAXY, std::vector<short>(SEAXY, 0)); 

    tblCoord sea1_pos{ 1, 8 };
    tblCoord sea2_pos{ 11 * CELLX + 2, 8 };
    
    // tblCoord sea2_pos{ 0, 9 + 11 * CELLY + 2 };
    // tblCoord sea2_enemy_pos{ 11 * CELLX + 2, 9 };
    // tblCoord sea1_enemy_pos{ 11 * CELLX + 2, 9 + 11 * CELLY + 2 };

    char choice = 'N';
    auto prompt1_row = sea1_pos.y + 11 * CELLY;
    auto prompt2_row = sea2_pos.y + 11 * CELLY;

    std::string blank90(90, ' ');

    while (true) {
        draw_sea(sea1_pos, sea1, false);
        draw_sea(sea2_pos, sea2, false);
        if (!place_ships(sea1_pos, sea1)) {
            break;
        }
        draw_sea(sea1_pos, sea1, { 1, 1 }, { 10, 10 }, true);

        if (!place_ships(sea2_pos, sea2)) {
            break;
        }
        draw_sea(sea2_pos, sea2, { 1, 1 }, { 10, 10 }, true);

        short total_decks1 = 20;
        short total_decks2 = 20;
        // [0] - 21, [1] - 22, [2] - 23
        // [3] - 31, [4] - 32, [5] - 41
        std::vector<short> fleet1 = {
            2, 2, 2, 3, 3, 4
        };
        std::vector<short> fleet2 = fleet1;

        bool player1 = true;
        short result = 0;
        while (true) {
            if (player1) {
                result = shoot(sea2_pos, "Player 1's move: ", sea2, fleet2, total_decks2);
            }
            else {
                result = shoot(sea1_pos, "Player 2's move: ", sea1, fleet1, total_decks1);
            }

            if (result == -1) {
                break;
            }
            else if (result == 0) {
                player1 = !player1;
            }
            else if (result == 1) {
                if (player1) {
                    std::cout << "Player 1 won.";
                }
                else {
                    std::cout << "Player 2 won.";
                }
                break;
            }
        }

        goto_xy(0, prompt1_row + 1);
        std::cout << "Press 'y' if you want to play again: ";
        std::cin >> choice;
        std::cin.ignore();

        goto_xy(0, prompt1_row);
        std::cout << blank90 << "\n";
        std::cout << blank90 << "\n";
        if (choice != 'y') {
            break;
        }

        clear_sea(sea1);
        clear_sea(sea2);
    }

    ::system("cls");
    std::cout << "Good bye!\n";
}

template<typename T>
std::vector<std::vector<T>> snake_passage(size_t rows, size_t cols) {
    std::vector<std::vector<T>> arr (rows, std::vector<T> (cols, T {}));
    int direction = 1;
    T number = 0;
    for (size_t i = 0; i < rows; ++i) {
        for (size_t j = 0; j < cols; ++j) {
            int index = (cols - 1) * (1 - direction) / 2 + j * direction;
            arr[i][index] = number++;
        }
        direction = -direction;
    }
    return arr;
}

std::vector<std::vector<std::vector<bool>>> 
make_cube(const std::vector<std::vector<int>> & plane, int max_height) {
    auto rows = plane.size();
    auto cols = plane[0].size();
    std::vector<std::vector<std::vector<bool>>> 
    cube (rows, std::vector<std::vector<bool>>(cols, std::vector<bool>(max_height, false)));

    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            auto height = plane[i][j];
            if (height > 0) {
                height = height > max_height ? max_height : height;
                for (int k = 0; k < height; ++k) {
                    cube[i][j][k] = true;
                }
            }
        }
    }
    return cube;
}

void print_level(const std::vector<std::vector<std::vector<bool>>> & cube, int level) {
    auto rows = cube.size();
    auto cols = cube[0].size();
    auto levels = cube[0][0].size();
    if (level < 0 || level >= levels) {
    	std::cout << "Incorrect level.\n";
    }
    for (size_t i = 0; i < rows; ++i) {
        for (size_t j = 0; j < cols; ++j) {
            std::cout << (cube[i][j][level] ? 1 : 0) << " ";
        }
        std::cout << "\n";
    }

}

int main () {

//    std::cin.clear();
//    std::cin.ignore(std::cin.rdbuf()->in_avail(), '\n');

    std::cout << std::boolalpha;
    bool allCorrect = false;
/**/
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

    std::cout << "\nTask 2. Tic-tac-toe.\n";
    std::cout << "Press a key and \"Enter\" to start...";
    std::cin.ignore();
    std::cin.get();

    tic_tac_toe();

    std::cout << "\nTask 3. Matrix.\n";
    const int rows = 4, cols = 4;

    std::array<std::array<int, cols>, rows> m1 = { {
        { 1, 2, 3, 4 },
        { 5, 6, 7, 8 },
        { 1, 2, 3, 4 },
        { 5, 6, 7, 8 }
    } };
    std::cout << "Matix m1:\n";
    print(m1);

    std::cin.ignore();

    std::array<std::array<int, cols>, rows> m2;
    std::cout << "Enter " << rows << " lines of " << cols << " numbers each:\n";
    for (int i = 0; i < rows; ++i) {
        while (true) {
            if (getRowNumbers("Enter at least " + std::to_string(cols) +
                " numbers to initialize line " +
                std::to_string(i + 1) + ": ", m2[i])) {
                break;
            }
        }
    }
    std::cout << "Matix m2:\n";
    print(m2);

    if (equal_matrix(m1, m2)) {
        std::cout << "Converting matrix m2 to diagonal matrix.\n";
        to_diagonal_matrix(m2);
        std::cout << "Diagonal matrix:\n";
        print(m2);
    }
    else {
        std::cout << "The matrices aren't equal.\n";
    }

    std::cout << "\nTask 4. Multiplying a matrix by a vector.\n";
    const int Nrows = 5, Mcols = 4;

    std::array<std::array<float, Mcols>, Nrows> a = { {
        { 1, 2, 3, 4 },
        { 5, 6, 7, 8 },
        { 1, 2, 3, 4 },
        { 5, 6, 7, 8 },
        { 1, 2, 3, 4 }
    } };
    std::cout << "Given matix a:\n";
    print(a);

    std::array<float, Mcols> v = { 1, 2, 3, 4 };
    std::cout << "and vector v:\n";
    print(v, 0, Mcols - 1);

    auto axv = matrix_x_vector(a, v);
    std::cout << "Result of a x v:\n";
    print(axv, 0, Nrows - 1);

    std::cout << "Enter " << Nrows << " lines of " << Mcols << " numbers each for matrix a:\n";
    for (int i = 0; i < Nrows; ++i) {
        while (true) {
            if (getRowNumbers("Enter at least " + std::to_string(Mcols) +
                " numbers to initialize line " +
                std::to_string(i + 1) + ": ", a[i])) {
                break;
            }
        }
    }
    std::cout << "Matix a:\n";
    print(a);
    std::cout << "Enter " << Mcols << " numbers for vector v:\n";
    while (true) {
        if (getRowNumbers("Enter at least " + std::to_string(Mcols) +
            " numbers to initialize vector v: ", v)) {
            break;
        }
    }
    print(v, 0, Mcols - 1);

    axv = matrix_x_vector(a, v);
    std::cout << "Result of a x v:\n";
    print(axv, 0, Nrows - 1);

    std::cout << "\nTask 5. Pimple.\n";
    std::cout << "Press a key and \"Enter\" to start...";
    std::cin.ignore();
    std::cin.get();

    pimples();

    std::cout << "\nTask 6. Snake passage.\n";
    std::array<std::array<int, 5>, 5> nums {};
    int direction = 1;
    int number = 0;
    for (int i = 0; i < 5; ++i) {
        for (int j = 0; j < 5; ++j) {
            int index = 4 * (1 - direction) / 2 + j * direction;
            nums[i][index] = number++;
        }
        direction = -direction;
    }
    print(nums);

    auto snake = snake_passage<int>(6, 8);
    print(snake);

    std::cout << "\nTask 7. Almost Minecraft.\n";
    int rows_count = 6, cols_count = 5, levels_count = 10;
    std::vector<std::vector<int>> 
    mheights(rows_count, std::vector<int>(cols_count, 0));

    std::cout << "Given a matrix of " << rows_count << " rows and "
              << cols_count << " columns.\n";
    std::cout << "Enter the heights of the points into this matrix:\n";
    for (int i = 0; i < rows_count; ++i) {
        while (true) {
            if (getRowNumbers("Enter " + std::to_string(cols_count) +
                " numbers to initialize line " +
                std::to_string(i + 1) + ": ", mheights[i])) {
                break;
            }
        }
    }

    std::cout << "The matrix initialized:\n";
    print(mheights);

    auto cube = make_cube(mheights, levels_count);

    int level = 0;
    char choice = '0';
    while (true) {
        while (!getNumber("Enter the slice height (0 - 9) you want to output:", level)) {
            if (level >= 0 && level < 10) {
                break;
            }
        }

        print_level(cube, level);

        std::cout << "Press 'y' if you want to output another slice: ";
        std::cin >> choice;

        if (choice != 'y') {
            break;
        }
        std::cin.ignore();
    }
//*/

    std::cout << "\nTask 8. Sea battle.\n";
    std::cout << "Press a key and \"Enter\" to start...";
    std::cin.ignore();
    std::cin.get();

    sea_battle();

    return 0;
}
