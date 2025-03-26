#include <array>
#include <iostream>
#include <string>
#include <vector>
#include <windows.h>

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

int main () {

    tic_tac_toe();

    std::cout << "\nTask 6. Snake passage.\n";
    auto snake = snake_passage<int>(6, 8);
    print(snake);

    return 0;
}
