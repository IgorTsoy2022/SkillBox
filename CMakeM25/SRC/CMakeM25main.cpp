#include <iostream>
#include <sstream>

#include <array>

/*
#include "m25surgery.h"

#include "m25cpu.h"
#include "m25ram.h"
#include "m25disk.h"
#include "m25gpu.h"
#include "m25kbd.h"
*/


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

// kbd

namespace kbd {
    std::string input() {
        std::string out = "";
        std::getline(std::cin, out);
        return out;
    }
} // namespace kbd

// gpu

namespace gpu {
    template <typename T>
    void display(const T& value) {
        std::cout << value;
    }

    void output_line(const std::string& text) {
        std::cout << text << std::endl;
    }

    void prompt(const std::string& text) {
        std::cout << text;
    }
} // namespace gpu


// ram

#define BUFFER_SIZE 8

namespace ram {
    std::array<int, BUFFER_SIZE> buffer{};

    int read(int id) {
        if (id < 0 || id >= BUFFER_SIZE) {
            gpu::output_line("Out of range!");
            return 0;
        }
        return buffer[id];
    }

    void write(int id, int value) {
        if (id < 0 || id >= BUFFER_SIZE) {
            gpu::output_line("Out of range!");
            return;
        }
        buffer[id] = value;
    }

} // namespace ram

// cpu

namespace cpu {
    int compute(const int nums) {
        int result = 0;
        for (int i = 0; i < nums; ++i) {
            result += ram::read(i);
        }
        return result;
    }
} // namespace cpu

// disk

#include <fstream>
namespace disk {
    void load(const std::string& filename, const int nums) {
        std::ifstream file(filename);
        if (file.is_open()) {
            std::string str = "";
            int id = 0;
            while (file >> str) {
                if (is_number(str)) {
                    ram::write(id++, std::stod(str));
                }
                if (id == nums) {
                    break;
                }
            }
            file.close();
            return;
        }
        else {
            gpu::output_line("File not found!\n");
        }
    }

    void save(const std::string& filename, const int nums) {
        std::ofstream file(filename);
        if (file.is_open()) {
            for (int i = 0; i < nums; ++i) {
                file << ram::read(i) << " ";
            }
        }
        file.close();
    }

} // namespace disk

void computer() {
    std::array<int, BUFFER_SIZE> buffer{};
    const std::string filename = "data.txt";
    gpu::output_line("Commands for computer:");
    gpu::output_line("\"display\" - Display numbers from ram.");
    gpu::output_line("\"compute\" - Add numbers from buffer.");
    gpu::output_line("\"write\"   - Write numbers into RAM.");
    gpu::output_line("\"read\"    - Read numbers from RAM.");
    gpu::output_line("\"save\"    - Save to file.");
    gpu::output_line("\"load\"    - Load from file.");
    gpu::output_line("\"exit\"    - Quit.");

    std::string input = "";
    while (true) {
        if (input == "exit") {
            break;
        }

        if (input == "write") {
        	input.clear();
        	int id = 0;
            while(true) {
                if (input == "exit") {
                    break;
                }

                if (input.size() > 0) {
                    std::istringstream iss(input);
                    std::string str = "";
                    while (iss >> str) {
                        if (is_number(str)) {
                            ram::write(id++, std::stod(str));
                            if (id == BUFFER_SIZE) {
                                break;
                            }
                        }
                    }
                }

                if (id == BUFFER_SIZE) {
                    break;
                }

                gpu::prompt("Enter number(s): ");
                input = kbd::input();
            }
        }

        if (input == "read" || input == "display") {
            for (int i = 0; i < BUFFER_SIZE; ++i) {
                gpu::display(ram::read(i));
                gpu::display(' ');
            }
            gpu::display('\n');
        }

        if (input == "compute") {
            gpu::display(cpu::compute(BUFFER_SIZE));
            gpu::display('\n');
        }

        if (input == "save") {
            disk::save(filename, BUFFER_SIZE);
        }

        if (input == "load") {
            disk::load(filename, BUFFER_SIZE);
        }

        gpu::prompt("Command > ");
        input = kbd::input();
    }

}

int main() {
//	std::cout << "Task 1. Surgery.\n";
//    surgery();

    std::cout << "\nTask 2. Computer operation.\n";

    computer();

	return 0;
}
