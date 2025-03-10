#include <cstdlib>
#include <ctime>
#include <iostream>
#include <fstream>
#include <sstream>
#include <array>
#include <string>
#include <vector>
#include <map>
#include <unordered_map>

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

struct date {
    int year = 0;
    int month = 0;
    int day = 0;
};

bool is_date(const date & d) {
    if (d.month < 1 || d.month > 12 ||
         d.day < 1) {
        return false;
    }
    switch (d.month) {
    case 1:
    case 3:
    case 5:
    case 7:
    case 8:
    case 10:
    case 12:
        return d.day < 32;
    case 2:
        return d.day < 29 +
        leap_year(d.year);
    default:
        return d.day < 31;
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

bool is_ddmmyyyy(const std::string& ddmmyyyy, const char delimeter,
                 date & d) {
    auto size = ddmmyyyy.size();
    if (size < 6) {
        return false;
    }
    auto nums = split(ddmmyyyy, delimeter);
    if (nums.size() != 3) {
        return false;
    }

    int year = 0, month = 0, day = 0;
    if (!is_number(nums[0])) {
       return false;
    }
    d.day = std::stoi(nums[0]);
    if (!is_number(nums[1])) {
        return false;
    }
    d.month = std::stoi(nums[1]);
    if (!is_number(nums[2])) {
        return false;
    }
    d.year = std::stoi(nums[2]);

    return is_date(d);
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

// Task 4. ATM machine.

template<std::size_t N>
std::map<int, int>
initialize_banknotes(const std::array<int, N> & nominals) {
    std::map<int, int> banknotes;
    banknotes[0] = 0;
    for (int i = 0; i < N; ++i) {
        banknotes[nominals[i]] = 0;
    }
    return banknotes;
}

long readATM(const std::string & filename, 
             std::map<int, int> & banknotes) {
    long total_amount = 0;
    int total_notes = 0;
    auto sizeofint = sizeof(int);

    std::ifstream file(filename, std::ios::binary);
    if (file.is_open()) {
        auto it = banknotes.begin();
        if (it != banknotes.end()) {
            ++it;
            for (; it != banknotes.end(); ++it) {
                file.read((char*) & it -> second, sizeofint);
                total_notes += it -> second;
                total_amount += it -> first * it -> second;
            }
        }
        file.close();
        banknotes[0] = total_notes;
    }
    else {
        for (auto & [_, notes] : banknotes) {
            notes = 0;
        }
    }

    return total_amount;
}

long showATM(const std::map<int, int> & banknotes, 
             std::ostream & stream = std::cout) {
    long total_amount = 0;
    std::string snominal = "";
    std::cout << "Current ATM status:\n";
    auto it = banknotes.begin();
    if (it != banknotes.end()) {
        ++it;
        for (; it != banknotes.end(); ++it) {
            total_amount += it -> first * it -> second;

            if (it -> first < 1000) {
                snominal = "   " + std::to_string(it -> first) +
                           " RUR : ";
            }
            else if (it->first < 10000) {
                snominal = " " +  std::to_string(it -> first / 1000) +
                           "'000 RUR : ";
            }
            else {
                snominal = std::to_string(it->first / 1000) +
                           "'000 RUR : ";
            }
            stream << snominal;

            if (it -> second < 10) {
                stream << "     ";
            }
            else if (it -> second < 100) {
                stream << "    ";
            }
            else if (it -> second < 1000) {
                stream << "   ";
            }
            else if (it -> second < 10000) {
                stream << "  ";
            }
            stream << it -> second << "\n";
        }
    }

    stream << "Total      : ";
    if (banknotes.count(0) > 0) {
        if (banknotes.at(0) < 10) {
            stream << "     ";
        }
        else if (banknotes.at(0) < 100) {
            stream << "    ";
        }
        else if (banknotes.at(0) < 1000) {
            stream << "   ";
        }
        else if (banknotes.at(0) < 10000) {
            stream << "  ";
        }
        stream << banknotes.at(0) << "\n";
    }
    else {
        stream << " N/A\n";
    }

    stream << "Total amount : " << total_amount << " RUR\n";

    return total_amount;
}

void writeATM(const std::string & filename,
              std::map<int, int> banknotes) {
    auto sizeofint = sizeof(int);
    std::ofstream file(filename, std::ios::binary);
    auto it = banknotes.begin();
    if (it != banknotes.end()) {
        ++it;
        for (; it != banknotes.end(); ++it) {
            file.write((char *) & it -> second, sizeofint);
        }
    }
    file.close();
}

void depositATM(std::map<int, int> & banknotes, const int fill_note = 0,
                const int max_notes = 1000) {
    auto total_notes = banknotes[0];
    if (total_notes == max_notes) {
        return;
    }
    auto size = banknotes.size();
    if (size < 2) {
        return;
    }
 
    std::srand(std::time(nullptr));
    // std::rand() % (max - min + 1) + min;
    auto min_notes = max_notes / (size - 1) / 2;
    auto divisor = max_notes / (size - 1) - min_notes + 1;

//    auto it = banknotes.rbegin();
    auto it = banknotes.end();
    --it;
    if (fill_note > 0) {
        auto fill_it = banknotes.find(fill_note);
        if (fill_it != banknotes.end()) {
            it = fill_it;
        }
    }

    while (total_notes < max_notes) {
 
        auto notes = std::rand() % divisor + min_notes;
        auto delta = max_notes - total_notes;
        if (notes > delta) {
            notes = delta;
        }

        it->second += notes;
        total_notes += notes;
        --it;

        if (it == banknotes.begin()) {
            it = banknotes.end();
            --it;
        }

        /*
        if (it -> first == 0 || it == banknotes.rend()) {
            it = banknotes.rbegin();
        }
        */
    }
    banknotes[0] = total_notes;
}

bool withdrawATM(std::map<int, int>& banknotes, long amount,
                 int & request_nominal) {
    auto it = banknotes.rbegin();
    if (it == banknotes.rend()) {
        return false;
    }
    request_nominal = 0;

    int total = 0;
    std::vector<std::pair<int, int>> withdraw{};
    for (; it != banknotes.rend(); ++it) {
        auto nominal = it -> first;
        if (nominal == 0) {
            break;
        }

        if (amount >= nominal) {
            auto notes = amount / nominal;
            if (it -> second < notes) {
                notes = it -> second;
                if (request_nominal == 0) {
                    request_nominal = nominal;
                    std::cout << "request_nominal=" << request_nominal << "\n";
                }
            }
            total += notes;
            amount -= notes * nominal;
            withdraw.push_back({ nominal, notes });
        }
    }

    if (amount == 0) {
        for (const auto& notes : withdraw) {
            banknotes[notes.first] -= notes.second;
        }
        banknotes[0] -= total;
        return true;
    }

    return false;
}

int main() {

    std::cout << std::boolalpha;
/*
    std::ofstream file;
    file.open("test.txt");
    file.setf(std::ios::boolalpha);
    file.setf(std::ios::showpos);
    file.setf(std::ios::showpoint);
    file.setf(std::ios::fixed);
    file.precision(3);
    file.width(10);

    int number = 8;
    file.write((char*)&number, sizeof(number));
    file.write("\n", 1);

    file.close();
 */

    std::cout << "Task 1. Entry in the statement of accounts.\n";
    {
        std::cout << "Consecutively enter the rows with the entries : Name and\n";
        std::cout << "Surname of the payee, date and amount of payment. The\n";
        std::cout << "data must be separated by spaces (type \"exit\" for exit):\n";

        std::string filename = "payroll_test.txt";
        std::ofstream file;
        file.open(filename, std::ios::app);
        if (file.is_open()) {
            date d{};
            std::string input;
            std::string gap = " ";
            std::string fname = "";
            std::string lname = "";
            std::string dt = "";
            std::string amount = "";
            while (std::getline(std::cin, input)) {
                if (input == "exit") {
                    break;
                }
                std::stringstream input_stream(input);
                input_stream >> fname >> lname >> dt >> amount;
                if (is_ddmmyyyy(dt, '.', d)) {
                    if (is_number(amount)) {
                        file << fname << gap << lname << gap
                            << dt << gap << amount << "\n";
                    }
                    else {
                        std::cout << "Incorrect amount!\n";
                    }
                }
                else {
                    std::cout << "Incorrect date!\n";
                }
            }
        }
        file.close();
        text_view(filename);
    }

    std::cout << "\nTask 2. Drawing random pictures.\n";
    {
        std::cout << "Enter the dimensions of the picture: width and height: ";
        std::string input;
        std::string strwidth = "";
        std::string strheight = "";
        char byte = 0;
        char lf = '\n';
        int width = 0, height = 0;
        while (std::getline(std::cin, input)) {
            if (input == "exit") {
                break;
            }
            std::stringstream input_stream(input);
            input_stream >> strwidth >> strheight;
            if (is_number(strwidth) && is_number(strheight)) {
                width = std::stoi(strwidth);
                height = std::stoi(strheight);
                std::ofstream file;
                file.open("binary_pic.txt", std::ios::binary);
                if (file.is_open()) {
                    std::srand(std::time(nullptr));
                    for (int i = 0; i < height; ++i) {
                        for (int j = 0; j < width; ++j) {
                            byte = std::rand() % 2;
                            std::cout << (int)byte;
                            file.write(&byte, 1);
                        }
                        std::cout << lf;
                        file.write(&lf, 1);
                    }
                }

                file.close();
                break;
            }
            else {
                std::cout << "Non-numeric data entered!\n";
            }
        }
    }

    std::cout << "\nTask 3. Fishing.\n";
    {
        std::ofstream file;
        file.open("coolspot.txt");
        file << "sunfish\n";
        file << "shad\n";
        file << "carp\n";
        file << "bass\n";
        file << "bullhead\n";
        file << "carp\n";
        file << "walleye\n";
        file << "catfish\n";
        file << "carp\n";
        file.close();
    }
    {
        std::string coolspot = "coolspot.txt";
        std::string fishcorral = "fishcorral.txt";
        std::string fish = "";
        std::string fishfromspot = "";
        std::unordered_map<std::string, int> fishes_in_corral;

        std::ifstream file_coolspot(coolspot);
        if (file_coolspot.is_open()) {
            std::ofstream file_fishcorral(fishcorral, std::ios::app);
            while (true) {
                std::cout << "Enter the species of fish you intend to catch (\"exit\" for exit): ";
                std::cin >> fish;
                std::cin.clear();
                if (fish == "exit") {
                    break;
                }
                while (!file_coolspot.eof()) {
                    file_coolspot >> fishfromspot;
                    if (fish == fishfromspot) {
                        file_fishcorral << fish << "\n";
                        ++fishes_in_corral[fish];
                        std::cout << "One fish \"" << fish << "\" caught.\n";
                        break;
                    }
                }
                if (file_coolspot.eof()) {
                    std::cout << "The flow has reached the end.\n";
                    std::cout << "Press 'y' if you want to fish again:";
                    std::cin >> fish;
                    if (fish == "y") {
                        file_coolspot.close();
                        file_coolspot.open(coolspot);
                    }
                    else {
                        break;
                    }
                }
            }
            file_fishcorral.close();
        }
        else {
            std::cout << "File \"" << coolspot << "\" not found! Look for another fishing spot.\n";
        }
        file_coolspot.close();

        std::cout << "Fish caught in the session:\n";
        for (const auto& [fish, q] : fishes_in_corral) {
            std::cout << fish << " - " << q << "\n";
        }
    }

    std::cout << "\nTask 4. ATM machine.\n";
    // 100 200 500 1000 2000 5000
    // 0   1   2   3    4    5
    {
        std::string filename = "ATM.bin";
        const int note_types = 6;
        const std::array<int, note_types> nominals = {
            100, 200, 500, 1000, 2000, 5000
        };

        auto banknotes = initialize_banknotes(nominals);

        auto total_amount = readATM(filename, banknotes);
        auto total_notes = banknotes[0];
        
        showATM(banknotes);

        int request_nominal = 0;
        std::string input = "";
        while (true) {
            std::cout << "Enter \"+\" for depositing notes,"
                      << " \"-\" for cash withdrawal,"
                      << " \"x\" for quit: ";
            std::cin >> input;
            if (input == "x") {
                break;
            }
            if (input == "+") {
                depositATM(banknotes, request_nominal);
                request_nominal = 0;
                total_notes = banknotes[0];
                total_amount = showATM(banknotes);
                continue;
            }
            if (input == "-") {
                std::cout << "Enter the amount to withdraw multiples of 100: ";
                std::cin >> input;
                if (is_number(input)) {
                    long amount = std::stod(input);
                    if (!withdrawATM(banknotes, amount, request_nominal)) {
                        std::cout << "The requested amount cannot be issued.\n";
                        std::cout << "Please enter another amount.\n";
                    }
                    else {
                        showATM(banknotes);
                    }
                }
                else {
                    std::cout << "Incorrect number!\n";
                }
            }
        }
        writeATM(filename, banknotes);
        showATM(banknotes);
    }

    return 0;
}