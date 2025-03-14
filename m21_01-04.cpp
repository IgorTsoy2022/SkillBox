#include <ctime>
#include <cmath>
#include <iostream>
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

struct DATE {
    int year = 0;
    int month = 0;
    int day = 0;
};

struct PAYROLL {
    std::string name = "";
    DATE date;
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

bool is_date(const DATE& date) {
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
                      const char delimeter, DATE& date) {
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

std::string date_to_ddmmyyyy(const DATE& date,
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

            DATE date{};
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

struct POINT {
    double x = 0.0;
    double y = 0.0;
};

POINT operator-(const POINT& a) {
    POINT result;
    result.x = -a.x;
    result.y = -a.y;
    return result;
}

POINT operator+(const POINT& a, const POINT& b) {
    POINT result;
    result.x = a.x + b.x;
    result.y = a.y + b.y;
    return result;
}

POINT operator-(const POINT& a, const POINT& b) {
    POINT result;
    result.x = a.x - b.x;
    result.y = a.y - b.y;
    return result;
}

POINT operator*(const POINT& a, const double c) {
    POINT result;
    result.x = a.x * c;
    result.y = a.y * c;
    return result;
}

POINT operator*(const double c, const POINT& a) {
    POINT result;
    result.x = a.x * c;
    result.y = a.y * c;
    return result;
}

double vector_length(const POINT& a) {
    return std::sqrt(a.x * a.x + a.y * a.y);
}

POINT vector_normalize(const POINT& a) {
    POINT result;
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
};

std::vector<CHARACTER> initializing_enemies(const int number) {
    std::vector<CHARACTER> enemies(number);
    std::srand(std::time(nullptr));
    // std::rand() % (max - min + 1) + min;
    int id = 1;
    for (auto & enemy : enemies) {
        enemy.name = "Enemy #" + std::to_string(id++);
        enemy.health = std::rand() % 101 + 50;
        enemy.armor = std::rand() % 51;
        enemy.damage = std::rand() % 16 + 15;
    }
    
    return enemies;
}

void print(const std::vector<CHARACTER> & enemies) {
    for (const auto & enemy : enemies) {
        std::cout << enemy.name << " health: " << enemy.health << " armor: " << enemy.armor << " damage: " << enemy.damage << "\n";
    }
}

void goto_xy(SHORT x, SHORT y) {
    HANDLE hStdOut = ::GetStdHandle(STD_OUTPUT_HANDLE);
    COORD position = { x, y };
    ::SetConsoleCursorPosition(hStdOut, position);
}

void draw_frame(const int width, const int height, SHORT row) {
    goto_xy(0, row);
    std::cout << "+";
    for (SHORT i = 0; i < width; ++i) {
        std::cout << "---";
    }
    std::cout << "+\n";
    for (SHORT j = 0; j < height; ++j) {
        std::cout << "|";
        for (SHORT i = 0; i < width; ++i) {
            std::cout << " . ";
        }
        std::cout << "|\n";
    }
    std::cout << "+";
    for (SHORT i = 0; i < width; ++i) {
        std::cout << "---";
    }
    std::cout << "+\n";
}

int main() {

    std::cout << std::boolalpha;
/*
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
                        POINT a = { std::stod(sax), std::stod(say) };
                        POINT b = { std::stod(sbx), std::stod(sby) };
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
                        POINT a = { std::stod(sax), std::stod(say) };
                        POINT b = { std::stod(sbx), std::stod(sby) };
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
                        POINT a = { std::stod(sax), std::stod(say) };
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
                        POINT a = { std::stod(sax), std::stod(say) };
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
                        POINT a = { std::stod(sax), std::stod(say) };
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
//*/

    std::cout << "\nTask 4. Game.\n";
    {
    	const int width = 15;
    	const int height = 15;
    	const int number_of_enemies = 5;
    	auto enemies = initializing_enemies(number_of_enemies);
    	
    	std::vector<std::vector<int>> field(height, std::vector<int>(width, 0));
    	draw_frame(width, height, 4);
    	
    	print(enemies);
    }

    return 0;
}