#include "m25computer.h"

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
            while (true) {
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