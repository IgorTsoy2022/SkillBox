#include <iostream>
#include <fstream>
#include <sstream>
#include <ctime>
#include <iomanip>

#include <string>
#include <vector>
#include <map>
#include <set>

#pragma warning(disable : 4996)

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

// Task 1. Time tracking.


// Task 2. SEASONS.


// Task 3. Analysis of car occupancy in a train.


int main() {
/*
    {
        std::time_t t = std::time(nullptr);
        std::tm* local = std::localtime(&t);

        std::cout << local->tm_hour << std::endl;
        std::cout << std::asctime(local) << std::endl;
        std::cout << std::put_time(local, "%Y/%B/%d %H:%M:%S") << std::endl;
    }

    {
        std::time_t t = std::time(nullptr);
        std::tm local = *std::localtime(&t);
        std::cin >> std::get_time(&local, "%H:%M");
        std::cout << std::asctime(&local) << std::endl;
    }

    {
// #pragma warning(suppress : 4996)
        std::time_t t1 = std::time(nullptr);
        int hold = 0;
        std::cin >> hold;
        std::time_t t2 = std::time(nullptr);

        double d = std::difftime(t1, t2);
        std::time_t f = t2 + (std::time_t)d;

        std::tm* local_future = std::localtime(&f);
        std::cout << std::asctime(local_future) << std::endl;

        local_future->tm_sec += 10;

        std::cout << std::asctime(local_future) << std::endl;
    }
*/   

    std::cout << "Task 1. Time tracking.\n";
    {
    }

    std::cout << "\nTask 2. Seasons.\n";
    {
    }

    std::cout << "\nTask 3. Analysis of car occupancy in a train.\n";
    {
    }

    return 0;
}