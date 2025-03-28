//#include <iostream>
//#include <sstream>
#include "m25surgery.h"

#include "m25cpu.h"
#include "m25ram.h"
#include "m25disk.h"
#include "m25gpu.h"
#include "m25kbd.h"

/*
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
*/

int main() {
	std::cout << "Task 1. Surgery.\n";
    surgery();

    std::cout << "\nTask 2. Computer operation.\n";

	return 0;
}
