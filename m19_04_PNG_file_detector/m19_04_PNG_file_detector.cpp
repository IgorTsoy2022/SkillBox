#include <iostream>
#include <fstream>
#include <string>

static void tolower(std::string& text) {
    for (auto& c : text) {
        if (c >= 'A' && c <= 'Z') {
            c = c + 32;
        }
    }
}

// Task 4. PNG file detector.

static std::string get_extension(const std::string & filename) {
    bool has_extension = false;
    int size = filename.size();
    int start = size - 1;
    while (start >= 0) {
        has_extension = (filename[start] == '.');
        if (has_extension) {
            break;
        }
        --start;
    }

    if (has_extension) {
        return filename.substr(start + 1, size - start - 1);
    }

    return "";
}

static void check_png_file(const std::string & filename) {
    auto size = filename.size();
    bool extension_is_png = false;
    if (size > 4) {
        std::string extension = get_extension(filename);
        tolower(extension);
        extension_is_png = (extension == "png");
    }

    std::ifstream fs;
    fs.open(filename, std::ios::binary);
    if (fs.is_open()) {
        if (extension_is_png) {
            std::cout << "The extension of the file is \"png\"\n";
        }
        else {
            std::cout << "The extension of the file is not \"png\"\n";
        }

        char buffer[4];	
        std::string comp = "0PNG";
        comp[0] = -119;

        fs.read(buffer, 4);
        if (comp == buffer) {
            std::cout << "First 4 bytes of the file indicate that it is probably a png-file.\n";
        }
    }
    else {
        std::cout << "File not found!\n";
     }
    fs.close();
}

int main() {

    std::cout << "\nTask 4. PNG file detector.\n";

    std::string filename = "money.png";

/*
        std::ofstream fs;
        fs.open(filename, std::ios::binary);
        char buffer[5];

        if (fs.is_open()) {
            assign(buffer, "0PNG");
            buffer[0] = -119;
            fs.write(buffer, 4);
        }
        fs.close();
*/

    std::cout << "Enter the filename to check if it is a png file: ";
    std::getline(std::cin, filename);
    std::cin.clear();

    check_png_file(filename);

    return 0;
}