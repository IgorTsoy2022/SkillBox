#include <iostream>
#include <sstream>

#include <chrono>
#include <ctime>
#include <iomanip>
#include <thread>

#include <string>
#include <vector>
#include <map>
#include <unordered_map>
#include <set>

#include <windows.h>

//#pragma warning(disable : 4996)

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

std::string int_to_date(int decdate) {
    int month = decdate / 100;
    int day = decdate - month * 100;
    std::string out = "";
    switch (month) {
    case 1:
        out = "January ";
        break;
    case 2:
        out = "February ";
        break;
    case 3:
        out = "March ";
        break;
    case 4:
        out = "April ";
        break;
    case 5:
        out = "May ";
        break;
    case 6:
        out = "June ";
        break;
    case 7:
        out = "July ";
        break;
    case 8:
        out = "August ";
        break;
    case 9:
        out = "September ";
        break;
    case 10:
        out = "October ";
        break;
    case 11:
        out = "November ";
        break;
    case 12:
        out = "December ";
        break;
    default:
        break;
    }

    out += std::to_string(day);
    return out;
}

std::tm make_tm(const int year, const int month, const int day) {
    std::tm tm{};
    tm.tm_year = year - 1900;
    tm.tm_mon = month - 1;
    tm.tm_mday = day;
    return tm;
}

int find_nearest(const int current_decdate,
    const std::map<int, std::unordered_map<std::string, std::time_t>> & birthdays) {
    if (birthdays.empty()) {
        return -1;
    }

    for (const auto& [decdate, _] : birthdays) {
        if (decdate >= current_decdate) {
            return decdate;
        }
    }
    return birthdays.begin()->first;
}

template<typename C>
void print(const C & list, const int indent) {
    for (const auto& [name, date] : list) {
        std::cout << std::setfill(' ') << std::setw(indent) << "" << name
                  << " " << std::put_time(std::localtime(&date), "%B %d, %Y")
                  << "\n";
    }
}

// Task 3. Timer.

void goto_xy(SHORT x, SHORT y) {
    HANDLE hStdOut = ::GetStdHandle(STD_OUTPUT_HANDLE);
    COORD position = { x, y };
    ::SetConsoleCursorPosition(hStdOut, position);
}

void show_time(SHORT col, SHORT row, std::tm & tm) {
    goto_xy(col, row);
    std::cout << "Time Remaining : ";
    std::cout << std::setfill('0') << std::setw(2) << tm.tm_hour << ":"
              << std::setfill('0') << std::setw(2) << tm.tm_min << ":"
              << std::setfill('0') << std::setw(2) << tm.tm_sec << "\n";
}

void change_time(std::tm& tm, int seconds) {
    double s = tm.tm_hour * 3600 + tm.tm_min * 60 + tm.tm_sec + seconds;
    if (s < 0) {
        tm.tm_hour = 0;
        tm.tm_min = 0;
        tm.tm_sec = 0;
        return;
    }

    int h = 0, m = 0;
    if (s >= 3600) {
        h = s / 3600;
        s -= h * 3600;
    }
    tm.tm_hour = h;
    if (s >= 60) {
        m = s / 60;
        s -= m * 60;
    }
    tm.tm_min = m;
    tm.tm_sec = s;
}

int main() {
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
                    std::cout << " " << task.first;
                    if (task.second > 0) {
                        std::cout << ", duration: " << to_time(task.second) << "\n";
                    }
                    else {
                        std::cout << " in progress...\n";
                    }
                }
            }
            std::cout << "Command > "; 
        }
    }

    std::cout << "\nTask 2. Birthday reminder.\n";
    {
        std::map<int, std::unordered_map<std::string, std::time_t>> birthdays;
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
                                if (is_valid_date(nums[0], nums[1], nums[2])) {
                                    auto tm = make_tm(nums[0], nums[1], nums[2]);
                                    auto key = nums[1] * 100 + nums[2];

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
                for (const auto& [key, persons] : birthdays) {
                    std::cout << int_to_date(key) << "\n";
                    print(persons, 4);
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
                        if (is_valid_date(nums[0], nums[1], nums[2])) {
                            auto decdate = nums[1] * 100 + nums[2];

                            auto nearest = find_nearest(decdate, birthdays);

                            if (birthdays.count(nearest) > 0) {
                                auto persons = birthdays.at(nearest);
                                if (nearest == decdate) {
                                    if (persons.size() > 1) {
                                         std::cout << "Today is the birthday of the following people:\n";
                                         print(persons, 4);
                                    }
                                    else {
                                        auto it = persons.begin();
                                        std::tm loc = *std::localtime(&it->second);
                                        std::cout << "Today is " << it->first << "'s birthday!\n";
                                        std::cout  << "He was born on " << std::put_time(&loc, "%B %d, %Y") << "\n";
                                    }
                                }
                                else {
                                    print(persons, 4);
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

    {
        ::system("cls");
        std::cout << "Task 3. Timer.\n";

        std::cout << "Commands for timer.\n";
        std::cout << "\"new\"   - Set new time.\n";
        std::cout << "\"run\"   - Start timer.\n";
        std::cout << "\"exit\"  - Quit.\n";
        const int command_row = 5;
        std::string blank80(80, ' ');
        std::string input = "";
        std::tm saved_tm{};
        do {
            if (input == "exit") {
                break;
            }

            if (input == "new") {
                input.clear();
                std::cout << blank80;
                std::cout << blank80;
                std::cout << blank80;
                std::cout << blank80;
                do {
                    if (input == "exit") {
                        break;
                    }

                    if (input.size() > 0) {
                        int hours = 0, minutes = 0, seconds = 0;
                        auto nums = split_into_numbers(input, ':');

                        auto size = nums.size();
                        if (size > 2) {
                            hours = nums[0];
                            minutes = nums[1];
                            seconds = nums[2];
                        }
                        else if (size > 1) {
                            minutes = nums[0];
                            seconds = nums[1];
                        }
                        else if (size > 0) {
                            seconds = nums[0];
                        }

                        if (minutes >= 0 && minutes <= 60) {
                            if (seconds >= 0 && seconds <= 60) {
                                goto_xy(0, command_row + 1);
                                std::cout << blank80 << "\n";
                                std::cout << blank80 << "\n";
                                std::cout << blank80 << "\n";
                                goto_xy(0, command_row + 1);

                                if (seconds > 59) {
                                    ++minutes;
                                    seconds -= 60;
                                }
                                if (minutes > 59) {
                                    ++hours;
                                    minutes -= 60;
                                }

                                saved_tm.tm_hour = hours;
                                saved_tm.tm_min = minutes;
                                saved_tm.tm_sec = seconds;
                                show_time(4, command_row + 2, saved_tm);
                                break;
                            }
                            else {
                                std::cout << "Seconds must be in the range 0 - 60.\n";
                            }
                        }
                        else {
                            std::cout << "Minutes must be in the range 0 - 60.\n";
                        }
                    }
                    goto_xy(0, command_row + 1);
                    std::cout << blank80 << "\n";
                    std::cout << blank80 << "\n";
                    goto_xy(0, command_row + 1);
                    std::cout << "    Enter the timer time in seconds (s), minutes and secods (mm:ss),\n";
                    std::cout << "    or hours, minutes and secods (hh:mm:ss) > ";
                } while (std::getline(std::cin, input));
            }

            if (input == "run") {
                input.clear();
                double seconds = saved_tm.tm_hour * 3600 + saved_tm.tm_min * 60 + saved_tm.tm_sec;
                std::tm tm{};
                tm.tm_hour = saved_tm.tm_hour;
                tm.tm_min = saved_tm.tm_min;
                tm.tm_sec = saved_tm.tm_sec;

                std::time_t prev = std::time(nullptr);
                std::time_t now = prev;
                std::time_t end = prev + (std::time_t)seconds;
                while (end >= now) {
                    show_time(4, command_row + 2, tm);
                    std::this_thread::sleep_for(std::chrono::seconds(1));
                    now = std::time(nullptr);
                    change_time(tm, prev - now);
                    prev = now;
                }
/*
                while (seconds >= 0) {
                    show_time(4, command_row + 2, tm);
                    std::this_thread::sleep_for(std::chrono::seconds(1));
                    --seconds;
                    change_time(tm, -1);
                }
*/
                std::cout << "\a" << "    Ding! Ding! Ding!\n";
            }

            goto_xy(0, command_row);
            std::cout << blank80;
            goto_xy(0, command_row);
            std::cout << "Command > ";
        } while (std::getline(std::cin, input));

        std::cout << blank80;
        std::cout << blank80;
        std::cout << blank80;
        std::cout << blank80;
        goto_xy(11, command_row);
    }

    return 0;
}