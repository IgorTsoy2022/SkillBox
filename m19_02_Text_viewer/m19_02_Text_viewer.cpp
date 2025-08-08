#include <iostream>
#include <fstream>
#include <sstream>

// Task 2. Text viewer.

static bool text_view(const std::string & filename) {
    bool isOk = false;
    std::ifstream fs;
    fs.open(filename, std::ios::binary);
    if (fs.is_open()) {
        char buffer [33];
        while (!fs.eof()) {
            fs.read(buffer, 32);
            buffer[fs.gcount()] = 0;
            std::cout << buffer;
        }
        std::cout << "\n";
        isOk = true;
    }
    fs.close();
    return isOk;
}

int main() {
    std::cout << "\nTask 2. Text viewer.\n";

    std::string filename = "Hello darkness.txt";
    std::cout << "Enter the filename to read: ";
    std::getline(std::cin, filename);
    std::cin.clear();

    if (!text_view(filename)) {
        std::cout << "File not found!\n";
    }
 
    return 0;
}