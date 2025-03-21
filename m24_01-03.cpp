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

#define TO_STRING(text) #text

std::string to_time(double sec) {
    const double epsilon = 0.001;
    std::string out = "";
    if (sec < 0) {
        out = "-";
        sec = -sec;
    }
    
    int days = 0;
    if (sec > 86400 || std::abs(sec - 86400) < epsilon) {
        days = sec / 86400;
        sec -= days * 86400;
        sec = sec < 0 ? -sec : sec;
        out += std::to_string(days) + " days ";
    }

    int hours = 0;
    if (sec > 3600 || std::abs(sec - 3600) < epsilon) {
        hours = sec / 3600;
        sec -= hours * 3600;
        sec = sec < 0 ? -sec : sec;
    }
    if (hours < 10) {
        out += "0";
    }
    out += std::to_string(hours) + ":";

    int minutes = 0;
    if (sec > 60 || std::abs(sec - 60) < epsilon) {
        minutes = sec / 60;
        sec -= minutes * 60;
        sec = sec < 0 ? -sec : sec;
    }
    if (minutes < 10) {
        out += "0";
    }
    out += std::to_string(minutes) + ":";

    if (sec < 10) {
        out += "0";
    }
    out += std::to_string(sec);
    
    return out;
}

// Task 2. SEASONS.


// Task 3. Analysis of car occupancy in a train.


int main() {

    {
        std::time_t t = std::time(nullptr);
        std::tm* local = std::localtime(&t);

        std::cout << local->tm_hour << std::endl;
        std::cout << std::asctime(local) << std::endl;
        std::cout << std::put_time(local, "%Y/%B/%d %H:%M:%S") << std::endl;
    }
/*
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
    	std::time_t t1 = std::time(nullptr);
    std::cout << t1 << "\n";
    		int hold;
    	std::cin >> hold;
    	std::time_t t2 = std::time(nullptr);
    std::cout << t2 << "\n";
        double d = std::difftime(t2, t1);
        std::cout << "d = " << d << "\n";
        std::time_t begt = (std::time_t)0;
        
        std::time_t dt = (std::time_t)d;
        dt = t2 - t1;
        std::cout << "dt=" << dt << "\n";
        std::tm loc = *std::localtime(&dt);        std::cout << std::put_time(&loc, "%H:%M:%S") << std::endl;
        std::cout << std::asctime(&loc) << "\n";
        std::cout << loc.tm_hour << "h " << loc.tm_min << "m " << loc.tm_sec << "s\n";

        std::cout << to_time(86563) << "\n";

    	std::map<std::string, std::pair<std::time_t, double>> tasks;
    	std::string input = "";
    	std::string current_task = "";

    	std::cout << "Commands for time tracking:\n";
        std::cout << "\"begin task_name\" - start a new task (current task\n"
                         << "will be closed);\n";
        std::cout << "\"end\" - close current task;\n";
        std::cout << "\"exit\" - quit.\n";
        std::cout << "Command > ";
        while (std::getline(std::cin, input)) {
            if (input == "exit") {
                break;
            }
            if (input == "end") {
                if (current_task.size() > 0) {
                    auto p = tasks.at(current_task);
                    
                }
            }
            else {
                std::string str1 = "";
                std::string str2 = "";
                std::stringstream input_stream(input);
                input_stream >> str1 >> str2;
                if (str1 == "begin" && str2.size() > 0) {
                
                }
            }
            std::cout << "Command > "; 
        }
    }

    std::cout << "\nTask 2. Seasons.\n";
    {
    }

    std::cout << "\nTask 3. Analysis of car occupancy in a train.\n";
    {
    }

    return 0;
}