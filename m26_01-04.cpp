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
    std::string get_name() const {
        return name_;
    }

    void set_date(const std::tm date) {
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
        track_order_.push_back(tracks_.size()  - 1);
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
    std::vector<int> track_order_;
    int current_track_ = 0;
    int duration_ = 0;
    std::time_t start_time_{};
    enum class mode {
        single = 1,
        seriatim = 2,
        shuffle = 3
    } mode_ = mode::single;

    void build_order() {
        track_order_.resize(tracks_.size(), 0);
        for (int i = 0; i < tracks_.size(); ++i) {
            track_order_[i] = i;
        }
    }

    void shuffle_tracks() {
        std::random_device rd;
        std::mt19937 g(rd());
        std::shuffle(track_order_.begin(), track_order_.end(), g);
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
	
	for (auto& t : soundtracks) {
	    std::cout << t.get_name() << " " << t.get_date().tm_year + 1900 << "." << t.get_date().tm_mon + 1  << "." << t.get_date().tm_mday << "\n";
	}

    Player player(soundtracks);

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