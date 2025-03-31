#include "m25kbd.h"

namespace kbd {
    std::string input() {
        std::string out = "";
        std::getline(std::cin, out);
        return out;
    }
} // namespace kbd