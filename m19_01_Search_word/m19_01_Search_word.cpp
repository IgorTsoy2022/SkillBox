#include <iostream>
#include <fstream>
#include <string>

static void toupper(std::string& text) {
    for (auto& c : text) {
        if (c >= 'a' && c <= 'z') {
            c = c - 32;
        }
    }
}

// Task 1. Search word

static int search_in_file(const std::string& filename, std::string& search_word) {
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
    std::cout << "Task 1. Search word.\n";

    std::string filename = "Hello darkness.txt";
    std::string word = "silence";
        
    std::cout << "Enter the filename to read: ";
    std::getline(std::cin, filename);
    std::cin.clear();

    bool filefound = false;
    std::fstream fs;
    fs.open(filename);
    filefound = fs.is_open();
    if (filefound) {
        std::cout << "File content:\n";
        std::string buffer = "";
        while (std::getline(fs, buffer)) {
            std::cout << buffer << "\n";
        }
    }
    fs.close();

    if (filefound) {
        std::cout << "\nFile: \"" << filename << "\".\n";
        std::cout << "Enter a word for search (\"exit\" for exit): ";
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
    else {
        std::cout << "\nFile: \"" << filename << "\" not found!\n";
    }
    
    return 0;
}