#include <iostream>
#include <iomanip>
#include <thread>
#include <mutex>
#include <map>
#include <random>
#include <windows.h>

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

class Pool {
public:
    Pool() = delete;
    
    Pool(const int length, const int distance, Swimmers* swimmers)
        : length_(length)
        , distance_(distance)
        , swimmers_(swimmers)
    {};

    void add(const std::string& name, const double speed_record = 0) {
        if (find(name) != nullptr) {
            std::cout << name << " already exists in the pool." << std::endl;
            return;
        }
        auto swimmer = swimmers_->add(name, speed_record);
        name_width_ = name_width_ > name.size() ? name_width_ : name.size();
        pool_swimmers_.emplace_back(swimmer);
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

    void start() {
        in_the_swim_ = pool_swimmers_.size();
        standings_.resize(in_the_swim_, {});
        std::vector<std::thread> threads(in_the_swim_);

        for (int i = 0; i < in_the_swim_; ++i) {
            int speed = std::rand() % (2624 - 1706 + 1) + 1706;
            threads[i] = std::thread(&Pool::track, this, speed, i);
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
            std::cout << std::setw(name_width_ + 1) << std::left
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
            std::cout << std::setw(name_width_ + 1) << std::left
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

    void track(const int speed, const int track_no) {
        int current_time = 0;
        int current_pos = 0;
        int right_border = name_width_ + 4 + length_;

        int current_speed = speed;

        double elapsed_distance = 0.0;
        while (in_the_swim_ > 0) {
            std::this_thread::sleep_for(std::chrono::seconds(1));
            ++current_time;

            mtx.lock();

            if (current_time % 2 == 0) {
                current_speed += std::rand() % 201 - 100;
            }
            elapsed_distance += double(current_speed) / 1000;

            if (current_pos > 0) {
                goto_xy(current_pos, track_no + 2);
                std::cout << " ";
            }

            if (elapsed_distance >= distance_) {
                double elapsed_time = double(current_time) 
                       - (elapsed_distance - distance_) * 1000
                       / double(current_speed);
                standings_[track_no] = { track_no, elapsed_time };

                current_pos = shuttle(length_, distance_) + name_width_ + 2;
                goto_xy(current_pos, track_no + 2);
                std::cout << track_no;
                goto_xy(right_border, track_no + 2);
                std::cout << std::right << std::fixed << std::setprecision(1)
                          << double(distance_);

                --in_the_swim_;
                current_time = 0;
            }
            else {
        	    current_pos = shuttle(length_, elapsed_distance) + name_width_ + 2;
                goto_xy(current_pos, track_no + 2);
                std::cout << track_no;
                goto_xy(right_border, track_no + 2);
                std::cout << std::right << std::fixed << std::setprecision(1)
                          << elapsed_distance;
            }

            mtx.unlock();
            if (current_time == 0) {
                return;
            }
        }
    }
};

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

    goto_xy(0, 2);
    pool.print_swimmers();

    pool.start();

    goto_xy(0, 8);
    pool.print_standings();
    pool.print_high_score();

    return 0;
}