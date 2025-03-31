#include "m25disk.h"

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