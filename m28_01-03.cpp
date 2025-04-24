#include <iostream>
#include <sstream>
#include <cmath>
#include <ctime>
#include <thread>
#include <mutex>
#include <map>
#include <string>
#include <string_view>
#include <vector>

std::vector<std::string> history;
std::mutex history_access;

void wait_for_call(int time, std::string name) {
    std::this_thread::sleep_for(std::chrono::seconds(time));
    std::cout << "Id: " << std::this_thread::get_id() << std::endl;
    std::cout << "Event " << name << " after " << time << " sec.\n";
    history_access.lock();
    history.push_back(name);
    history_access.unlock();
}




// Task 1. The 100-meter swim


// Task 2. Simulation of train station operation


// Task 3. Simulation of online restaurant kitchen operation

int main() {

/*
    std::cout << "Id: " << std::this_thread::get_id() << std::endl;

    std::thread call(wait_for_call, 3, "abra");
    std::thread call2(wait_for_call, 3, "abra2");
    call.detach();
    call2.detach();

//    std::this_thread::sleep_for(std::chrono::seconds(5));
//    std::cout << "Another event after " << 5 << " sec.\n";
    std::this_thread::sleep_for(std::chrono::seconds(10));
    if (call.joinable()) call.join();
    if (call2.joinable()) call2.join();

    std::cout << "Missed:\n";
    history_access.lock();
    for (const auto& name : history) {
        std::cout << name << "\n";
    }
    history_access.unlock();
*/

    {
        std::cout << "Task 1. The 100-meter swim.\n";

        std::map<std::string, double> swimmers;
        std::map<int, std::string_view> results;

        int swimmers_count = 6;
        swimmers["Michael Phelps"] = 0;
        swimmers["Ian Thorpe"] = 0;
        swimmers["Ryan Lochte"] = 0;
        swimmers["Mark Spitz"] = 0;
        swimmers["Johnny Weissmuller"] = 0;
        swimmers["Matt Biondi"] = 0;

        int max_speed = 2136;
        int min_speed = 1860;
        std::srand(std::time(nullptr));
        for (const auto& [name, _] : swimmers) {
            int i = std::rand() % (max_speed - min_speed + 1) + min_speed;
            swimmers[name] = double(i) / 1000;
        }

        std::cout << "The number of concurrent threads is "
                  << std::thread::hardware_concurrency() << "\n";

    }

// Task 2. Simulation of train station operation


// Task 3. Simulation of online restaurant kitchen operation


    return 0;
}