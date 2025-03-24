#include <iostream>
#include <fstream>
#include <sstream>
#include <ctime>
#include <iomanip>

#include <string>
#include <vector>
#include <map>
#include <unordered_map>
#include <set>

#include <windows.h>
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

std::vector<int>
split_into_numbers(const std::string& str, const char delimiter = ' ') {
    std::vector<int> result;
    std::string word = "";
    int num = 0;
    for (const auto& c : str) {
        if (c == delimiter) {
            if (word.size() > 0) {
                if (!is_number(word)) {
                    return {};
                }
                num = std::stod(word);
                result.push_back(num);
                word.clear();
            }
        }
        else {
            if (c >= '0' && c <= '9') {
                word += c;
            }
            else {
                return {};
            }
        }
    }
    if (word.size() > 0) {
        if (!is_number(word)) {
            return {};
        }
        num = std::stod(word);
        result.push_back(num);
    }
    return result;
}

int leap_year(const int year) {
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

bool is_valid_date(const int year, const int month, const int day) {
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

std::tm make_tm(const int year, const int month, const int day) {
    std::tm tm{};
    tm.tm_year = year - 1900;
    tm.tm_mon = month - 1;
    tm.tm_mday = day;
    return tm;
}

std::time_t find_nearest(const std::time_t date, const std::map<std::time_t, std::unordered_map<std::string, std::time_t>> & birthdays) {
    if (birthdays.empty()) {
        return -1;
    }
    std::tm local = *std::localtime(&date);
    int year = local.tm_year;
    auto tm = make_tm(0, local.tm_mon - 1, local.tm_mday);
    std::time_t current_day = std::mktime(&tm);
    for (const auto& [date, _] : birthdays) {
        if (date >= current_day) {
            return date;
        }
    }
    return birthdays.begin()->first;
}

template<typename C>
void print(const C & list, const int indent) {
    std::string blank(indent, ' ');
    for (const auto& [name, date] : list) {
        std::tm local = *localtime(&date);
        std::cout << blank << name << " " << std::put_time(&local, "%B %d, %Y") << "\n";
    }
}

// Task 3. Timer.

void goto_xy(SHORT x, SHORT y) {
    HANDLE hStdOut = ::GetStdHandle(STD_OUTPUT_HANDLE);
    COORD position = { x, y };
    ::SetConsoleCursorPosition(hStdOut, position);
}

void show_time(short col, short row, int min, int sec) {
    goto_xy(col, row);
    
}

int main() {
        
   //     std::cout << std::setfill('0') << std::setw(5) << 42;
        
       std::time_t t = std::time(nullptr);
        std::tm local = *std::localtime(&t);
        std::string inputstr = "";
        std::cin >> inputstr;
        std::istringstream iss(inputstr);
        iss >> std::get_time(&local, "%H:%M");
        std::cout << std::asctime(&local) << std::endl;
        
        
    return 0;    
        std::cout << "Commands for timer.\n";
	    std::cout << "\"new\"   - Set new time.\n";
	    std::cout << "\"run\"   - Start timer.\n";
	    std::cout << "\"stop\"  - Stop timer.\n";
        std::cout << "\"exit\"  - Quit.\n";
        std::string input = "";
        do {
            if (input == "exit") {
                break;
            }

            if (input == "new") {
                input.clear();
                do {
                    if (input == "exit") {
                        break;
                    }

                    if (input.size() > 0) {
                        auto nums = split_into_numbers(input, ':');

                        if (nums.size() > 1) {
                            if (nums[1] >= 0 && nums[1] < 60) {
                                std::istringstream iss(input);
                                
                                break;
                            }
                            else {
                                std::cout << "Seconds must be in the range 0 - 59.\n";
                            }
                        }
                        else {
                            std::cout << "Incorrect time!\n";
                        }
                    }
                    std::cout << "    Enter the time: mm:ss > ";
                } while (std::getline(std::cin, input));
            }

            std::cout << "Command > ";
        } while (std::getline(std::cin, input));

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

/*
    std::time_t t = std::time(nullptr);
    std::tm local = *std::localtime(&t);
    std::cin >> std::get_time(&local, "%H:%M");
    std::cout << std::asctime(&local) << std::endl;
    return 0;
*/

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
        std::cout << "\"exit\"   - Quit.\n";
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
 
    std::cout << "\nTask 2. Birthday reminder.\n";
    {
        std::map<std::time_t, std::unordered_map<std::string, std::time_t>> birthdays;
        std::cout << "List of date birth.\n";
        std::cout << "Commands for birtgdays tracking:\n";
        std::cout << "\"add\"   - Add a new person.\n";
        std::cout << "\"list\"  - Display all records.\n";
        std::cout << "\"date\"  - Display nearest birthday.\n";
        std::cout << "\"exit\"  - Quit.\n";
        std::cout << "Command > ";

        std::string input = "";
        while (std::getline(std::cin, input)) {
            if (input == "exit") {
                break;
            }

            if (input == "add") {
                std::cout << "    Name > ";
                while (std::getline(std::cin, input)) {
                    if (input == "exit") {
                        break;
                    }

                    if (input.size() > 0) {
                        std::cout << "    Date of birth YYYY.MM.DD > ";
                        std::string date = "";
                        while (std::getline(std::cin, date)) {
                            if (date == "exit") {
                                break;
                            }

                            auto nums = split_into_numbers(date, '.');
                            if (nums.size() > 2) {
                    	        auto is_valid = is_valid_date(nums[0], nums[1], nums[2]);
                                if (is_valid) {
                                    auto tm = make_tm(0, nums[1], nums[2]);
                                    auto key = std::mktime(&tm);
                                    tm.tm_year += nums[0];
                                  birthdays[key][input] = std::mktime(&tm);
                                    break;
                                }
                                else {
                                    std::cout << "    It's not a valid date!\n";
                                }
                            }
                            else {
                                std::cout << "    Incorrect format of date!\n";
                            }
                            std::cout << "    Date of birth YYYY.MM.DD > ";
                        }
                    }
                    std::cout << "    Name > ";
                }
            }

            if (input == "list") {
                for (const auto& [key, persone] : birthdays) {
                    std::tm local = *localtime(&key);
                    std::cout << std::put_time(&local, "%B %d") << "\n";
                    print(persone, 4);
                }
            }

            if (input == "date") {
                std::cout << "Today's date YYYY.MM.DD > ";
                while (std::getline(std::cin, input)) {
                    if (input == "exit") {
                        break;
                    }

                    auto nums = split_into_numbers(input, '.');
                    if (nums.size() > 2) {
                        auto is_valid = is_valid_date(nums[0], nums[1], nums[2]);
                        if (is_valid) {
                            auto tm = make_tm(0, nums[1], nums[2]);
                            auto time = std::mktime(&tm);

                            auto ftime = find_nearest(time, birthdays);

                            if (birthdays.count(ftime) > 0) {
                                if (ftime == time) {
                                    if (birthdays.at(ftime).size() > 1) {
                                         std::cout << "Today is the birthday of the following people:\n";
                                         print(birthdays.at(ftime), 4);
                                    } 
                                    else {
                                        auto it = birthdays.at(ftime).begin();
                                        std::tm loc = *std::localtime(&it->second);
                                        std::cout << "Today is " << it->first << "'s birthday!\n";
                                        std::cout  << "He was born on " << std::put_time(&loc, "%B %d, %Y") << "\n";
                                    }
                                }
                                else {
                                    print(birthdays.at(ftime), 4);
                                }

                            }
                        }
                        else {
                            std::cout << "    It's not a valid date!\n";
                        }
                    }
                    else {
                        std::cout << "    Incorrect format of date!\n";
                    }
                    std::cout << "Today's date YYYY.MM.DD > ";
                }
            }
        
            std::cout << "Command > ";
        }

    }
*/

    std::cout << "\nTask 3. Timer.\n";
    {
    }

    return 0;
}