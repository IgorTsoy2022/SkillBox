#include <iostream>
#include <fstream>
#include <string>

int main() {
    std::cout << "Task 1. Search word.\n";
    std::fstream fs;
    fs.open("Hello darkness.txt");
    std::string searching_word = "silence";
    int size = searching_word.size();
    int count = 0;
    std::string word = "";
    if (fs.is_open()) {
        while (fs >> word) {
            if (word.size() >= size) {
                if (word.substr(0, size) == searching_word) {
                     ++count;
                }
            }
        }
    }
    
    fs.close();
    
    return 0;
}