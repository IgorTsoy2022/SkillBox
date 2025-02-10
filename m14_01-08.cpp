#include <chrono>
#include <iostream>
#include <array>
#include <cmath>
#include <string>
#include <cassert>
#include <vector>
#include <map>
#include <conio.h> 
#include <thread>
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

template<typename T, std::size_t rows, std::size_t cols>
void print(const std::array<std::array<T, cols>, rows>& arr) {
    for (const auto& arrow : arr) {
        for (size_t i = 0; i < cols - 1; ++i) {
            std::cout << arrow[i] << " ";
        }
        std::cout << arrow[cols - 1] << "\n";
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

short check_table(const char(&tbl)[3][3], bool next_move_is_X) {
    // status = 1 : X - winner
    // status = 2 : 0 - winner
    // status = 3 : draw

    SHORT status = 0;
    SHORT horizontals[3] = { 0, 0, 0 };
    SHORT verticals[3] = { 0, 0, 0 };
    SHORT diagonals[2] = { 0, 0 };
    SHORT moves_count = 0;

    for (short i = 0; i < 3; ++i) {
        for (short j = 0; j < 3; ++j) {
            if (tbl[i][j] == 'X') {
                ++horizontals[i];
                ++verticals[j];
                if (i == j) {
                    ++diagonals[0];
                    if (i == 1) {
                        ++diagonals[1];
                    }
                }
                else if (i == 0 && j == 2 ||
                         i == 2 && j == 0) {
                    ++diagonals[1];
                }
            }
            else if (tbl[i][j] == '0') {
                --horizontals[i];
                --verticals[j];
                if (i == j) {
                    --diagonals[0];
                    if (i == 1) {
                        --diagonals[1];
                    }
                }
                else if (i == 0 && j == 2 ||
                         i == 2 && j == 0) {
                    --diagonals[1];
                }
            }
            else {
                ++moves_count;
            }
        }
        if (horizontals[i] == 3) {
            return 1;
        }
        else if (horizontals[i] == -3) {
            return 2;
        }
    }

    for (short i = 0; i < 3; ++i) {
        if (verticals[i] == 3) {
            return 1;
        }
        if (verticals[i] == -3) {
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

    if (moves_count == 1) {
        for (short i = 0; i < 3; ++i) {
            if (horizontals[i] < -1 && !next_move_is_X ||
                horizontals[i] > 1 && next_move_is_X) {
                return 0;
            }
            if (verticals[i] < -1 && !next_move_is_X ||
                verticals[i] > 1 && next_move_is_X) {
                return 0;
            }
            if (i < 2) {
               if (diagonals[i] < -1 && !next_move_is_X ||
                   diagonals[i] > 1 && next_move_is_X) {
                   return 0;
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

//    clrscr();

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
    bool  current_move_is_X = (choice != 'X' && choice != 'x') ? false : true;
    std::cin.ignore();
    goto_xy(0, 4);
    std::cout << blank;

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

            // status = check_table(tbl, current_move_is_X);
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




template<size_t Rows, size_t Cols>
void clear_cells(std::array<std::array<bool, Cols>, Rows>& pimples) {
    for (auto& arrows : pimples) {
        for (auto& cell : arrows) {
            cell = true;
        }
    }
}

template<size_t Rows>
void draw_field(std::array<std::array<bool, 12>, Rows>& pimples, SHORT row) {
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
    if (size < 4 && size > 12) {
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


template<size_t Rows, size_t Cols>
void draw_region(std::array<std::array<bool, Cols>, Rows>& pimples,
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

template<size_t Rows, size_t Cols>
void burst_region(std::array<std::array<bool, Cols>, Rows> & pimples,
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


std::vector<std::vector<std::vector<bool>>> make_cube(const std::vector<std::vector<int>> & plane, int max_height) {
    auto rows = plane.size();
    auto cols = plane[0].size();
     std::vector<std::vector<std::vector<bool>>> cube (rows, std::vector<std::vector<bool>>(cols, std::vector<bool>(max_height, false)));

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
    using namespace std::chrono_literals;

    std::cout << std::boolalpha;
    bool allCorrect = false;
///*
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
    std::cout << "Press any key to start...";
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
    std::cout << "Press any key to start...";
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
//*/

    std::cout << "\nTask 7. Almost Minecraft.\n";
    int rows_count = 6, cols_count = 5, levels_count = 10;
    std::vector<std::vector<int>> mheights = { 
        { 5, 5, 5, 5, 5 },
        { 4, 4, 4, 4, 4 },
        { 3, 3, 2, 3, 3 },
        { 2, 1, 1, 1, 2 },
        { 1, 1, 1, 1, 1 },
        { 1, 0, 0, 0, 1 }
    };
    
    auto cube = make_cube(mheights, levels_count);
    print_level(cube, 0);

    std::cout << "\nTask 8. Sea battle.\n";

    return 0;
}
