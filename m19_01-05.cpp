#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <tuple>
#include <unordered_map>

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

int search_in_file(const std::string& filename, std::string& search_word) {
    std::fstream fs;
    fs.open(filename);

    int size = search_word.size();
    int count = 0;
    toupper(search_word);
    std::string word = "";
    if (fs.is_open()) {
        while (fs >> word) {
            if (word.size() >= size) {
                toupper(word);
                if (word.substr(0, size) == search_word) {
                    ++count;
                }
            }
        }
    }
    else {
        std::cout << "File " << filename << " not found.\n";
    }

    fs.close();

    return count;
}


int main() {
/*
    std::cout << "Task 1. Search word.\n";
    {
        std::string filename = "C:\\CPP\\GIT\\SkillBox-main\\Hello darkness.txt";
        std::string word = "silence";

        std::fstream fs;
        fs.open(filename);
        if (fs.is_open()) {
            std::cout << "File content:\n";
            std::string buffer = "";
            while (std::getline(fs, buffer)) {
                std::cout << buffer << "\n";
            }
        }
        fs.close();

        std::cout << "\nFile: \"" << filename << "\".\n";
        std::cout << "Enter word for search (\"exit\" for exit): ";
        while (std::getline(std::cin, word)) {
            auto word_count = search_in_file(filename, word);

            std::cout << "The word \"" << word << "\" was ";
            if (word_count > 0) {
                std::cout << "found " << word_count << " times.\n";
            }
            else {
                std::cout << "not found.\n";
            }

            if (word == "EXIT") {
                break;
            }

            std::cout << "\nEnter word for search (\"exit\" for exit): ";
        }
    }

    std::cout << "\nTask 2. Text viewer.\n";
    {
        std::string filename = "C:\\CPP\\GIT\\SkillBox-main\\Hello darkness.txt";
//        std::getline(std::cin, filename);

        std::ifstream fs;
        fs.open(filename, std::ios::binary);
        if (fs.is_open()) {
            std::cout << "File content:\n";
            char buffer [33];

            while (!fs.eof()) {
                fs.read(buffer, 32);
                buffer[fs.gcount()] = 0;
                std::cout << buffer;
            }
        }
        fs.close();

    }

    std::cout << "\nTask 3. Payroll.\n";
    {
        std::string filename = "C:\\CPP\\GIT\\SkillBox-main\\Payroll.txt";
        std::ifstream fs;
        fs.open(filename, std::ios::binary);
        if (fs.is_open()) {
            std::string buffer = "";
            std::string first_name = "";
            std::string surname = "";
            std::string highest_bidder = "";
            double wage = 0, maxwage = 0, total = 0;
            std::string date = "";
            while (std::getline(fs, buffer)) {
                std::stringstream buffer_stream(buffer);
                buffer_stream >> first_name >> surname >> wage >> date;
                total += wage;
                if (wage > maxwage) {
                    maxwage = wage;
                    highest_bidder = first_name + " " + surname;
                }
                std::cout << first_name << " " << surname << " " << wage << " " << date << "\n";
            }
            std::cout << "The total amount of payments on the payroll was " << total << " USD.\n";
            std::cout << "The maximum amount of " << maxwage
                      << " USD was paid to " << highest_bidder << ".\n";
        }
        fs.close();

    }
//*/

    std::cout << "\nTask 4. PNG file detector.\n";

    return 0;
}