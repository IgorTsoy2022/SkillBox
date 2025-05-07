#include <iostream>
#include <sstream>
#include <iomanip>
#include <algorithm>
#include <cmath>
#include <ctime>
#include <deque>
#include <thread>
#include <mutex>
#include <map>
#include <random>
#include <string>
#include <string_view>
#include <vector>
#include <windows.h>

static bool is_number(const std::string str) {
    if (str.size() < 1) {
        return false;
    }
    for (const auto& c : str) {
        if (c >= '0' && c <= '9') {
            continue;
        }
        if (c == '.' || c == '-' || c == '+') {
            continue;
        }
        return false;
    }
    return true;
}

static std::string to_time(double sec) {
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
    }

    int hours = 0;
    if (sec > 3600 || std::abs(sec - 3600) < epsilon) {
        hours = sec / 3600;
        sec -= hours * 3600;
        sec = sec < 0 ? -sec : sec;
    }
    if (days > 0 || hours > 0) {
        if (days > 0) {
            out += " ";
        }
        if (hours < 10) {
            out += "0";
        }
        output_stream << hours;
        out += output_stream.str() + " h";
        output_stream.str("");
        output_stream.clear();
    }

    int minutes = 0;
    if (sec > 60 || std::abs(sec - 60) < epsilon) {
        minutes = sec / 60;
        sec -= minutes * 60;
        sec = sec < 0 ? -sec : sec;
    }
    if (days > 0 || hours > 0 || minutes > 0) {
        if (days > 0 || hours > 0) {
            out += " ";
        }
        if (minutes < 10) {
            out += "0";
        }
        output_stream << minutes;
        out += output_stream.str() + " m";
        output_stream.str("");
        output_stream.clear();
        out += " ";
    }

    if (sec < 10) {
        out += "0";
    }
    output_stream << sec;
    out += output_stream.str() + " s";

    return out;
}

static void goto_xy(SHORT x, SHORT y) {
    HANDLE hStdOut = ::GetStdHandle(STD_OUTPUT_HANDLE);
    COORD position = { x, y };
    ::SetConsoleCursorPosition(hStdOut, position);
}

std::mutex mtx;

// Task 1. The 100-meter swim

template<typename T>
std::vector<std::vector<T>> snake_passage(size_t rows, size_t cols) {
    std::vector<std::vector<T>> arr(rows, std::vector<T>(cols, T{}));
    int direction = 1;
    T number = 1;
    for (size_t i = 0; i < rows; ++i) {
        for (size_t j = 0; j < cols; ++j) {
            int index = (cols - 1) * (1 - direction) / 2 + j * direction;
            arr[i][index] = number++;
        }
        direction = -direction;
    }
    return arr;
}

template<typename T1, typename T2>
T1 shuttle(const T1 length, const T2 distance) {
    long loops = distance / length;
    T1 rest = distance - loops * length;
    rest = loops % 2 > 0 ? length - rest : rest;
    return rest;
}

class Swimmer {
public:
    Swimmer() {};

    Swimmer(const std::string& name, double speed_record = 0.0)
        : name_(name)
        , speed_record_(speed_record)
    {};

    std::string& name() {
        return name_;
    }

    double& speed_record() {
        return speed_record_;
    }

    ~Swimmer() {};

private:
    std::string name_ = "Unknown";
    double speed_record_ = 0.0;
};

class Swimmers {
public:
    Swimmers() {};

    Swimmer* add(const std::string& name, const double speed_record) {
        if (swimmers_.count(name) > 0) {
            return swimmers_[name];
        }
        Swimmer* swimmer = new Swimmer(name, speed_record);
        swimmers_[swimmer->name()] = swimmer;
        return swimmer;
    }

    void remove(const std::string_view name) {
        if (swimmers_.count(name) > 0) {
            delete swimmers_[name];
            swimmers_.erase(name);
        }
    }

    Swimmer* swimmer(std::string_view name) {
        if (swimmers_.count(name) > 0) {
            return swimmers_[name];
        }
        return nullptr;
    }

    void print() {
        for (const auto& [key, v] : swimmers_) {
            std::cout << key << " " << v->name() << "\n";
        }
    }

    ~Swimmers() {
        for (const auto & [_, swimmer] : swimmers_) {
            delete swimmer;
        }
    };
private:
    std::map<std::string_view, Swimmer*> swimmers_;
};

class Pool;
void track(Pool* pool, const int track_no);

class Pool {
public:
    Pool() = delete;
    
    Pool(const int length, const int distance, Swimmers* swimmers)
        : length_(length)
        , distance_(distance)
        , swimmers_(swimmers)
    {};

    const int length() const {
        return length_;
    }

    const int distance() const {
        return distance_;
    }

    void add(const std::string& name, const double speed_record = 0) {
        if (find(name) != nullptr) {
            std::cout << name << " already exists in the pool." << std::endl;
            return;
        }
        auto swimmer = swimmers_->add(name, speed_record);
        name_width_ = name_width_ > name.size() ? name_width_ : name.size();
        pool_swimmers_.emplace_back(swimmer);
    }

    const int name_width() const {
        return name_width_;
    }

    void remove(const std::string_view name) {
        auto it = std::find_if(pool_swimmers_.begin(), pool_swimmers_.end(),
                               [&name](Swimmer* swimmer) { 
            return swimmer->name() == name;
            });
        if (it == pool_swimmers_.end()) {
            std::cout << name << " doesn't exist in the pool." << std::endl;
            return;
        }

        pool_swimmers_.erase(it);
    }

    Swimmer* find(const std::string name) {
        auto it = std::find_if(pool_swimmers_.begin(), pool_swimmers_.end(),
                               [&name](Swimmer* swimmer) { 
            return swimmer->name() == name;
            });
        if (it == pool_swimmers_.end()) {
            return nullptr;
        }
        return *it;
    }

    const int tracks_count() const {
        return pool_swimmers_.size();
    }

    void shuffle() {
        std::random_device rd;
        std::mt19937 g(rd());
        std::shuffle(pool_swimmers_.begin(), pool_swimmers_.end(), g);
    }

    void sort_by_records() {
        std::sort(pool_swimmers_.begin(), pool_swimmers_.end(),
                  [] (Swimmer* a, Swimmer* b) {
        return a->speed_record() > b->speed_record();
        });
    }

    void sort_by_winners() {
        std::sort(standings_.begin(), standings_.end(),
                  [](std::pair<int, double> a, std::pair<int, double> b) {
            return a.second < b.second;
        });
    }

    Swimmer* pool_swimmer(const int track_no) const {
        return pool_swimmers_[track_no];
    }

    int& in_the_swim() {
        return in_the_swim_;
    }

    void resize_standings() {
        standings_.resize(pool_swimmers_.size(), {});
        in_the_swim_ = pool_swimmers_.size();
    }

    void write_standings(const int track_no, const double seconds) {
        standings_[track_no] = { track_no, seconds };
    }

    void start() {
        resize_standings();
        std::vector<std::thread> threads(pool_swimmers_.size());

        for (int i = 0; i < pool_swimmers_.size(); ++i) {
            threads[i] = std::thread(track, this, i);
        }

        for (auto & thread : threads) {
            if (thread.joinable()) {
                thread.join();
            }
        }
    }

    void print_standings() {
        sort_by_winners();
        std::cout << std::setw((name_width_ + 1) / 2 + 9) << "Standings\n";

        for (const auto& value : standings_) {
            std::cout << " " << std::setw(name_width_ + 1) << std::left
                      << pool_swimmers_[value.first]->name() << std::right
                      << std::fixed << std::setprecision(3);
            if (value.second < 10) {
            	std::cout << " ";
            }
            std::cout << value.second << "\n";
        }
    }

    void print_swimmers() {
        for (const auto& value : pool_swimmers_) {
            std::cout << value->name() << std:: endl;
        }
    }

    void print_high_score() {
        sort_by_records();
        std::cout << std::setw((name_width_ - 3) / 2 + 16)
                  << "High-score table\n";

        for (const auto& value : pool_swimmers_) {
            std::cout << "   " << std::setw(name_width_ + 1) << std::left
                      << value->name() << std::right << std::fixed
                      << std::setprecision(3);
            if (value->speed_record() < 10) {
            	std::cout << " ";
            }
            std::cout << value->speed_record() << "\n";
        }
    }

    void clear() {
        pool_swimmers_.clear();
        standings_.clear();
    }

    ~Pool() {};
private:
    int length_ = 50;
    int distance_ = 100;
    int name_width_ = 0;
    Swimmers* swimmers_ = nullptr;
    
    std::vector<Swimmer*> pool_swimmers_;
    std::vector<std::pair<int, double>> standings_;

    int in_the_swim_ = 0;
};

void track(Pool* pool, const int track_no) {
    int current_time = 0;
    int current_pos = 0;
    int current_speed = std::rand() % (2624 - 1706 + 1) + 1706;
    double elapsed_distance = 0.0;
    while (pool->in_the_swim() > 0) {
        std::this_thread::sleep_for(std::chrono::seconds(1));
        ++current_time;

        mtx.lock();

        if (current_time % 5 == 0) {
             current_speed += std::rand() % 201 - 100;
        }
        elapsed_distance += double(current_speed) / 1000;

        if (current_pos > 0) {
            goto_xy(current_pos, track_no + 2);
            std::cout << " ";
        }

        if (elapsed_distance >= pool->distance()) {
            double elapsed_time = double(current_time) 
                   - (elapsed_distance - pool->distance()) * 1000
                   / double(current_speed);
            pool->write_standings(track_no, elapsed_time);
            current_pos = shuttle(pool->length(), pool->distance())
                        + pool->name_width() + 2;
            goto_xy(current_pos, track_no + 2);
            std::cout << track_no;
            goto_xy(pool->name_width() + 4 + pool->length(), track_no + 2);
            std::cout << std::right << std::fixed << std::setprecision(1)
                      << double(pool->distance());
            --pool->in_the_swim();
            current_time = 0;
        }
        else {
        	current_pos = shuttle(pool->length(), elapsed_distance)
                        + pool->name_width() + 2;
            goto_xy(current_pos, track_no + 2);
            std::cout << track_no;
            goto_xy(pool->name_width() + 4 + pool->length(), track_no + 2);
            std::cout << std::right << std::fixed << std::setprecision(1)
                      << elapsed_distance;
        }

        mtx.unlock();
        if (current_time == 0) {
            return;
        }
    }
}

int main() {
    std::cout << "Task 1. The 100-meter swim.\n";

    std::srand(std::time(nullptr));

    Swimmers swimmers;
    Pool pool(25, 100, &swimmers);

    pool.add("Michael Phelps", 2.624);
    pool.add("Ian Thorpe", 2.059);
    pool.add("Ryan Lochte", 1.972);
    pool.add("Mark Spitz", 1.952);
    pool.add("Johnny Weissmuller", 1.706);
    pool.add("Matt Biondi", 2.043);

    pool.shuffle();
    std::cout << "\n";
    pool.print_swimmers();

    pool.start();

    goto_xy(0, 9);
    pool.print_standings();

    return 0;
}