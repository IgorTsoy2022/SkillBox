#include <array>
#include <iostream>
#include <string>
#include <thread>
#include <windows.h>

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

//----------------------------------------------------//
//    Pimples
//----------------------------------------------------//

void goto_xy(SHORT x, SHORT y) {
    HANDLE hStdOut = ::GetStdHandle(STD_OUTPUT_HANDLE);
    COORD position = { x, y };
    ::SetConsoleCursorPosition(hStdOut, position);
}

struct tblCoord {
    SHORT x = 0;
    SHORT y = 0;
};

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

int main () {

    pimples();

    return 0;
}
