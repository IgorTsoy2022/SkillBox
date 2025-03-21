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
    
    std::ostringstream output_stream;
    int days = 0;
    if (sec > 86400 || std::abs(sec - 86400) < epsilon) {
        days = sec / 86400;
        sec -= days * 86400;
        sec = sec < 0 ? -sec : sec;

        output_stream << days;
        out += output_stream.str() + " day";
        output_stream.str("");
        output_stream.clear();
        if (days > 1) {
            out += "s";
        }
        if (sec > 0) {
            out += " ";
        }
    }

    int hours = 0;
    if (sec > 3600 || std::abs(sec - 3600) < epsilon) {
        hours = sec / 3600;
        sec -= hours * 3600;
        sec = sec < 0 ? -sec : sec;
    }
    if (days > 0 || hours > 0) {
        if (hours < 10) {
            out += "0";
        }
        output_stream << hours;
        out += output_stream.str() + " h";
        output_stream.str("");
        output_stream.clear();
        if (sec > 0) {
            out += " ";
        }
    }

    int minutes = 0;
    if (sec > 60 || std::abs(sec - 60) < epsilon) {
        minutes = sec / 60;
        sec -= minutes * 60;
        sec = sec < 0 ? -sec : sec;
    }
    if (days > 0 || hours > 0 || minutes > 0) {
        if (minutes < 10) {
            out += "0";
        }
        output_stream << minutes;
        out += output_stream.str() + " m";
        output_stream.str("");
        output_stream.clear();
        if (sec > 0) {
            out += " ";
        }
    }

    if (sec < 10) {
        out += "0";
    }
    output_stream << sec;
    out += output_stream.str() + " s";
    
    return out;
}

// Task 2. Birthday reminder.

std::vector<std::string>
split(const std::string& str, const char delimiter = ' ') {
    std::vector<std::string> result;
    std::string word = "";
    for (const auto& c : str) {
        if (c == delimiter) {
            if (word.size() > 0) {
                result.push_back(word);
                word.clear();
            }
        }
        else {
            word += c;
        }
    }
    if (word.size() > 0) {
        result.push_back(word);
    }
    return result;
}

int leap_year(int year) {
    if (year % 400 == 0) {
        return 1;
    }
    if (year % 100 == 0) {
        return 0;
    }
    if (year % 4 == 0) {
        return 1;
    }
    return 0;
}

bool is_date(const int year, const int month, const int day) {
    if (month < 1 || month > 12 || day < 1) {
        return false;
    }
    switch (month) {
    case 1:
    case 3:
    case 5:
    case 7:
    case 8:
    case 10:
    case 12:
        return day < 32;
    case 2:
        return day < 29 +
            leap_year(year);
    default:
        return day < 31;
    }
}



// Task 3. Timer.


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

    std::time_t t = std::time(nullptr);
    std::tm local = *std::localtime(&t);
    std::cin >> std::get_time(&local, "%H:%M");
    std::cout << std::asctime(&local) << std::endl;
    return 0;

/*
    std::cout << "Task 1. Time tracking.\n";
    {
        std::time_t current_task_time = 0;
        std::map<std::time_t, std::pair<std::string, double>> tasks;
    	std::string input = "";

    	std::cout << "Commands for time tracking:\n";
        std::cout << "\"begin\"  - Start a new task (current task will be closed).\n";
        std::cout << "\"end\"    - Close current task.\n";
        std::cout << "\"status\" - Output information about all completed tasks and\n";
        std::cout << "           the time spent on them. The currently running task,\n";
        std::cout << "           if any, is also displayed.\n";
        std::cout << "\"exit\"   - quit.\n";
        std::cout << "Command > ";
        while (std::getline(std::cin, input)) {
            if (input == "exit") {
                break;
            }
            if (input == "begin" || input == "end") {
                if (current_task_time > 0) {
                    tasks.at(current_task_time).second = 
                        std::time(nullptr) - current_task_time;
                    current_task_time = 0;
                }
                if (input == "begin") {
                    input = "";
                    std::cout << "Enter a task name: ";
                    while (std::getline(std::cin, input)) {
                        if (input == "exit") {
                            break;
                        }
                        if (input.size() > 0) {
                            current_task_time = std::time(nullptr);
                            tasks[current_task_time] = { input, 0 };
                            break;
                        }
                        std::cout << "Enter a task name: ";
                    }
                }
            }
            else if (input == "status") {
                for (const auto& [key, task] : tasks) {
                    std::cout << std::put_time(std::localtime(&key), "%Y.%m.%d %H:%M:%S");
                    std::cout << " " << task.first << ", duration: ";
                    std::cout << to_time(task.second) << "\n";
                }
            }
            std::cout << "Command > "; 
        }
    }
*/

    std::cout << "\nTask 2. Birthday reminder.\n";
    {
        std::string input = "";

        std::cout << "List of date birth.\n";
        std::cout << "Enter name and date of birth (\"exit\" for quit).\n";
        std::cout << "Name > ";
        while (std::getline(std::cin, input)) {
            if (input == "exit") {
                break;
            }
            input = "";
            std::cout << "Date of birth YYYY.MM.DD > ";
            while (std::getline(std::cin, input)) {
                if (input == "exit") {
                    break;
                }
                if (input.size() > 0) {
                    auto words = split(input, '.');

                    break;
                }
                std::cout << "Date of birth YYYY.MM.DD > ";
            }

            std::cout << "Name > ";
        }
    }

    std::cout << "\nTask 3. Timer.\n";
    {
    }

    return 0;
}