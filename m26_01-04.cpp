#include <algorithm>
#include <iostream>
#include <ctime>
#include <random>
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

    void set_duration(int seconds) {
        duration_ = seconds;
    }
    int get_duration() const {
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
    Player() {}

    Player(std::vector<Track>& tracks)
        : tracks_(std::move(tracks))
        , current_track_(0) {
        if (tracks_.size() < 1) {
            current_track_ = -1;
        }
        else {
            build_order();
        }
    }

    void add_track(const Track& track) {
        tracks_.push_back(track);
        tracks_order_.push_back(tracks_.size() - 1);
    }

    std::vector<Track> get_tracks() const {
        return tracks_;
    }
    void set_tracks(const std::vector<Track>& tracks) {
        tracks_ = tracks;
        build_order();
        if (mode_ == mode::shuffle) {
            shuffle_tracks();
        }
    }

    mode get_mode() const {
        return mode_;
    }
    void set_mode(const mode& player_mode) {
        if (mode_ == player_mode) {
            return;
        }

        mode_ = player_mode;
        if (mode_ == mode::shuffle) {
            shuffle_tracks();
        }
        else {
            build_order();
        }
    }

    status get_status() const {
        return status_;
    }

    void play() {
        if (status_ == status::on_play) {
            return;
        }

    }

    void stop() {
        if (status_ == status::stopped) {
            return;
        }
    }

    void pause() {
        if (status_ == status::paused) {
            return;
        }
    }

    void fast_forward() {

    }

    void rewind() {

    }

    void repeat() {

    }

    Track get_current_track() {
        if (tracks_.size() > 0) {
            return tracks_[current_track_];
        }
        return {};
    }

    void next() {
        current_track_ = std::rand() % (tracks_.size() + 1);
    }

    ~Player() {}

private:
    std::vector<Track> tracks_;
    std::vector<int> tracks_order_;
    std::time_t start_time_{};
    int current_track_ = 0;
    int duration_ = 0;

    mode mode_ = mode::single;
    status status_ = status::stopped;

    void print_status() {
        auto now = std::time(nullptr);
        if (status_ == status::stopped) {
            std::cout << "Player stopped.\n";
            return;
        }
        if (status_ == status::paused) {
            std::cout << "Player paused.\n";
            if (current_track_ > -1) {
                auto track = &tracks_[tracks_order_[current_track_]];
                std::cout << "Current track: \"" << track->get_name()
                          << "\" " << date_str(track->get_date())
                          << " \n";
            }
            return;
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
	    },
        { "fff", make_tm(1977, 9, 9), 9 },
        { "sss", make_tm(1977, 10, 10), 9 }
	 };

    Player player;
    player = soundtracks;
    for (const auto& t : player.get_tracks()) {
        std::cout << t.get_name() << " " << date_str(t.get_date()) << "\n";
    }


    player.set_mode(mode::shuffle);

    std::vector<int> nums = {
        1, 2, 3 ,4, 5, 6, 7, 8 };
    
    std::random_device rd;
    std::mt19937 g(rd());
    std::shuffle(nums.begin(), nums.end(), g);

    for (const auto& n : nums) {
        std::cout  << n << ", ";
    }
    std::cout << std::endl;

    std::cout << soundtracks.size() << "\n";

    std::cout << "Task 1. Audio player.\n";

//    std::srand(std::time(nullptr));

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