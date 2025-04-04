#include <algorithm>
#include <iostream>
#include <sstream>
#include <ctime>
#include <random>
#include <string>
#include <vector>

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
    Track() {}

    Track(const std::string& name, const std::tm& date, const int duration)
        : name_(name)
        , date_(date)
        , duration_(duration) {}

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

    ~Track() {}

private:
    std::string name_ = "";
    std::tm date_{};
    int duration_ = 0;
};

class Player {
public:
    Player() {}

    Player(std::vector<Track>& tracks)
        : tracks_(std::move(tracks)) {
        if (tracks_.size() < 1) {
            current_track_ = -1;
        }
        else {
            current_track_ = 0;
            build_order();
        }
    }

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
        auto max_seconds = tracks_[tracks_order_[current_track_]].get_duration() - track_elapsed_;
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
                std::cout << "Elapsed time: " << time_str(track_elapsed_) << "\n";
             }
            return;
        }
        if (status_ == status::paused) {
            std::cout << "Player paused.\n";
            if (current_track_ > -1) {
                std::cout << "Current track: ";
                tracks_[tracks_order_[current_track_]].print();
                std::cout << "Elapsed time: " << time_str(track_elapsed_) << "\n";
            }
            return;
        }
        std::cout << "Player on play.\n";
        if (current_track_ > -1) {
            std::cout << "Current track: ";
            tracks_[tracks_order_[current_track_]].print();
            std::cout << "Elapsed time: " << time_str(std::time(nullptr) - start_time_) << "\n";
            std::cout << "track_elapsed_ : " << time_str(track_elapsed_) << "\n";
        }
    }

    void print_list() {
        for (const auto& id : tracks_order_) {
            tracks_[tracks_order_[id]].print();
        }
    }

    ~Player() {}

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

int main() {
	
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

    std::cout << "Task 1. Audio player.\n";

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

    return 0;
}