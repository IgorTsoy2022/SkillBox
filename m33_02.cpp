#include <iostream>
#include <exception>
#include <array>
#include <vector>
#include <string>
#include <windows.h>

void goto_xy(SHORT x, SHORT y) {
    HANDLE hStdOut = ::GetStdHandle(STD_OUTPUT_HANDLE);
    COORD position = { x, y };
    ::SetConsoleCursorPosition(hStdOut, position);
}

class FishException : public std::exception {
public:
    const char* what() const noexcept override {
        return "Caught a fish!";
    }
};

class BootException : public std::exception {
public:
    const char* what() const noexcept override {
        return "Caught a boot!";
    }
};

class NothingException : public std::exception {
public:
    const char* what() const noexcept override {
        return "Nothing caught! Try again.";
    }
};

class AlreadyBeenException : public std::exception {
public:
    const char* what() const noexcept override {
        return "Already been. Nothing caught! Try again.";
    }
};

static void draw_frame(const SHORT row) {
    goto_xy(0, row);
    std::cout << "   | A | B | C |\n";
    std::cout << "---+---+---+---+\n";
    std::cout << " 1 |   |   |   |\n";
    std::cout << "---+---+---+---+\n";
    std::cout << " 2 |   |   |   |\n";
    std::cout << "---+---+---+---+\n";
    std::cout << " 3 |   |   |   |\n";
    std::cout << "---+---+---+---+\n";
}

static void draw_cell(const SHORT x, const SHORT y, const SHORT row, const char c) {
    goto_xy(x * 4 + 1, y * 2 + row);
    std::cout << c;
}

static void draw_pond(const std::array<std::array<char, 3>, 3>& pond, const SHORT row) {
    for (short i = 0; i < 3; ++i) {
        for (short j = 0; j < 3; ++j) {
            draw_cell(j + 1, i, row, pond[i][j]);
        }
    }
    goto_xy(0, row + 6);
}

static void clear_pond(std::array<std::array<char, 3>, 3>& pond) {
    for (auto& arrows : pond) {
        for (auto& cell : arrows) {
            cell = ' ';
        }
    }
}

//  std::rand() % (max - min + 1) + min;
static void RandomSetFishAndBoots(std::array<std::array<char, 3>, 3>& pond) {
    clear_pond(pond);
    pond[rand() % 3][rand() % 3] = 'F';
    for (int i = 0; i < 3; ++i) {
        while (true) {
            int row = rand() % 3;
            int col = rand() % 3;
            if (pond[row][col] == ' ') {
                pond[row][col] = 'B';
                break;
            }
        }
    }

}

static void check_cell(const std::string& input,
                       std::array<std::array<char, 3>, 3>& pond, const SHORT row) {
    if (input.size() < 1 || input.size() > 5) {
        throw std::invalid_argument("Input length is " +
              std::to_string(input.size()) +
              ". Minimum length is 2, maximum length is 5.");
    }

    int x = 0;
    int y = 0;
    for (const char& c : input) {
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
        throw std::invalid_argument("Incorrect input = \"" + input + "\".");
    }

// At this point 'x' and 'y' are greater than 0.

    switch (pond[y - 1][x - 1]) {
    case ' ':
        pond[y - 1][x - 1] = 'X';
        draw_cell(x, y - 1, row, 'X');
        goto_xy(0, row + 7);
        throw NothingException();
        break;
    case 'B':
        throw BootException();
        break;
    case 'F':
        throw FishException();
        break;
    case 'X':
        throw AlreadyBeenException();
        break;
    default:
        break;
    }
}



int main() {
    ::system("cls");

    std::srand(std::time(nullptr));

    goto_xy(0, 0);
    std::cout << "Enter the coords of the cell in the\n";
    std::cout << "format: A1 or 1a or type \"exit\" for exit.\n";
    std::array<std::array<char, 3>, 3> pond;

    std::string blank(80, ' ');
    std::string input = "";

    SHORT pond_row = 5;
    SHORT prompt_row = 11;

    while (true) {
        RandomSetFishAndBoots(pond);
        draw_frame(pond_row - 2);

        goto_xy(0, prompt_row);
        std::cout << blank << "\n" << blank;

        bool exit = false;
        bool won = false;
        for (int i = 0; i < 9; ++i) {
            while (true) {
                goto_xy(0, prompt_row);
                std::cout << "Enter coords > ";

                std::getline(std::cin, input);
                goto_xy(0, prompt_row + 1);
                std::cout << blank;
                goto_xy(0, prompt_row + 1);
                if (input == "exit") {
                    exit = true;
                    break;
                }

                try {
                    check_cell(input, pond, pond_row);
                }
                catch (const std::invalid_argument& e) {
                    std::cerr << "Invalid argument: " << e.what() << std::endl;
                }
                catch (const NothingException& e) {
                    std::cerr << e.what() << std::endl;
                }
                catch (const BootException& e) {
                    exit = true;
                    std::cerr << e.what() << std::endl;
                    break;
                }
                catch (const FishException& e) {
                    won = true;
                    exit = true;
                    std::cerr << e.what() << std::endl;
                    break;
                }
                catch (const AlreadyBeenException& e) {
                    std::cerr << e.what() << std::endl;
                }

            }
            if (exit) {
                break;
            }
        }

        draw_pond(pond, pond_row);

        goto_xy(0, prompt_row + 2);
        std::cout << "You " << (won ? "won" : "lost") << "!\n";
        std::cout << "Enter 'y' if you want to play again > ";
        std::getline(std::cin, input);

        if (input != "y") {
            break;
        }
        goto_xy(0, prompt_row + 2);
        std::cout << blank << "\n" << blank;
    }

    goto_xy(0, prompt_row + 3);
    std::cout << blank;
    goto_xy(0, prompt_row + 3);
    std::cout << "Game over!\n";

    return 0;
}