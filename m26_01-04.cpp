#include <iostream>
#include <ctime>
#include <string>
#include <vector>

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

class Track {
public:
    Track() {}

    Track(const std::string name, const std::tm date, const int duration)
        : name_(name)
        , date_(date)
        , duration_(duration) {}

    void set_name(const std::string& name) {
        name_ = name;
    }
    std::string get_name() {
        return name_;
    }

    void set_date(const std::tm date) {
        date_.tm_year = date.tm_year;
        date_.tm_mon = date.tm_mon;
        date_.tm_mday = date.tm_mday;
    }
    std::tm get_date() {
        return date_;
    }

    void set_duration(int seconds) {
        duration_ = seconds;
    }
    int get_duration() {
        return duration_;
    }

    ~Track() {}

private:
    std::string name_ = "";
    std::tm date_{};
    int duration_ = 0;
};

class Player {
public:

    Track get_current_track() {
        if (tracks_.size() > 0) {
            return tracks_[current_track_];
        }
        return {};
    }
 //  std::rand() % (max - min + 1) + min;
     void next() {
         current_track_ = std::rand() % (tracks_.size() + 1);
     }

private:
    std::vector<Track> tracks_;
    int current_track_ = 0;
};

int main() {

    std::cout << "Task 1. Audio player.\n";

    std::srand(std::time(nullptr));

    std::cout << "Commands for audio player:\n";
    std::cout << "\"play\"    - Play current track.\n";
    std::cout << "\"pause\"   - Pause.\n";
    std::cout << "\"next\"    - Go to next (random) track.\n";
    std::cout << "\"stop\"    - Stop.\n";
    std::cout << "\"exit\"    - Exit.\n";

    std::string input = "";
    while (true) {
        if (input == "exit") {
            break;
        }


        std::cout << "Command > ";
        std::getline(std::cin, input);
    }

    return 0;
}