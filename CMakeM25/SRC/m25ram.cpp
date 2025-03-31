#include "m25ram.h"

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
