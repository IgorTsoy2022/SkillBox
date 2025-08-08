#include <iostream>
#include <string>
#include <vector>
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

void goto_xy(SHORT x, SHORT y) {
    HANDLE hStdOut = ::GetStdHandle(STD_OUTPUT_HANDLE);
    COORD position = { x, y };
    ::SetConsoleCursorPosition(hStdOut, position);
}

struct tblCoord {
    SHORT x = 0;
    SHORT y = 0;
};

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
    std::cout << "separated by a space (a10 8a).\n";
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

int main () {

    sea_battle();

    return 0;
}
