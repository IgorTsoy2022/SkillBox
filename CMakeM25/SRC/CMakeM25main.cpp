#include <sstream>
#include "CMakeM25.h"


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

int main() {
	std::cout << "Task 1. Surgery.\n";
    std::cout << "Commands for surgery:\n";
    std::cout << "\"scalpel\"  - Make an incision.\n";
    std::cout << "\"hemostat\" - Apply a hemostatic clamp to the point.\n";
    std::cout << "\"tweezers\" - Clamp with tweezers at the point.\n";
    std::cout << "\"suture\"   - Stitch between the points.\n";
    std::cout << "\"exit\"     - Quit.\n";
    std::string input = "";
    std::cout << "Command > ";
    while (std::getline(std::cin, input)) {
        if (input == "exit") {
            break;
        }

        std::string str1 = "";
        std::string str2 = "";
        COORD incision_begin, incision_end;
        if (input == "scalpel") {
            bool correct = false;
            input = "";
            std::cout << "    Enter the coordinates for the start and end of the incision:\n";
            std::cout << "    Start coordinates (x y): ";
            while (std::getline(std::cin, input)) {
                if (input == "exit") {
                    break;
                }
                std::stringstream input_stream(input);
                input_stream >> str1 >> str2;
                correct = is_number(str1) && is_number(str2);
                if (correct) {
                    incision_begin.x = std::stod(str1);
                    incision_begin.y = std::stod(str2);
                    break;
                }
                else {
                    std::cout << "    Incorrect number!\n";
                }
                std::cout << "    Start coordinates (x y): ";
            }
            if (correct) {
                std::cout << "    End coordinates (x y): ";
                while (std::getline(std::cin, input)) {
                    if (input == "exit") {
                        break;
                    }
                    std::stringstream input_stream(input);
                    input_stream >> str1 >> str2;
                    correct = is_number(str1) && is_number(str2);
                    if (correct) {
                        incision_end.x = std::stod(str1);
                        incision_end.y = std::stod(str2);
                        break;
                    }
                    else {
                        std::cout << "    Incorrect number!\n";
                    }
                    std::cout << "    End coordinates (x y): ";
                }
            }
            if (correct) {
                incision(incision_begin, incision_end);
            }
        }
        std::cout << "Command > ";
    }

	return 0;
}
