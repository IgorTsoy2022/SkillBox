#include <ctime>
#include <cmath>
#include <iostream>    // streambuf, hex, sgetc, sbumpc
#include <fstream>
#include <sstream>
#include <string>
#include <vector>

#include <windows.h>

template<typename A>
void print(const A& arr, size_t from, size_t to) {
    std::cout << "[";
    if (from > to) {
        for (size_t i = from; i > to; --i) {
            std::cout << arr[i] << " ";
        }
    }
    else {
        for (size_t i = from; i < to; ++i) {
            std::cout << arr[i] << " ";
        }
    }
    std::cout << arr[to] << "]\n";
}

void toupper(std::string& text) {
    for (auto& c : text) {
        if (c >= 'a' && c <= 'z') {
            c = c - 32;
        }
    }
}

void tolower(std::string& text) {
    for (auto& c : text) {
        if (c >= 'A' && c <= 'Z') {
            c = c + 32;
        }
    }
}

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

// Task 1. Entry in the statement of accounts.

struct CURRDATE {
    int year = 0;
    int month = 0;
    int day = 0;
};

struct PAYROLL {
    std::string name = "";
    CURRDATE date;
    double amount = 0.0;
};

int leap_year(int year) {
    if (year % 400 == 0) {
        return 1;
    }
    if (year % 100 == 0) {
        return 0;
    }
    if (year % 4 == 0) {
        return 1;
    }
    return 0;
}

bool is_date(const CURRDATE& date) {
    if (date.month < 1 || date.month > 12 ||
        date.day < 1) {
        return false;
    }
    switch (date.month) {
    case 1:
    case 3:
    case 5:
    case 7:
    case 8:
    case 10:
    case 12:
        return date.day < 32;
    case 2:
        return date.day < 29 +
        leap_year(date.year);
    default:
        return date.day < 31;
    }
}

std::vector<std::string> 
split(const std::string& str, const char delimeter = ' ') {
    std::vector<std::string> result;
    std::string word = "";
    for (const auto& c : str) {
        if (c == delimeter) {
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

bool ddmmyyyy_to_date(const std::string& ddmmyyyy,
                      const char delimeter, CURRDATE& date) {
    auto size = ddmmyyyy.size();
    if (size < 6) {
        return false;
    }
    auto nums = split(ddmmyyyy, delimeter);
    if (nums.size() != 3) {
        return false;
    }

    if (!is_number(nums[0])) {
       return false;
    }
    date.day = std::stoi(nums[0]);
    if (!is_number(nums[1])) {
        return false;
    }
    date.month = std::stoi(nums[1]);
    if (!is_number(nums[2])) {
        return false;
    }
    date.year = std::stoi(nums[2]);

    return is_date(date);
}

std::string date_to_ddmmyyyy(const CURRDATE& date,
                             const char delimeter = '.') {
    std::string ddmmyyyy = "";

    if (date.day < 10) {
        ddmmyyyy = "0";
    }
    ddmmyyyy += std::to_string(date.day) + delimeter;
    if (date.month < 10) {
        ddmmyyyy += "0";
    }
    ddmmyyyy += std::to_string(date.month) + delimeter;
    ddmmyyyy += std::to_string(date.year);

    return ddmmyyyy;
}

std::vector<PAYROLL> read(const std::string& filename,
                          std::ostream & stream = std::cout) {
    std::vector<PAYROLL> paylist;

    std::ifstream file(filename, std::ios::binary);
    if (file.is_open()) {
        PAYROLL p{};
        while (!file.eof()) {
            int size = 0;
            file.read((char*)&size, sizeof(size));
            if (file.eof()) {
                break;
            }
            p.name.resize(size);
            file.read((char*)p.name.c_str(), size);

            file.read((char*)&size, sizeof(size));
            std::string strdate = "";
            strdate.resize(size);
            file.read((char*)strdate.c_str(), size);
            if (!ddmmyyyy_to_date(strdate, '.', p.date)) {
                break;
            }

            file.read((char*)&p.amount, sizeof(p.amount));
            if (file.eof()) {
                break;
            }

            paylist.push_back(p);
            stream << p.name << " " << strdate << " " << p.amount << "\n";
        }
        file.close();
    }

    return paylist;
}

void write(const std::string& filename, std::vector<PAYROLL> & paylist,
           std::istream& stream = std::cin) {
    std::ofstream file(filename, std::ios::app | std::ios::binary);
    if (file.is_open()) {
        std::string input;
        while (std::getline(stream, input)) {
            if (input == "exit") {
                break;
            }

            std::string name = "";
            std::string lname = "";
            std::string strdate = "";
            std::string stramount = "";

            std::stringstream input_stream(input);
            input_stream >> name >> lname >> strdate >> stramount;

            CURRDATE date{};
            PAYROLL p{};
            if (ddmmyyyy_to_date(strdate, '.', date)) {
                if (is_number(stramount)) {
                    p.name = name + " " + lname;
                    int size = p.name.size();
                    file.write((char*)&size, sizeof(size));
                    file.write((char*)p.name.c_str(), size);

                    p.date = date;
                    size = strdate.size();
                    file.write((char*)&size, sizeof(size));
                    file.write((char*)strdate.c_str(), size);

                    p.amount = std::stod(stramount);
                    file.write((char*)&p.amount, sizeof(p.amount));

                    paylist.push_back(p);
                }
                else {
                    std::cout << "Incorrect amount!\n";
                }
            }
            else {
                std::cout << "Incorrect date!\n";
            }
        }
        file.close();
    }
}

void print(const std::vector<PAYROLL>& plist) {
    for (const auto& p : plist) {
        std::cout << p.name << " " << date_to_ddmmyyyy(p.date)
                  << " " << p.amount << "\n";
    }
}

bool text_view(const std::string& filename) {
    bool isOk = false;
    std::ifstream fs;
    fs.open(filename, std::ios::binary);
    if (fs.is_open()) {
        char buffer[33];
        while (!fs.eof()) {
            fs.read(buffer, 32);
            buffer[fs.gcount()] = 0;
            std::cout << buffer;
        }
        isOk = true;
    }
    fs.close();
    return isOk;
}

// Task 2. Settlement.

enum class ROOM_TYPE {
    UNKNOWN = 0,
    BEDROOM = 1,
    LIVING = 2,
    NURSERY = 3,
    CABINET = 4,
    CANTEEN = 5,
    KITCHEN = 6,
    BATHROOM = 7
};

ROOM_TYPE get_room_type(int number) {
    switch (number) {
    case 1:
        return ROOM_TYPE::BEDROOM;
    case 2:
        return ROOM_TYPE::LIVING;
    case 3:
        return ROOM_TYPE::NURSERY;
    case 4:
        return ROOM_TYPE::CABINET;
    case 5:
        return ROOM_TYPE::CANTEEN;
    case 6:
        return ROOM_TYPE::KITCHEN;
    case 7:
        return ROOM_TYPE::BATHROOM;
    default :
        return ROOM_TYPE::UNKNOWN;
        break;
    }
}

std::string text_room_type(const ROOM_TYPE& rt) {
    switch (rt) {
    case ROOM_TYPE::BEDROOM:
        return "BEDROOM";
    case ROOM_TYPE::LIVING:
        return "LIVING ROOM";
    case ROOM_TYPE::NURSERY:
        return "CHILDREN ROOM";
    case ROOM_TYPE::CABINET:
        return "CABINET";
    case ROOM_TYPE::CANTEEN:
        return "DINING ROOM";
    case ROOM_TYPE::KITCHEN:
        return "KITCHEN";
    case ROOM_TYPE::BATHROOM:
        return "BATHROOM";
    case ROOM_TYPE::UNKNOWN:
        return "UNKNOWN";
    }
}

struct ROOM {
    ROOM_TYPE type = ROOM_TYPE::UNKNOWN;
    std::string name = "";
    double area = 0.0;
};

struct FLOOR {
    double height = 0.0;
    double area = 0.0;
    std::vector<ROOM> rooms;
};

enum class BUILDING_TYPE {
    UNKNOWN = 0,
    HOUSE = 1,
    GARAGE = 2,
    LODGE = 3,
    BATHHOUSE = 4
};

BUILDING_TYPE get_building_type(int number) {
    switch (number) {
    case 1:
        return BUILDING_TYPE::HOUSE;
    case 2:
        return BUILDING_TYPE::GARAGE;
    case 3:
        return BUILDING_TYPE::LODGE;
    case 4:
        return BUILDING_TYPE::BATHHOUSE;
    default:
        return BUILDING_TYPE::UNKNOWN;
        break;
    }
}

std::string text_building_type(const BUILDING_TYPE & bt) {
    switch(bt) {
    case BUILDING_TYPE::HOUSE:
        return "HOUSE";
    case BUILDING_TYPE::GARAGE:
        return "GARAGE";
    case BUILDING_TYPE::LODGE:
        return "LODGE";
    case BUILDING_TYPE::BATHHOUSE:
        return "BATHHOUSE";
    case BUILDING_TYPE::UNKNOWN:
        return "UNKNOWN";
    }
}

struct BUILDING {
    BUILDING_TYPE type = BUILDING_TYPE::UNKNOWN;
    double area = 0.0;
    int kilns = 0;
    std::vector<FLOOR> floors;
};

struct PLOT {
    double area = 0.0;
    std::vector<BUILDING> buildings;
};

void update_room(ROOM& room, std::istream& stream) {
    std::string input = "";
    std::string stype = "";
    std::string sname = "";
    std::string sarea = "";

    std::cout << "            Enter the type of room (1 for BEDROOM, 2 for LIVING ROOM,\n";
    std::cout << "            3 for CHILDREN ROOM, 4 for CABINET, 5 for DINING ROOM,\n";
    std::cout << "            6 for KITCHEN, 7 for BATHROOM), room name and area,\n";
    std::cout << "            separated by a space: ";

    if (std::getline(stream, input)) {
        std::stringstream input_stream(input);
        int number = 0;
        input_stream >> stype >> sname >> sarea;
        if (is_number(stype)) {
            number = std::stod(stype);
            room.type = get_room_type(number);
        }
        room.name = sname;

        if (is_number(sarea)) {
            room.area = std::stod(sarea);
        }
    }
}

void update_floor(FLOOR& floor, std::istream& stream) {
    std::string input = "";
    std::string sarea = "";
    std::string sheight = "";
    std::string srooms = "";

    std::cout << "        Enter the area in square meters, ceiling height\n";
    std::cout << "        and the number of rooms, separated by a space: ";

    if (std::getline(stream, input)) {
        std::stringstream input_stream(input);
        input_stream >> sarea >> sheight >> srooms;
        if (is_number(sarea) && is_number(sheight)) {
            floor.area = std::stod(sarea);
            floor.height = std::stod(sheight);
        }
        int number = 0;
        if (is_number(srooms)) {
            number = std::stod(srooms);
        }
        floor.rooms.resize(number);
        number = 1;
        for (auto& room : floor.rooms) {
            std::cout << "            Room #" << number++ << ":\n";
            update_room(room, std::cin);
        }
    }
}

void update_building(BUILDING & building, std::istream& stream) {
    std::string input = "";
    std::string stype = "";
    std::string sarea = "";
    std::string sfloors = "";
    std::string skilns = "";

    std::cout << "    Enter the type of building (1 for HOUSE, 2 for GARAGE,\n";
    std::cout << "    3 for LODGE, 4 for BATHHOUSE), the area in square meters,\n";
    std::cout << "    the number of floors, and, if any, the number of kilns,\n";
    std::cout << "    separated by a space: ";

    if (std::getline(stream, input)) {
        std::stringstream input_stream(input);
        int number = 0;
        input_stream >> stype >> sarea >> sfloors >> skilns;
        if (is_number(stype) && is_number(sarea)) {
            number = std::stod(stype);
            building.type = get_building_type(number);
            building.area = std::stod(sarea);
        }
        if (is_number(skilns)) {
            building.kilns = std::stod(skilns);
        }
        number = 0;
        if (is_number(sfloors)) {
            number = std::stod(sfloors);
        }
        building.floors.resize(number > 0 ? number : 1);
        number = 1;
        for (auto& floor : building.floors) {
            std::cout << "        Floor #" << number++ << ":\n";
            update_floor(floor, std::cin);
        }
    }
}

std::vector<PLOT> make_plots(const int number, std::istream& stream) {
    std::vector<PLOT> plots(number);
    std::string input = "";
    std::string sarea = "";
    std::string sbuildings = "";

    std::cout << "Enter the area in square meters and the number of buildings,\n";
    std::cout << "separated by a space for each plot.\n";
    std::cout << "For exit type \"exit\".\n";
    int index = 0;
    //            std::cout << "Plot #1: ";
    while (std::getline(stream, input)) {
        if (input == "exit") {
            break;
        }
        std::stringstream input_stream(input);
        input_stream >> sarea >> sbuildings;
        if (is_number(sarea) && is_number(sbuildings)) {
            plots[index].area = std::stod(sarea);
            int size = std::stod(sbuildings);
            if (size > 0) {
                plots[index].buildings.resize(size);
                int number = 1;
                for (auto& building : plots[index].buildings) {
                    std::cout << "    Building #" << number++ << ":\n";
                    update_building(building, std::cin);
                }
            }
            ++index;
        }
        if (index == number) {
            break;
        }
        std::cout << "Plot #" << index + 1 << ": ";
    }
    return plots;
}

void print(const std::vector<ROOM>& rooms) {
    for (const auto r : rooms) {
        std::cout << "            " << text_room_type(r.type)
                  << " #" << r.name << " "
                  << r.area << " sq.m.\n";
    }
}

void print(const std::vector<FLOOR> & floors) {
    for (int i = 0; i < floors.size(); ++i) {
        std::cout << "        " << i + 1 << " floor "
                  << floors[i].area << " sq.m., height: "
                  << floors[i].height << " m.\n";
        print(floors[i].rooms);
    }
}

void print(const std::vector<BUILDING> & buildings) {
    for (const auto & b : buildings) {
        std::cout << "    " << b.floors.size() << "-floor "
                  << text_building_type(b.type) << " "
                  << b.area << " sq.m.";
        if (b.kilns > 0) {
            std::cout << ", " << b.kilns << " kilns.";
        }
        std::cout << "\n";
        print(b.floors);
    }
}

void print(const std::vector<PLOT> & plots) {
    for (int i = 0; i < plots.size(); ++i) {
        std::cout << "Plot #" << i + 1 << ": area = "
                  << plots[i].area << " sq.m.\n";
        print(plots[i].buildings);
    }
}

// Task 3. The mathematical vector.

struct VEC2D {
    double x = 0.0;
    double y = 0.0;
};

VEC2D operator-(const VEC2D& a) {
    VEC2D result;
    result.x = -a.x;
    result.y = -a.y;
    return result;
}

VEC2D operator+(const VEC2D& a, const VEC2D& b) {
    VEC2D result;
    result.x = a.x + b.x;
    result.y = a.y + b.y;
    return result;
}

VEC2D operator-(const VEC2D& a, const VEC2D& b) {
    VEC2D result;
    result.x = a.x - b.x;
    result.y = a.y - b.y;
    return result;
}

VEC2D operator*(const VEC2D& a, const double c) {
    VEC2D result;
    result.x = a.x * c;
    result.y = a.y * c;
    return result;
}

VEC2D operator*(const double c, const VEC2D& a) {
    VEC2D result;
    result.x = a.x * c;
    result.y = a.y * c;
    return result;
}

double vector_length(const VEC2D& a) {
    return std::sqrt(a.x * a.x + a.y * a.y);
}

VEC2D vector_normalize(const VEC2D& a) {
    VEC2D result;
    auto length = vector_length(a);
    result.x = a.x / length;
    result.y = a.y / length;
    return result;
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
              << ". To coorect enter another name: ";
    if (std::getline(std::cin, input)) {
        if (input.size() > 0) {
            hero.name = input;
        }
    }
    std::cout << "Current hero's health is " << hero.health
              << ". To coorect enter another value: ";
    if (std::getline(std::cin, input)) {
        if (is_number(input)) {
            int num = std::stod(input);
            hero.health = num < 0 ? 0 : num;
        }
    }
    std::cout << "Current hero's armor is " << hero.armor
              << ". To coorect enter another value: ";
    if (std::getline(std::cin, input)) {
        if (is_number(input)) {
            int num = std::stod(input);
            hero.armor = num < 0 ? 0 : num;
        }
    }
    std::cout << "Current hero's damage is " << hero.damage
              << ". To coorect enter another value: ";
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

int move(std::vector<std::vector<int>>& map, std::vector<CHARACTER>& enemies, CHARACTER& hero, int& rest_enemies, char c) {
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

    std::cout << std::boolalpha;

    std::cout << "Task 1. Payroll.\n";
    {
        std::string filename = "payroll_test.bin";
        std::vector<PAYROLL> paylist;

        std::cout << "Type \"list\" to view the statement"
                  << " or \"add\" to add an entry\n";
        std::cout << "or \"exit\" for exit: ";
        bool hasbeenread = false;
        bool hasbeenwritten = false;
        std::string input;
        while (std::getline(std::cin, input)) {
            if (input == "exit") {
                if (hasbeenread || hasbeenwritten) {
                    if (!hasbeenread) {
                        std::cout << "Records have been added:\n";
                    }
                    else {
                        std::cout << "File content:\n";
                    }
                    if (paylist.empty()) {
                        std::cout << "Empty.\n";
                    }
                    else {
                        print(paylist);
                    }
                }
                break;
            }
            if (input == "list") {
                paylist = read(filename);
                hasbeenread = true;
            }
            else if (input == "add") {
                std::cout << "Consecutively enter the rows with the entries : Name and\n";
                std::cout << "Surname of the payee, date and amount of payment. The\n";
                std::cout << "data must be separated by spaces (type \"exit\" for exit):\n";
                write(filename, paylist);
                hasbeenwritten = true;
            }
            std::cout << "Type \"list\" to view the statement"
                      << " or \"add\" to add an entry\n";
            std::cout << "or \"exit\" for exit: ";
        }
    }

    std::cout << "\nTask 2. Settlement.\n";
    {
        int number = 0;
        std::string input = "";
        std::cout << "Enter the number of plots:";
        std::cin >> input;
        if (is_number(input)) {
            number = std::stod(input);
        }
        if (number > 0) {
            auto plots = make_plots(number, std::cin);
            std::cout << "The plots of settlement:\n";
            print(plots);

            double total_area = 0.0;
            double total_building_area = 0.0;
            for (const auto& plot : plots) {
                total_area += plot.area;
                for (const auto& building : plot.buildings) {
                    total_building_area += building.area;
                }
            }
            std::cout << "Total area of settlement: " << total_area << "\n";
            std::cout << "Total area of buildings: " << total_building_area << "\n";
        }
    }

    std::cout << "\nTask 3. The mathematical vector.\n";
    {
        std::string input = "";
        std::string sax = "";
        std::string say = "";
        std::string sbx = "";
        std::string sby = "";
        std::cout << "Operations with 2-dimensional vectors.\n";
        std::cout << "Command \"+\" for vector addition.\n";
        std::cout << "Command \"-\" for vector subtraction.\n";
        std::cout << "Command \"scalar\" for multiplication of a vector by a scalar.\n";
        std::cout << "Command \"length\" for vector length.\n";
        std::cout << "Command \"normalize\" for vector normalization.\n";
        std::cout << "Command \"exit\" for exit.\n";
        std::cout << "Command: ";
        while (std::getline(std::cin, input)) {
            if (input == "exit") {
                break;
            }
            if (input == "+") {
                std::cout << "Sequentially enter the coordinates of the 2 vectors: ";
                if (std::getline(std::cin, input)) {
                    std::stringstream input_stream(input);
                    input_stream >> sax >> say >> sbx >> sby;
                    if (is_number(sax) && is_number(say) &&
                        is_number(sbx) && is_number(sby)) {
                        VEC2D a = { std::stod(sax), std::stod(say) };
                        VEC2D b = { std::stod(sbx), std::stod(sby) };
                        auto result = a + b;
                        std::cout << "The result of a + b: "
                                  << result.x << " " << result.y << "\n";
                    }
                    else {
                        std::cout << "Incorrect number!\n";
                    }
                }
            }
            if (input == "-") {
                std::cout << "Sequentially enter the coordinates of the 2 vectors: ";
                if (std::getline(std::cin, input)) {
                    std::stringstream input_stream(input);
                    input_stream >> sax >> say >> sbx >> sby;
                    if (is_number(sax) && is_number(say) &&
                        is_number(sbx) && is_number(sby)) {
                        VEC2D a = { std::stod(sax), std::stod(say) };
                        VEC2D b = { std::stod(sbx), std::stod(sby) };
                        auto result = a - b;
                        std::cout << "The result of a - b: "
                                  << result.x << " " << result.y << "\n";
                    }
                    else {
                        std::cout << "Incorrect number!\n";
                    }
                }
            }
            if (input == "scalar") {
                std::cout << "Sequentially enter the coordinates of a vector and a scalar: ";
                if (std::getline(std::cin, input)) {
                    std::stringstream input_stream(input);
                    input_stream >> sax >> say >> sbx;
                    if (is_number(sax) && is_number(say) &&
                        is_number(sbx)) {
                        VEC2D a = { std::stod(sax), std::stod(say) };
                        auto result = a * std::stod(sbx);
                        std::cout << "The result of a * scalar: "
                                  << result.x << " " << result.y << "\n";
                    }
                    else {
                        std::cout << "Incorrect number!\n";
                    }
                }
            }
            if (input == "length") {
                std::cout << "Sequentially enter the coordinates of a vector: ";
                if (std::getline(std::cin, input)) {
                    std::stringstream input_stream(input);
                    input_stream >> sax >> say;
                    if (is_number(sax) && is_number(say)) {
                        VEC2D a = { std::stod(sax), std::stod(say) };
                        std::cout << "The length of the vector a: "
                                  << vector_length(a) << "\n";
                    }
                    else {
                        std::cout << "Incorrect number!\n";
                    }
                }
            }
            if (input == "normalize") {
                std::cout << "Sequentially enter the coordinates of a vector: ";
                if (std::getline(std::cin, input)) {
                    std::stringstream input_stream(input);
                    input_stream >> sax >> say;
                    if (is_number(sax) && is_number(say)) {
                        VEC2D a = { std::stod(sax), std::stod(say) };
                        auto result = vector_normalize(a);
                        std::cout << "Normalized vector a: "
                                  << result.x << " " << result.y << "\n";
                    }
                    else {
                        std::cout << "Incorrect number!\n";
                    }
                }
            }
            std::cout << "Command: ";
        }
    }

    std::cout << "\nTask 4. Game.\n";
    {
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
    }

    return 0;
}