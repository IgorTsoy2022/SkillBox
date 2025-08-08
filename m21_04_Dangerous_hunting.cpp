#include <iostream>    // streambuf, hex, sgetc, sbumpc
#include <fstream>
#include <string>
#include <vector>

#include <windows.h>

bool is_number(const std::string str) {
    if (str.size() < 1) {
        return false;
    }
    for (const auto& c : str) {
        if (c >= '0' && c <= '9') {
            continue;
        }
        if (c == '.') {
            continue;
        }
        return false;
    }
    return true;
}

// Task 4. Game.

struct CHARACTER {
    std::string name = "Unknown";
    int health = 0;
    int armor = 0;
    int damage = 0;
    int x = 0;
    int y = 0;
};

void initializing_hero(CHARACTER& hero) {
    std::string input = "";
    std::cout << "Current hero's name is " << hero.name
              << ". To correct enter another name: ";
    if (std::getline(std::cin, input)) {
        if (input.size() > 0) {
            hero.name = input;
        }
    }
    std::cout << "Current hero's health is " << hero.health
              << ". To correct enter another value: ";
    if (std::getline(std::cin, input)) {
        if (is_number(input)) {
            int num = std::stod(input);
            hero.health = num < 0 ? 0 : num;
        }
    }
    std::cout << "Current hero's armor is " << hero.armor
              << ". To correct enter another value: ";
    if (std::getline(std::cin, input)) {
        if (is_number(input)) {
            int num = std::stod(input);
            hero.armor = num < 0 ? 0 : num;
        }
    }
    std::cout << "Current hero's damage is " << hero.damage
              << ". To correct enter another value: ";
    if (std::getline(std::cin, input)) {
        if (is_number(input)) {
            int num = std::stod(input);
            hero.damage = num < 0 ? 0 : num;
        }
    }
}

void initializing_enemies(const int number, 
                          std::vector<CHARACTER>& enemies) {
    if (number < 1) {
        return;
    }
    enemies.resize(number);

    int id = 1;
    for (auto & enemy : enemies) {
        enemy.name = "Enemy #" + std::to_string(id++);
        //  std::rand() % (max - min + 1) + min;
        enemy.health = std::rand() % 101 + 50;
        enemy.armor = std::rand() % 51;
        enemy.damage = std::rand() % 16 + 15;
    }
}

bool place_character(std::vector<std::vector<int>>& map,
                     CHARACTER& character,
                     const int character_id) {
    const int height = map.size();
    if (height < 1) {
        return false;
    }
    const int width = map[0].size();
    if (width < 1) {
        return false;
    }

    //  std::rand() % (max - min + 1) + min;
    const int x = std::rand() % width;
    const int y = std::rand() % height;

    for (int i = y; i < height; ++i) {
        for (int j = x; j < width; ++j) {
            if (map[i][j] == 0) {
                map[i][j] = character_id;
                character.x = j;
                character.y = i;
                return true;
            }
        }
        for (int j = 0; j < x; ++j) {
            if (map[i][j] == 0) {
                map[i][j] = character_id;
                character.x = j;
                character.y = i;
                return true;
            }
        }
    }
    for (int i = 0; i < y; ++i) {
        for (int j = x; j < width; ++j) {
            if (map[i][j] == 0) {
                map[i][j] = character_id;
                character.x = j;
                character.y = i;
                return true;
            }
        }
        for (int j = 0; j < x; ++j) {
            if (map[i][j] == 0) {
                map[i][j] = character_id;
                character.x = j;
                character.y = i;
                return true;
            }
        }
    }
    return false;
}

void initializing_map(const int width, const int height,
                      std::vector<std::vector<int>>& map,
                      std::vector<CHARACTER>& enemies, CHARACTER & hero) {
    map.clear();
    map.resize(height, std::vector<int>(width, 0));
    int id = 1;
    for (auto & enemy : enemies) {
        if (!place_character(map, enemy, id++)) {
            return;
        }
    }
    place_character(map, hero, id);
}

void save(const std::string& filename,
          const std::vector<std::vector<int>>& map,
          const std::vector<CHARACTER>& enemies,
          const CHARACTER& hero) {
    std::ofstream file(filename, std::ios::binary);
    if (!file.is_open()) {
        return;
    }

    int size = hero.name.size();
    file.write((char*)&size, sizeof(size));
    file.write((char*)hero.name.c_str(), size);
    file.write((char*)&hero.health, sizeof(hero.health));
    file.write((char*)&hero.armor, sizeof(hero.armor));
    file.write((char*)&hero.damage, sizeof(hero.damage));

    size = enemies.size();
    file.write((char*)&size, sizeof(size));

    for (const auto& enemy : enemies) {
        size = enemy.name.size();
        file.write((char*)&size, sizeof(int));
        file.write((char*)enemy.name.c_str(), size);
        file.write((char*)&enemy.health, sizeof(enemy.health));
        file.write((char*)&enemy.armor, sizeof(enemy.armor));
        file.write((char*)&enemy.damage, sizeof(enemy.damage));
    }

    int width = 0, height = map.size();
    if (height > 0) {
        width = map[0].size();
    }

    file.write((char*)&width, sizeof(width));
    file.write((char*)&height, sizeof(height));

    for (auto& rows : map) {
        for (auto& point : rows) {
            file.write((char*)&point, sizeof(point));
        }
    }

    file.close();
}

int load(const std::string& filename,
          std::vector<std::vector<int>>& map,
          std::vector<CHARACTER>& enemies, CHARACTER& hero) {
    std::ifstream file(filename, std::ios::binary);
    if (!file.is_open()) {
        return 0;
    }

    int size = 0;
    file.read((char*)&size, sizeof(int));
    if (file.eof()) {
        file.close();
        return 0;
    }
    hero.name.resize(size);
    file.read((char*)hero.name.c_str(), size);
    if (file.eof()) {
        file.close();
        return 0;
    }
    file.read((char*)&hero.health, sizeof(hero.health));
    if (file.eof()) {
        file.close();
        return 0;
    }
    file.read((char*)&hero.armor, sizeof(hero.armor));
    if (file.eof()) {
        file.close();
        return 0;
    }
    file.read((char*)&hero.damage, sizeof(hero.damage));
    if (file.eof()) {
        file.close();
        return 0;
    }

    file.read((char*)&size, sizeof(int));
    if (file.eof()) {
        file.close();
        return 0;
    }
    enemies.resize(size);

    for (auto& enemy : enemies) {
        file.read((char*)&size, sizeof(int));
        if (file.eof()) {
            file.close();
            return 0;
        }
        enemy.name.resize(size);
        file.read((char*)enemy.name.c_str(), size);
        if (file.eof()) {
            file.close();
            return 0;
        }
        file.read((char*)&enemy.health, sizeof(enemy.health));
        if (file.eof()) {
            file.close();
            return 0;
        }
        file.read((char*)&enemy.armor, sizeof(enemy.armor));
        if (file.eof()) {
            file.close();
            return 0;
        }
        file.read((char*)&enemy.damage, sizeof(enemy.damage));
        if (file.eof()) {
            file.close();
            return 0;
        }
    }

    int width = 0, height = 0;
    file.read((char*)&width, sizeof(int));
    if (!file.eof()) {
        file.read((char*)&height, sizeof(int));
    }
    if (width == 0 || height == 0) {
        file.close();
        return 0;
    }

    map.clear();
    map.resize(height, std::vector<int>(width, 0));

    const int hero_id = enemies.size() + 1;
    int rest_enemies = 0;
    int row = 0, col = 0;
    for (auto& rows : map) {
        for (auto& point : rows) {
            file.read((char*)&point, sizeof(point));
            if (file.eof()) {
                file.close();
                return 0;
            }
            if (point > 0) {
                if (point < hero_id) {
                    ++rest_enemies;
                    enemies[point - 1].x = col;
                    enemies[point - 1].y = row;
                }
                else if (point == hero_id) {
                    hero.x = col;
                    hero.y = row;
                }
            }
            ++col;
        }
        col = 0;
        ++row;
    }

    file.close();
    return rest_enemies;
}

void print(const CHARACTER& person) {
    std::cout << person.name
              << " health: " << person.health
              << " armor: " << person.armor
              << " damage: " << person.damage << "\n";
    std::cout << "Coords: x = " << person.x << ", y = " << person.y << "\n";
}

void print(const std::vector<CHARACTER> & enemies) {
    for (const auto & enemy : enemies) {
        std::cout << enemy.name
                  << " health: " << enemy.health
                  << " armor: " << enemy.armor
                  << " damage: " << enemy.damage << "\n";
        std::cout << "Coords: x = " << enemy.x << ", y = " << enemy.y << "\n";
    }
}

int move(std::vector<std::vector<int>>& map, std::vector<CHARACTER>& enemies,
         CHARACTER& hero, int& rest_enemies, char c) {
    const int height = map.size();
    if (height < 1) {
        return 0;
    }
    const int width = map[0].size();
    if (width < 1) {
        return 0;
    }

    const int hero_id = enemies.size() + 1;
    if (hero_id < 2) {
        return 0;
    }

    if (hero.health < 1) {
        return 2;
    }

    int dx = 0, dy = 0;
    switch (c) {
    case 'u' :
        dy = -1;
        break;
    case 'd' :
        dy = 1;
        break;
    case 'l' :
        dx = -1;
        break;
    case 'r' :
        dx = 1;
        break;
    }

    bool can_move = false;
    int target_x = hero.x + dx;
    int target_y = hero.y + dy;

    if (target_x >= 0 && target_x < width && target_y >= 0 && target_y < height) {
        int id = map[target_y][target_x];
        if (id > 0 && id < hero_id) {
            --id;
            int damage = hero.damage;
            int armor = enemies[id].armor;
            int health = enemies[id].health;
            if (damage > armor) {
                enemies[id].armor = 0;
                damage -= armor;
                if (damage >= health) {
                    enemies[id].health = 0;
                    can_move = true;
                    --rest_enemies;
                    health = -health;
                }
                else {
                    enemies[id].health -= damage;
                    health = -damage;
                }
                armor = -armor;
            }
            else {
                enemies[id].armor -= damage;
                armor = -damage;
                health = 0;
            }
            std::cout << "Enemy #" << id +1
                      << ". Armor: " << armor << " = " << enemies[id].armor
                      << ". Health: " << health << " = " << enemies[id].health << ".\n";
        }
        else {
            can_move = true;
        }
        if (can_move) {
            map[hero.y][hero.x] = 0;
            map[target_y][target_x] = hero_id;
            hero.x = target_x;
            hero.y = target_y;
        }
        if (rest_enemies < 1) {
            return 1;
        }
    }

 //  std::rand() % (max - min + 1) + min;

    int enemy_id = 0;
    for (auto & enemy : enemies) {
        ++enemy_id;
        if (enemy.health == 0) {
            continue;
        }
        int dx = 0, dy = 0;
        int dir = std::rand() % 4 + 1;
        switch (dir) {
        case 1:
            dy = -1;
            break;
        case 2:
            dy = 1;
            break;
        case 3:
            dx = -1;
            break;
        case 4:
            dx = 1;
            break;
        }
        can_move = false;
        target_x = enemy.x + dx;
        target_y = enemy.y + dy;
        if (target_x >= 0 && target_x < width && target_y >= 0 && target_y < height) {
            int id = map[target_y][target_x];
            if (id == hero_id) {
                int damage = enemy.damage;
                int armor = hero.armor;
                int health = hero.health;
                if (damage > armor) {
                    hero.armor = 0;
                    damage -= armor;
                    if (damage >= health) {
                        hero.health = 0;
                        can_move = true;
                        health = -health;
                    }
                    else {
                        hero.health -= damage;
                        health = -damage;
                    }
                    armor = -armor;
                }
                else {
                    hero.armor -= damage;
                    health = 0;
                    armor = -damage;
                }
                std::cout << hero.name << " is attacked by " << enemy.name
                          << ". Armor: " << armor << " = " << hero.armor
                          << ". Health: " << health << " = " << hero.health << ".\n";

            }
            else if (id == 0) {
                can_move = true;
            }
            if (can_move) {
                map[enemy.y][enemy.x] = 0;
                map[target_y][target_x] = enemy_id;
                enemy.x = target_x;
                enemy.y = target_y;
            }
            if (hero.health < 1) {
                return 2;
            }
        }
    }
    return 0;
}

void goto_xy(SHORT x, SHORT y) {
    HANDLE hStdOut = ::GetStdHandle(STD_OUTPUT_HANDLE);
    COORD position = { x, y };
    ::SetConsoleCursorPosition(hStdOut, position);
}

void draw_frame(const std::vector<std::vector<int>> & map, const int hero_id,
                const SHORT row) {
    const int height = map.size();
    if (height < 1) {
        return;
    }
    const int width = map[0].size();
    if (width < 1) {
        return;
    }

    const std::string header = "Dangerous hunting";

    goto_xy(0, row);

    std::string blank = "";
    if (width * 3 > header.size()) {
        blank.resize((width * 3 - header.size()) / 2, ' ');
    }

    std::cout << blank << header << "\n";

    std::cout << "+";
    for (int i = 0; i < width; ++i) {
        std::cout << "---";
    }
    std::cout << "+\n";

    for (const auto& rows : map) {
        std::cout << "|";

        for (const auto& point : rows) {
            if (point == 0) {
                std::cout << " . ";
            }
            else if (point == hero_id) {
                std::cout << " P ";
            }
            else {
                std::cout << " E ";
            }
        }

        std::cout << "|\n";
    }

    std::cout << "+";
    for (int i = 0; i < width; ++i) {
        std::cout << "---";
    }
    std::cout << "+\n";
}

int main() {

   	::system("cls");
 
   	const int width = 20;
   	const int height = 20;
   	const int number_of_enemies = 5;
    const SHORT map_row = 0;
    const SHORT prompt_row = height + map_row + 3;

    const std::string prompt_line1 = 
          "Enter \"save\" for save game, \"load\" for load game,\n";
    const std::string prompt_line2 =
          "\"new\" for new game, 'u' for move up, 'd' for move down,\n";
    const std::string prompt_line3 =
          "'l' for move left, 'r' for move right, \"exit\" for exit: ";
    const std::string blank(90, ' ');

    std::string filename = "battle.bin";
    CHARACTER hero;
    std::vector<CHARACTER> enemies;
    std::vector<std::vector<int>> map;

    std::srand(std::time(nullptr));

    auto rest_enemies = load(filename, map, enemies, hero);
        
    if (rest_enemies < 1 || hero.health < 1) {

        std::cout << "Dangerous hunting\n";

        initializing_hero(hero);

        rest_enemies = number_of_enemies;
        initializing_enemies(number_of_enemies, enemies);

        initializing_map(width, height, map, enemies, hero);

        save(filename, map, enemies, hero);
        ::system("cls");
    }

    draw_frame(map, number_of_enemies + 1, map_row);

    goto_xy(0, prompt_row);

    bool clear_screen = false;
    int result = 0;
    // result = 1 - hero won
    // result = 2 - hero lost
    std::string input = "";
    std::cout << prompt_line1 << prompt_line2 << prompt_line3;
    while (std::getline(std::cin, input)) {
        if (input == "exit") {
            break;
        }
        if (input == "save") {
            if (clear_screen) {
                ::system("cls");
                clear_screen = false;
                draw_frame(map, number_of_enemies + 1, map_row);
                goto_xy(0, prompt_row + 3);
            }

            save(filename, map, enemies, hero);
            std::cout << "The game is saved.\n";
            clear_screen = true;

            goto_xy(0, prompt_row + 2);
            std::cout << blank;
            goto_xy(0, prompt_row);
        }
        else if (input == "load") {
            if (clear_screen) {
                ::system("cls");
                clear_screen = false;
                draw_frame(map, number_of_enemies + 1, map_row);
                goto_xy(0, prompt_row + 3);
            }

            rest_enemies = load(filename, map, enemies, hero);
            std::cout << "The game is loaded.\n";
            clear_screen = true;

            goto_xy(0, prompt_row + 2);
            std::cout << blank;
            goto_xy(0, prompt_row);
        }
        else if (input == "new") {
            if (clear_screen) {
                ::system("cls");
                clear_screen = false;
                draw_frame(map, number_of_enemies + 1, map_row);
                std::cout << prompt_line1 << prompt_line2 << prompt_line3
                          << "\n";
            }

            initializing_hero(hero);
            rest_enemies = number_of_enemies;
            initializing_enemies(number_of_enemies, enemies);
            initializing_map(width, height, map, enemies, hero);
            save(filename, map, enemies, hero);

            print(enemies);
            clear_screen = true;

            goto_xy(0, prompt_row + 2);
            std::cout << blank;
            goto_xy(0, prompt_row);
        }
        else if (input == "u" || input == "d" ||
                 input == "l" || input == "r") {

            if (clear_screen) {
                ::system("cls");
                clear_screen = false;
                draw_frame(map, number_of_enemies + 1, map_row);
                goto_xy(0, prompt_row + 3);
            }

            result = move(map, enemies, hero, rest_enemies, input[0]);
            clear_screen = true;
            draw_frame(map, number_of_enemies + 1, map_row);

            if (result == 1) {
                std::cout << hero.name << " won!" << "\n";
                break;
            }
            else if (result == 2) {
                std::cout << hero.name << " lost!" << "\n";
                break;
            }

            goto_xy(0, prompt_row + 2);
            std::cout << blank;
            goto_xy(0, prompt_row);
        }
        else {
            if (clear_screen) {
                ::system("cls");
                clear_screen = false;
                draw_frame(map, number_of_enemies + 1, map_row);
            }
            else {
                goto_xy(0, prompt_row + 2);
                std::cout << blank;
                goto_xy(0, prompt_row);
            }
        }
        std::cout << prompt_line1 << prompt_line2 << prompt_line3;
    }

    save(filename, map, enemies, hero);

    if (clear_screen) {
        ::system("cls");
        clear_screen = false;
        draw_frame(map, number_of_enemies + 1, map_row);
        if (result == 1) {
            std::cout << hero.name << " won!" << "\n";
        }
        else if (result == 2) {
            std::cout << hero.name << " lost!" << "\n";
        }
    }

    print(hero);
    print(enemies);

    return 0;
}
