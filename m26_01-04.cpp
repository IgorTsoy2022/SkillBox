#include <algorithm>
#include <iostream>
#include <sstream>
#include <iomanip>
#include <ctime>
#include <random>
#include <map>
#include <set>
#include <string>
#include <string_view>
#include <vector>

#include <windows.h>

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

std::string date_str(const std::tm& tm) {
    std::string out = std::to_string(tm.tm_year + 1900) + ".";
    if (tm.tm_mon < 9) {
        out += "0";
    }
    out += std::to_string(tm.tm_mon + 1) + ".";
    if (tm.tm_mday < 10) {
        out += "0";
    }
    out += std::to_string(tm.tm_mday);
    return out;
}

std::string time_str(int seconds) {
    int minutes = seconds / 60;
    seconds -= minutes * 60;
    std::string out = std::to_string(minutes) + ":";
    if (seconds < 10) {
        out += "0";
    }
    out += std::to_string(seconds);
    return out;
}

// Task 1. Audio player

enum class mode {
    single = 1,
    seriatim = 2,
    shuffle = 3
};

enum class status {
    stopped = 0,
    on_play = 1,
    paused = 2
};

class Track {
public:
    Track() {};

    Track(const std::string& name, const std::tm& date, const int duration)
        : name_(name)
        , date_(date)
        , duration_(duration)
    {};

    void set_name(const std::string& name) {
        name_ = name;
    }
    std::string get_name() const {
        return name_;
    }

    void set_date(const std::tm& date) {
        date_.tm_year = date.tm_year;
        date_.tm_mon = date.tm_mon;
        date_.tm_mday = date.tm_mday;
    }
    std::tm get_date() const {
        return date_;
    }

    void set_duration(const int seconds) {
        duration_ = seconds;
    }
    int get_duration() const {
        return duration_;
    }

    void print() const {
        std::cout << name_ << " " << date_str(date_) 
            << " " << time_str(duration_) << "\n";
    }

    ~Track() {};

private:
    std::string name_ = "";
    std::tm date_{};
    int duration_ = 0;
};

class Player {
public:
    Player() {};

    Player(std::vector<Track>& tracks)
        : tracks_(std::move(tracks)) {
        if (tracks_.size() < 1) {
            current_track_ = -1;
        }
        else {
            current_track_ = 0;
            build_order();
        }
    };

    void add_track(const Track& track) {
        tracks_.push_back(track);
        tracks_order_.push_back(tracks_.size() - 1);
        if (current_track_ < 0) {
            current_track_ = 0;
        }
    }

    void set_tracks(const std::vector<Track>& tracks) {
        tracks_ = tracks;
        if (tracks_.size() < 1) {
            current_track_ = -1;
        }
        else {
            current_track_ = 0;
            build_order();
            if (mode_ == mode::shuffle) {
                shuffle_tracks();
            }
        }
    }
    std::vector<Track> get_tracks() const {
        return tracks_;
    }

    void set_track_break(const int seconds) {
        track_break_ = seconds;
    }
    int get_track_break() const {
        return track_break_;
    }

    void set_mode(const mode& player_mode) {
        if (mode_ == player_mode) {
            return;
        }

        if (player_mode == mode::seriatim &&
             mode_ != mode::seriatim ) {
            build_order();
        }
        else if (player_mode == mode::shuffle) {
            shuffle_tracks();
        }

        mode_ = player_mode;

        switch (mode_) {
        case mode::single:
            std::cout << "Single ";
            break;
        case mode::seriatim:
            std::cout << "Seriatim ";
            break;
        case mode::shuffle:
            std::cout << "Shuffle ";
            break;
        }
        std::cout << "mode is set.\n";
    }
    mode get_mode() const {
        return mode_;
    }

    status get_status() const {
        return status_;
    }

    void play() {
        if (tracks_.size() < 1) {
            return;
        }
        check_status();
        if (status_ == status::on_play) {
            return;
        }
        start_time_ = std::time(nullptr);
        if (status_ == status::paused) {
            start_time_ -= track_elapsed_;
        }
        else {
            track_elapsed_ = 0;
        }
        status_ = status::on_play;
    }

    void play(int track_no) {
        if (tracks_.size() < 1) {
            return;
        }
        check_status();
        if (track_no < 1 || track_no > tracks_.size()) {
            return;
        }
        track_no -= 1;
        for (int i = 0; i < tracks_order_.size(); ++i) {
            if (tracks_order_[i] == track_no) {
                current_track_ = i;
                break;
            }
        }
        status_ = status::on_play;
        track_elapsed_ = 0;
        start_time_ = std::time(nullptr);
    }

    void next() {
        if (tracks_.size() < 1) {
            return;
        }
        check_status();

        if (current_track_ == tracks_.size() - 1) {
            current_track_ = 0;
        }
        else {
            ++current_track_;
        }
        start_time_ = std::time(nullptr);
        track_elapsed_ = 0;
        status_ = status::on_play;
    }

    void pause() {
        check_status();
        if (status_ == status::paused) {
            return;
        }
        track_elapsed_ = std::time(nullptr) - start_time_;
        status_ = status::paused;
    }

    void stop() {
        check_status();
        if (status_ == status::stopped) {
            return;
        }
        track_elapsed_ = 0;
        status_ = status::stopped;
    }

    void fast_forward(const int seconds) {
        check_status();
        if (status_ != status::on_play) {
            return;
        }
        auto max_seconds = 
             tracks_[tracks_order_[current_track_]].get_duration() -
             track_elapsed_;
        if (seconds >= max_seconds) {
             if (current_track_ == tracks_.size() - 1) {
                 current_track_ = 0;
             }
             else {
                 ++current_track_;
             }
             track_elapsed_ = 0;
             status_ = status::stopped;
             return;
        }
        track_elapsed_ += seconds;
        start_time_ -= seconds;
    }

    void rewind(const int seconds) {
        check_status();
        if (status_ != status::on_play) {
            return;
        }
        if (seconds >= track_elapsed_) {
            track_elapsed_ = 0;
            status_ = status::stopped;
            return;
        }
        track_elapsed_ -= seconds;
        start_time_ += seconds;
    }

    void print_status() {
        check_status();

        switch (mode_) {
        case mode::single:
            std::cout << "Single mode.\n";
            break;
        case mode::seriatim:
            std::cout << "Seriatim mode.\n";
            break;
        case mode::shuffle:
            std::cout << "Shuffle mode.\n";
            break;
        }

        if (status_ == status::stopped) {
            std::cout << "Player stopped.\n";
             if (current_track_ > -1) {
                std::cout << "Current track: ";
                tracks_[tracks_order_[current_track_]].print();
                std::cout << "Elapsed time: "
                          << time_str(track_elapsed_) << "\n";
             }
            return;
        }
        if (status_ == status::paused) {
            std::cout << "Player paused.\n";
            if (current_track_ > -1) {
                std::cout << "Current track: ";
                tracks_[tracks_order_[current_track_]].print();
                std::cout << "Elapsed time: "
                          << time_str(track_elapsed_) << "\n";
            }
            return;
        }
        std::cout << "Player on play.\n";
        if (current_track_ > -1) {
            std::cout << "Current track: ";
            tracks_[tracks_order_[current_track_]].print();
            std::cout << "Elapsed time: "
                      << time_str(std::time(nullptr) - start_time_) << "\n";
            std::cout << "track_elapsed_ : " << time_str(track_elapsed_) << "\n";
        }
    }

    void print_list() {
        for (const auto& id : tracks_order_) {
            tracks_[tracks_order_[id]].print();
        }
    }

    ~Player() {};

private:
    std::vector<Track> tracks_;
    std::vector<int> tracks_order_;

    std::time_t start_time_{};
    int current_track_ = 0;
    int track_break_ = 3;
    int track_elapsed_ = 0;

    mode mode_ = mode::single;
    status status_ = status::stopped;

    void check_status() {
        if (status_ != status::on_play) {
            return;
        }

        auto elapsed_time = std::time(nullptr) - start_time_;
        if (mode_ == mode::single) {
            if (elapsed_time >= tracks_[tracks_order_[current_track_]].get_duration()) {
                status_ = status::stopped;
                if (current_track_ == tracks_.size() - 1) {
                    current_track_ = 0;
                }
                else {
                    ++current_track_;
                }
                track_elapsed_ = 0;
            }
            else {
                track_elapsed_ = elapsed_time;
            }
            return;
        }

        for (; current_track_ < tracks_.size(); ++current_track_) {
            auto track_duration = tracks_[tracks_order_[current_track_]].get_duration();
            if (elapsed_time >= track_duration) {
                elapsed_time -= track_duration;
                elapsed_time -= track_break_;
            }
            else {
                start_time_ = std::time(nullptr) - elapsed_time;
                track_elapsed_ = elapsed_time;
                elapsed_time = 0;
                break;
            }
        }
        if (elapsed_time > 0) {
            status_ = status::stopped;
            track_elapsed_ = 0;
            current_track_ = 0;
        }
    }

    void build_order() {
        tracks_order_.resize(tracks_.size(), 0);
        for (int i = 0; i < tracks_.size(); ++i) {
            tracks_order_[i] = i;
        }
    }

    void shuffle_tracks() {
        std::random_device rd;
        std::mt19937 g(rd());
        std::shuffle(tracks_order_.begin(), tracks_order_.end(), g);
    }
};

// Task 2. Mobile phone.

std::string toupper(const std::string& text) {
    std::string result = "";
    for (const auto& c : text) {
        if (c >= 'a' && c <= 'z') {
            result += (c - 32);
        }
        else {
            result += c;
        }
    }
    return result;
}

class Phone {
public:
    Phone() {};

    Phone(const std::vector<std::pair<std::string, std::string>>& phones) {
        for (const auto& p : phones) {
            max_length_ = max_length_ > p.first.size() ?
                max_length_ : p.first.size();
            auto key_name = toupper(p.first);
            auto key_number = phone_to_ll(p.second);

            origin_[key_name] = p.first;
            auto it = origin_.find(key_name);

            names_[it->first].insert(key_number);

            numbers_[key_number] = it->first;
        }
    };

    void add(const std::string& name, const std::string& phone) {
        auto key_name = toupper(name);
        auto key_number = phone_to_ll(phone);
        if (numbers_.count(key_number) > 0) {
            std::cout << "There is a contact with this number in the phone book:\n";
            std::cout << ll_to_phone(key_number) << " "
                      << get_origin_name(numbers_[key_number]) << "\n";
            if (numbers_[key_number] != key_name) {
                std::cout << "Rewrite to " << name << "? (y/n): ";
                std::string input = "";
                std::getline(std::cin, input);
                if (input.size() > 0) {
                    if (input[0] == 'y') {
                        auto old_key_name_size = numbers_[key_number].size();

                        std::string old_key_name = std::string(numbers_[key_number]);

                        auto handler_names = names_.extract(old_key_name);
                        auto handler_origin = origin_.extract(old_key_name);

                        handler_origin.key() = key_name;
                        origin_.insert(std::move(handler_origin));
                        origin_.at(key_name) = name;

                        auto it = origin_.find(key_name);

                        handler_names.key() = it->first;
                        names_.insert(std::move(handler_names));

                        numbers_[key_number] = it->first;

                    	if (name.size() > max_length_) {
                            max_length_ = name.size();
                    	}
                    	else if (old_key_name_size == max_length_) {
                            recalc_max_length();
                    	}
                    }
                }
            }
        }
        else {
            max_length_ = max_length_ > name.size() ?
                          max_length_ : name.size();

            origin_[key_name] = name;
            auto it = origin_.find(key_name);

            names_[it->first].insert(key_number);

            numbers_[key_number] = it->first;
        }
    }

    void call(const std::string& subscriber) {
        if (subscriber.size() < 1) {
            return;
        }

        if (subscriber[0] == '+' ||
            subscriber[0] >= '0' && subscriber[0] <= '9') {
            auto key_number = phone_to_ll(subscriber);
            std::cout << "Call " << ll_to_phone(key_number) << " ";
            if (numbers_.count(key_number) > 0) {
                std::cout << get_origin_name(numbers_[key_number]) << "\n";
                return;
            }
            else {
                std::cout << "\nThere is no record with that number in the book.\n";
            }

            std::cout << "Put a phone number in the book? (y/n): ";
            std::string input = "";
            std::getline(std::cin, input);
            if (input.size() > 0) {
                if (input[0] == 'y') {
                    std::cout << "Enter the subscriber's name: ";
                    input.clear();
                    std::getline(std::cin, input);
                    if (input.size() > 0) {
                        max_length_ = max_length_ > input.size() ?
                                      max_length_ : input.size();
                        auto key_name = toupper(input);

                        origin_[key_name] = input;
                        auto it = origin_.find(key_name);

                        names_[it->first].insert(key_number);

                        numbers_[key_number] = it->first;
                    }
                }
            }
            return;
        }

        auto key_name = toupper(subscriber);
        if (names_.count(key_name) > 0) {
            auto it = names_.at(key_name).begin();

            if (names_.at(key_name).size() > 1) {
                std::cout << get_origin_name(key_name)
                          << " has the following phone numbers:\n";
                int id = 0;
                for (const auto& num : names_.at(key_name)) {
                    std::cout << ++id << ". " << ll_to_phone(num) << "\n";
                }

                std::cout << "Select the number (1 - " << id << "): ";
                std::string input = "";
                std::getline(std::cin, input);
                if (is_number(input)) {
                    int count = std::stod(input);
                    if (count > 0 && count <= id) {
                        std::advance(it, count - 1);
                        std::cout << "Call " << get_origin_name(key_name) << " "
                                  << ll_to_phone(*it) << "\n";
                    }
                }
                return;
            }

            if (it != names_.at(key_name).end()) {
                std::cout << "Call " << get_origin_name(key_name) << " "
                          << ll_to_phone(*it) << "\n";
            }
            return;
        }

        std::cout << "There is no record with subscriber's name: " 
                  << subscriber << " in the book.\n";
    }

    void sms(const std::string& subscriber) {
        if (subscriber.size() < 1) {
            return;
        }

        if (subscriber[0] == '+' ||
            subscriber[0] >= '0' && subscriber[0] <= '9') {
            auto key_number = phone_to_ll(subscriber);
            std::cout << "sms text message to " << ll_to_phone(key_number) << " ";
            if (numbers_.count(key_number) > 0) {
                std::cout << get_origin_name(numbers_[key_number]) << "\n";
                std::cout << "Enter text message to send:\n";
                std::string text = "";
                std::getline(std::cin, text);
                return;
            }
            else {
                std::cout << "\nThere is no record with that number in the book.\n";
            }

            std::cout << "Put a phone number in the book? (y/n): ";
            std::string input = "";
            std::getline(std::cin, input);
            if (input.size() > 0) {
                if (input[0] == 'y') {
                    std::cout << "Enter the subscriber's name: ";
                    input.clear();
                    std::getline(std::cin, input);
                    if (input.size() > 0) {
                        max_length_ = max_length_ > input.size() ?
                            max_length_ : input.size();
                        auto key_name = toupper(input);

                        origin_[key_name] = input;
                        auto it = origin_.find(key_name);

                        names_[it->first].insert(key_number);

                        numbers_[key_number] = it->first;
                    }
                }
            }
            return;
        }

        auto key_name = toupper(subscriber);
        if (names_.count(key_name) > 0) {
            auto it = names_.at(key_name).begin();
            if (names_.at(key_name).size() > 1) {
                std::cout << get_origin_name(key_name)
                          << " has the following phone numbers:\n";
                int id = 0;
                for (const auto& num : names_.at(key_name)) {
                    std::cout << ++id << ". " << ll_to_phone(num) << "\n";
                }

                std::cout << "Select the number (1 - " << id << "): ";
                std::string input = "";
                std::getline(std::cin, input);
                if (is_number(input)) {
                    int count = std::stod(input);
                    if (count > 0 && count <= id) {
                        std::advance(it, count - 1);
                        std::cout << "sms text message to "
                                  << get_origin_name(key_name) << " "
                                  << ll_to_phone(*it) << "\n";
                        std::cout << "Enter text message to send:\n";
                        std::string text = "";
                        std::getline(std::cin, text);
                    }
                }
                return;
            }

            if (it != names_.at(key_name).end()) {
                std::cout << "sms text message to "
                          << get_origin_name(key_name) << " "
                          << ll_to_phone(*it) << "\n";
                std::cout << "Enter text message to send:\n";
                std::string text = "";
                std::getline(std::cin, text);
            }
            return;
        }

        std::cout << "There is no record with subscriber's name: "
                  << subscriber << " in the book.\n";
    }

    void list_numbers(const std::string_view format = "+# (###) ###-####") {
        for (const auto& [key_number, name] : numbers_) {
            std::cout << ll_to_phone(key_number, format) << " "
                      << get_origin_name(name) << "\n";
        }
    }

    void list_names(const std::string_view format = "+# (###) ###-####") {
        for (const auto& [key_name, numbers] : names_) {
            bool padding = false;
            for (const auto& num : numbers) {
                std::cout << std::left << std::setw(max_length_ + 1);

                if (!padding) {
                    std::cout << get_origin_name(key_name);
                }
                else {
                    std::cout << " ";
                }

                std::cout << ll_to_phone(num, format) << "\n";
                padding = true;
            }
        }
    }

    ~Phone() {};

private:
    std::map<std::string, std::string> origin_;
    std::map<long long, std::string_view> numbers_;
    std::map<std::string_view, std::set<long long>> names_;
    int max_length_ = 0;

    void recalc_max_length() {
        max_length_ = 0;
        for (const auto& [key, _] : names_) {
            max_length_ = max_length_ > key.size() ? 
                          max_length_ : key.size();
        }
    }

    std::string_view get_origin_name(const std::string_view key_name) {
        std::string key(key_name);
        if (origin_.count(key) > 0) {
            return origin_.at(key);
        }
        return "";
    }

    long long phone_to_ll(const std::string_view input) {
        const int MAX_DIGITS = 11;
        long long number = 0;
        int digits = 0;
        for (const char & c : input) {
            if (c >= '0' && c <= '9') {
                ++digits;
                number *= 10;
                number += c - 48;
                if (digits == MAX_DIGITS) {
                    return number;
                }
            }
        }
    
        return number;
    }

    std::string ll_to_phone(long long number,
                            const std::string_view format = "+# (###) ###-####") {
        std::string out = "";
        long long divisor = 0;
        for (const auto & c : format) {
            if (c == '#') {
                if (divisor == 0) {
            	    divisor = 1;
                }
                else {
                    divisor *= 10;
                }
            }
        }
    
        for (const auto & c : format) {
            if (c == '#') {
                auto curnum = number / divisor;
                out += std::to_string(curnum);
                number -= curnum * divisor;
                divisor /= 10;
            }
            else {
                out += c;
            }
        }

        return out;
    }

};

// Task 3. Desktop window.

class Vector {
public:
    Vector() {};

    Vector(const double x, const double y)
        : x_(x)
        , y_(y)
    {};

    Vector(const Vector& v)
        : x_(v.x_)
        , y_(v.y_)
    {};

    double get_x() const {
        return x_;
    }

    double get_y() const {
        return y_;
    }

    void set_x(const double value) {
        x_ = value;
    }

    void set_y(const double value) {
        y_ = value;
    }

    double length() const {
        return std::sqrt(x_ * x_ + y_ * y_);
    }

    Vector& operator=(const Vector& right) {
        if (this == &right) {
            return *this;
        }

        x_ = right.x_;
        y_ = right.y_;

        return *this;
    }

    const Vector& operator-() {
        return Vector(-x_, -y_);
    }

    friend Vector operator+(const Vector& a, const Vector& b) {
        return Vector(a.x_ + b.x_, a.y_ + b.y_);
    }

    friend Vector operator-(const Vector& a, const Vector& b) {
        return Vector(a.x_ - b.x_, a.y_ - b.y_);
    }

    friend Vector operator*(const Vector& a, const double c) {
        return Vector(a.x_ * c, a.y_ * c);
    }

    friend Vector operator*(const double c, const Vector& a) {
        return Vector(a.x_ * c, a.y_ * c);
    }

    ~Vector() {};

private:
    double x_ = 0;
    double y_ = 0;
};

Vector normalize(const Vector& v) {
    auto length = v.length();
    if (length > 0) {
        return Vector(v.get_x() / length, v.get_y() / length);
    }
    return {};
}


void goto_xy(SHORT x, SHORT y) {
    HANDLE hStdOut = ::GetStdHandle(STD_OUTPUT_HANDLE);
    COORD position = { x, y };
    ::SetConsoleCursorPosition(hStdOut, position);
}

class Coord {
public:
    Coord() {};

    Coord(const int x, const int y)
        : x_(x)
        , y_(y)
    {};

    Coord(const Coord& v)
        : x_(v.x_)
        , y_(v.y_)
    {};

    int get_x() const {
        return x_;
    }

    int get_y() const {
        return y_;
    }

    void set_x(const int value) {
        x_ = value;
    }

    void set_y(const int value) {
        y_ = value;
    }

    Coord& operator=(const Coord& right) {
        if (this == &right) {
            return *this;
        }

        x_ = right.x_;
        y_ = right.y_;

        return *this;
    }

    const Coord& operator-() {
        return Coord(-x_, -y_);
    }

    friend Coord operator+(const Coord& a, const Coord& b) {
        return Coord(a.x_ + b.x_, a.y_ + b.y_);
    }

    friend Coord operator+=(Coord& left, const Coord& right) {
        left.x_ += right.x_;
        left.y_ += right.y_;
        return left;
    }

    friend Coord operator-(const Coord& a, const Coord& b) {
        return Coord(a.x_ - b.x_, a.y_ - b.y_);
    }

    friend Coord operator-=(Coord& left, const Coord& right) {
        left.x_ -= right.x_;
        left.y_ -= right.y_;
        return left;
    }

    friend Coord operator*(const Coord& a, const double c) {
        return Coord(a.x_ * c, a.y_ * c);
    }

    friend Coord operator*(const double c, const Coord& a) {
        return Coord(a.x_ * c, a.y_ * c);
    }

    friend bool operator==(const Coord& left, const Coord& right) {
        return ((left.x_ == right.x_) && (left.y_ == right.y_));
    }

    ~Coord() {};

private:
    int x_ = 0;
    int y_ = 0;
};

class Window {
public:
    Window() {};

    Window(const int size_x, const int size_y)
        : size_x_(size_x)
        , size_y_(size_y)
    {};

    Window(const int size_x, const int size_y,
           const int pos_x, const int pos_y)
        : size_x_(size_x)
        , size_y_(size_y)
        , pos_x_(pos_x)
        , pos_y_(pos_y)
    {
        std::cout << "size_x_=" << size_x_ << " size_y_=" << size_y_ << " pos_x_=" << pos_x_ << " pos_y_=" << pos_y_ << "\n";
    };

    Window(const Window& wnd)
        : size_x_(wnd.size_x_)
        , size_y_(wnd.size_y_)
        , pos_x_(wnd.pos_x_)
        , pos_y_(wnd.pos_y_)
    {};

    Window(const Coord& size, const Coord& pos)
        : size_x_(size.get_x())
        , size_y_(size.get_y())
        , pos_x_(pos.get_x())
        , pos_y_(pos.get_y())
    {};

    Window& operator=(const Window& right) {
        if (this == &right) {
            return *this;
        }

        size_x_ = right.size_x_;
        size_y_ = right.size_y_;
        pos_x_ = right.pos_x_;
        pos_y_ = right.pos_y_;
        hide_ = right.hide_;

        return *this;
    }

    Window& operator=(Window&& right) noexcept {
        if (this == &right) {
            return *this;
        }

        size_x_ = right.size_x_;
        size_y_ = right.size_y_;
        pos_x_ = right.pos_x_;
        pos_y_ = right.pos_y_;
        hide_ = right.hide_;

        right.hide_ = true;
        right.size_x_ = 0;
        right.size_y_ = 0;
        right.pos_x_ = 0;
        right.pos_y_ = 0;

        return *this;
    }

    Coord get_size() const {
        return { size_x_, size_y_ };
    }
    void set_size(const Coord& size) {
        size_x_ = size.get_x();
        size_y_ = size.get_y();
    }
    void set_size(const int width, const int height) {
        size_x_ = width;
        size_y_ = height;
    }

    Coord get_pos() const {
        return { pos_x_, pos_y_ };
    }
    void set_pos(const Coord& pos) {
        size_x_ = pos.get_x();
        size_y_ = pos.get_y();
    }
    void set_pos(const int x, const int y) {
        size_x_ = x;
        size_y_ = y;
    }

    void hide_window(const bool hide) {
        hide_ = hide;
    }

    bool visible() {
        return !hide_;
    }

    ~Window() {};

private:
    bool hide_ = true;
    int size_x_ = 0;
    int size_y_ = 0;
    int pos_x_ = 0;
    int pos_y_ = 0;
};

class Screen {
public:
    Screen() {};

    Screen(const int size_x, const int size_y)
       : size_x_(size_x)
       , size_y_(size_y)
    {};

    Screen(const int size_x, const int size_y,
           const int pos_x, const int pos_y)
        : size_x_(size_x)
        , size_y_(size_y)
        , pos_x_(pos_x)
        , pos_y_(pos_y)
    {};

    Screen(const Coord& size, const Coord& pos)
        : size_x_(size.get_x())
        , size_y_(size.get_y())
        , pos_x_(pos.get_x())
        , pos_y_(pos.get_y())
    {};

    void set_window(int width, int height, int x, int y) {
        if (x < pos_x_) {
            x = pos_x_;
            if (width > size_x_) {
                width = size_x_;
            }
        }
        else if (x > pos_x_ + size_x_) {
            x = pos_x_ + size_x_;
            width = 0;
        }
        if (y < pos_y_) {
            y = pos_y_;
            if (height > size_y_) {
                height = size_y_;
            }
        }
        else if (y > pos_y_ + size_y_) {
            y = pos_y_ + size_y_;
            height = 0;
        }
        wnd_ = { width, height, x, y };
        std::cout << wnd_.get_pos().get_x() << " " << wnd_.get_pos().get_y() << "\n";
        
    }

    Window get_window() {
        return wnd_;
    }

    void hide_window(const bool hide) {
        wnd_.hide_window(hide);
        draw_window();
    }

    void draw_window() {
        char c = wnd_.visible() ? '1' : '0';
        auto row = wnd_.get_pos().get_y();
        auto max_row = row + wnd_.get_size().get_y();
        auto col = wnd_.get_pos().get_x();
        auto max_col = col + wnd_.get_size().get_x();
        for (; row < max_row; ++row) {
            goto_xy(col, row);
            for (auto i = col; i < max_col; ++i) {
                std::cout << c;
            }
            std::cout << "\n";
        }
    }

    void draw_screen() {
        
        char c = wnd_.visible() ? '1' : '0';
        auto wnd_pos = wnd_.get_pos();
        auto wnd_size = wnd_.get_size();
        auto left = wnd_pos.get_x();
        auto right = left + wnd_size.get_x();
        auto top = wnd_pos.get_y();
        auto bottom = top + wnd_size.get_y();
        for (auto row = pos_y_; row < pos_y_ + size_y_; ++row) {
            goto_xy(pos_x_, row);
            for (auto col = pos_x_; col < pos_x_ + size_x_; ++col) {
                if (row >= top && row <= bottom &&
                    col >= left && col <= right) {
                    std::cout << c;
                }
                else {
                    std::cout << '0';
                }
            }
//            std::cout << "\n";
        }
    }

    void close_window() {
        wnd_.hide_window(true);
        draw_window();
        wnd_ = {};
    }

    ~Screen() {};

private:
    int size_x_ = 0;
    int size_y_ = 0;
    int pos_x_ = 0;
    int pos_y_ = 0;
    Window wnd_{};
};

int main() {

    const int SCREEN_WIDTH = 10; // 80;
    const int SCREEN_HEIGHT = 10; //50;

    int prompt_row = 8;
    int screen_row = 10;
    ::system("cls");

    std::cout << "Commands for window:\n";
    std::cout << "\"window\"  - Make window.\n";
    std::cout << "\"move\"    - Move the window.\n";
    std::cout << "\"resize\"  - Resize the window.\n";
    std::cout << "\"display\" - Display the window.\n";
    std::cout << "\"hide\"    - Hide the window.\n";
    std::cout << "\"close\"   - Close the window and exit.\n";

    Screen scr(SCREEN_WIDTH, SCREEN_HEIGHT, 0, screen_row);
    goto_xy(0, screen_row);
//    scr.draw_screen();

    std::string blank90(90, ' ');
    std::string input = "";
    while (true) {
        if (input == "close") {
            scr.close_window();
            goto_xy(0, screen_row + SCREEN_HEIGHT);
            break;
        }

        if (input == "window") {
            goto_xy(0, prompt_row);
            std::cout << blank90 << "\n";
            std::cout << blank90 << "\n";
            goto_xy(0, prompt_row);
            std::cout << "Enter the dimensions and coordinates of the window: width, height, x, y:\n";
            std::cout << "> ";
            std::getline(std::cin, input);
            std::string str1 = "";
            std::string str2 = "";
            std::string str3 = "";
            std::string str4 = "";
            std::stringstream input_stream(input);
            input_stream >> str1 >> str2 >> str3 >> str4;
            int width = 0, height = 0, x = 0, y = 0;
            if (is_number(str1)) {
                width = std::stod(str1);
            }
            if (is_number(str2)) {
                height = std::stod(str2);
            }
            if (is_number(str3)) {
                x = std::stod(str3);
            }
            if (is_number(str4)) {
                y = std::stod(str4);
            }
            if (width > 0 && height > 0) {
                scr.set_window(width, height, x, y);
            //    scr.hide_window(false);

            }
        }

        goto_xy(0, prompt_row);
        std::cout << blank90 << "\n";
        std::cout << blank90 << "\n";
        goto_xy(0, prompt_row);
        std::cout << "Command > ";
        std::getline(std::cin, input);
    }

return 0;

    std::cout << "Task 1. Audio player.\n";
    {
        std::vector<Track> soundtracks = {
            { "The lonely shepherd",
              make_tm(1977, 06, 06), 260
            },
            { "Comfortably numb",
              make_tm(1979, 12, 06), 383
            },
            { "Nothing else matters",
              make_tm(1991, 8, 12), 388
            },
            { "Across the Universe",
              make_tm(1969, 12, 6), 229
            },
            { "How deep is your love",
              make_tm(1977, 9, 6), 230
            }
        };

        Player player;
        player = soundtracks;

        std::cout << "Commands for audio player:\n";
        std::cout << "\"mode n\" - Set mode (1 - single / 2 - seriatim / 3 - shuffle).\n";
        std::cout << "\"play\"   - Play current track.\n";
        std::cout << "\"play n\" - Play track n.\n";
        std::cout << "\"ff n\"   - Fast forward n seconds.\n";
        std::cout << "\"rw n\"   - Rewind n seconds.\n";
        std::cout << "\"pause\"  - Pause.\n";
        std::cout << "\"next\"   - Next track.\n";
        std::cout << "\"stop\"   - Stop.\n";
        std::cout << "\"status\" - Show status.\n";
        std::cout << "\"exit\"   - Exit.\n";

        std::string input = "";
        std::string number = "";
        while (true) {
            if (input == "exit") {
                player.stop();
                player.print_status();
                break;
            }

            number.clear();
            std::stringstream input_stream(input);
            input_stream >> input >> number;
            if (input == "mode" && number.size() > 0) {
                if (number == "1" || number == "single") {
                    player.set_mode(mode::single);
                }
                else if (number == "2" || number == "seriatim") {
                    player.set_mode(mode::seriatim);
                }
                else if (number == "3" || number == "shuffle") {
                    player.set_mode(mode::shuffle);
                }
            }

            if (input == "play") {
                if (is_number(number)) {
                    player.play(std::stod(number));
                }
                else {
                    player.play();
                }
                player.print_status();
            }

            if (input == "ff") {
                if (is_number(number)) {
                    player.fast_forward(std::stod(number));
                    player.print_status();
                }
            }

            if (input == "rw") {
                if (is_number(number)) {
                    player.rewind(std::stod(number));
                    player.print_status();
                }
            }

            if (input == "pause") {
                player.pause();
                player.print_status();
            }

            if (input == "next") {
                player.next();
                player.print_status();
            }

            if (input == "stop") {
                player.stop();
                player.print_status();
            }

            if (input == "status") {
                player.print_status();
            }

            std::cout << "Command > ";
            std::getline(std::cin, input);
        }
    }

    std::cout << "Task 2. Mobile phone.\n";
    {
        std::vector<std::pair<std::string, std::string>> phones = {
            { "Jhon Travolta", "+7 (965) 123-3556" },
            { "Sandra Bulloc", "79321457888" },
            { "Tom Hanks", "+7(555)223-45-67" },
            { "Bob Dilan", "+79651233566" },
            { "Jhon Travolta", "+78775678900"},
            { "Sigourney Weaver", "+78985678908" }
        };

        Phone p = phones;
        phones.clear();

        std::cout << "Commands for mobile phone:\n";
        std::cout << "\"add\"  - Enroll a caller in the address book.\n";
        std::cout << "           For a phone number, the first 11 digits are relevant.\n";
        std::cout << "\"call\" - Call a subscriber.\n";
        std::cout << "\"sms\"  - Send sms.\n";
        std::cout << "\"list\" - Show subscribers.\n";
        std::cout << "\"exit\" - Exit.\n";

        std::string input = "";
        while (true) {
            if (input == "exit") {
                break;
            }

            if (input == "add") {
                std::string name = "Unknown";
                std::string phone = "";
                std::cout << "Enter the name of the subscriber: ";
                std::getline(std::cin, name);
                std::cout << "Enter an 11-digit phone number in any format: ";
                std::getline(std::cin, phone);
                if (phone.size() > 0) {
                    p.add(name, phone);
                }
            }

            if (input == "call") {
                std::string subscriber = "";
                std::cout << "Enter the name or phone number of the subscriber.\n";
                std::cout << "The number can be entered in any format and must contain 11 digits:\n";
                std::getline(std::cin, subscriber);
                if (subscriber.size() > 0) {
                    p.call(subscriber);
                }
            }

            if (input == "sms") {
                std::string subscriber = "";
                std::cout << "Enter the name or phone number of the subscriber.\n";
                std::cout << "The number can be entered in any format and must contain 11 digits:\n";
                std::getline(std::cin, subscriber);
                if (subscriber.size() > 0) {
                    p.sms(subscriber);
                }
            }

            if (input == "list") {
                p.list_names();
            }

            std::cout << "Command > ";
            std::getline(std::cin, input);
        }
    }

    std::cout << "Task 3. Desktop window.\n";
    {

    }

    return 0;
}