#include <algorithm>
#include <iostream>
#include <sstream>
#include <iomanip>
#include <cmath>
#include <ctime>
#include <thread>
#include <mutex>
#include <map>
#include <random>
#include <string>
#include <string_view>
#include <vector>
#include <windows.h>

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

/*
std::vector<std::string> history;
std::mutex history_access;

void wait_for_call(int time, std::string name) {
    std::this_thread::sleep_for(std::chrono::seconds(time));
    std::cout << "Id: " << std::this_thread::get_id() << std::endl;
    std::cout << "Event " << name << " after " << time << " sec.\n";
    history_access.lock();
    history.push_back(name);
    history_access.unlock();
}
*/

void goto_xy(SHORT x, SHORT y) {
    HANDLE hStdOut = ::GetStdHandle(STD_OUTPUT_HANDLE);
    COORD position = { x, y };
    ::SetConsoleCursorPosition(hStdOut, position);
}

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

template<typename T>
T shuttle(const T length, const T distance) {
    long loops = distance / length;
    T rest = distance - loops * length;
    rest = loops % 2 > 0 ? length - rest : rest;
    return rest;
}


/*
template<typename C>
void print(const C& list, const int width, const int precision) {
    for (const auto& [name, v] : list) {
        std::cout << "   " << std::setw(width) << std::left << name << std::right << std::fixed << std::setprecision(precision);
        if (v < 10) {
        	std::cout << " ";
        }
        std::cout << v  << "\n";
    }
}
*/

std::mutex mtx;

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

    double& elapsed_time() {
        return elapsed_time_;
    }

    double& elapsed_distanse() {
        return elapsed_distance_ ;
    }

    ~Swimmer() {
        std::cout << "~Swimmer " << name_ << std::endl;
    }
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
        std::cout << "~Swimmers" << std::endl;
    };
private:
    std::map<std::string_view, Swimmer*> swimmers_;
};

class Pool {
public:
    Pool() = delete;
    
    Pool(const double length, const double distance, Swimmers* swimmers)
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
        auto it = std::find_if(pool_swimmers_.begin(), pool_swimmers_.end(), [&name](Swimmer* swimmer) { 
            return swimmer->name() == name;
            });
        if (it == pool_swimmers_.end()) {
            std::cout << name << " doesn't exist in the pool." << std::endl;
            return;
        }

        pool_swimmers_.erase(it);
    }

    Swimmer* find(const std::string name) {
        auto it = std::find_if(pool_swimmers_.begin(), pool_swimmers_.end(), [&name](Swimmer* swimmer) { 
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


    void write_standings() {
        for (const auto& swimmer : pool_swimmers_) {
            standings_.push_back({ swimmer, 1});
        }
    }

    void print() {
        std::cout << std::setw((name_width_ + 3) / 2 + 9) << "Standings" << std::endl;

        for (const auto& value : standings_) {
            std::cout << "   " << std::setw(name_width_ + 1) << std::left << value.first->name() << std::right << std::fixed << std::setprecision(3);
            if (value.second < 10) {
            	std::cout << " ";
            }
            std::cout << value.second << "\n";
        }
    }

    void print_records() {
        std::sort(pool_swimmers_.begin(), pool_swimmers_.end(), [] (Swimmer* a, Swimmer* b) {
        return a->speed_record() > b->speed_record();
        });

        std::cout << std::setw((name_width_ - 3) / 2 + 16) << "High-score table" << std::endl;

        for (const auto& value : pool_swimmers_) {
            std::cout << "   " << std::setw(name_width_ + 1) << std::left << value->name() << std::right << std::fixed << std::setprecision(3);
            if (value->speed_record() < 10) {
            	std::cout << " ";
            }
            std::cout << value->speed_record() << "\n";
        }
    }

    void clear() {
        pool_swimmers_.clear();
        standings_.clear();
        threads_.clear();
        place_id_ = 0;
        finished = false;
    }

    ~Pool() {
        std::cout << "~Pool" << std::endl;
    }
private:
    double length_ = 50;
    double distance_ = 100;
    int name_width_ = 0;
    Swimmers* swimmers_ = nullptr;
    
    std::vector<Swimmer*> pool_swimmers_;
    std::vector<std::pair<Swimmer*, double>> standings_;

    bool finished_ = false;
    int place_id_ = 0;
    std::vector<std::thread> threads_;

    void track(Swimmer* swimmer) {
        int current_time = 0;
        int current_speed = std::rand() % (2624 - 1706 + 1) + 1706;
        double elapsed_distance = 0.0;
        while (!finished_) {     std::this_thread::sleep_for(std::chrono::seconds(1));
            ++current_time;
            mtx.lock();

            if (current_time % 5 == 0) {
                 current_speed += std::rand() % (100 + 100 + 1) - 100;
            }
            elapsed_distance += double(current_speed) / 1000;

            if (elapsed_distance >= distance_) {
                double elapsed_time = double(current_time) - (elapsed_distance - distance_) * 1000 / double(current_speed);
                standings_[place_id_++] = { swimmer, elapsed_time };
                std::cout << swimmer->name() << " elapsed distance = " << distance_ << " coord = " << shuttle(length_, distance_) << "\n";
                current_time = 0;
            }
            else {
                std::cout << swimmer->name() << " elapsed distance = " << elapsed_distance << " coord = " << shuttle(length_, elapsed_distance) << "\n";
            }

            mtx.unlock();
            if (current_time == 0) {
                return;
            }
        }
    }
};


// Task 2. Simulation of train station operation


// Task 3. Simulation of online restaurant kitchen operation

int main() {
    std::srand(std::time(nullptr));
    //  std::rand() % (max - min + 1) + min;
    
    auto y = shuttle(25.0, 25.1);
std::cout << "coord=" << y << "\n";

/*
    std::cout << "Id: " << std::this_thread::get_id() << std::endl;

    std::thread call(wait_for_call, 3, "abra");
    std::thread call2(wait_for_call, 3, "abra2");
    call.detach();
    call2.detach();

//    std::this_thread::sleep_for(std::chrono::seconds(5));
//    std::cout << "Another event after " << 5 << " sec.\n";
    std::this_thread::sleep_for(std::chrono::seconds(10));
    if (call.joinable()) call.join();
    if (call2.joinable()) call2.join();

    std::cout << "Missed:\n";
    history_access.lock();
    for (const auto& name : history) {
        std::cout << name << "\n";
    }
    history_access.unlock();
*/

    {
        std::cout << "Task 1. The 100-meter swim.\n";

        Swimmers swimmers;
        Pool pool(50, 100, &swimmers);
        pool.add("Michael Phelps", 2.624);
        pool.add("Ian Thorpe", 2.059);
        pool.add("Ryan Lochte", 1.972);
        pool.add("Mark Spitz", 1.952);
        pool.add("Johnny Weissmuller", 1.706);
        pool.add("Matt Biondi", 2.043);

        pool.print_records();
        pool.shuffle();
        
        pool.print();
        
        int max_speed = 2624;
        int min_speed = 1706;
        std::srand(std::time(nullptr));
        for (int i = 0; i < 6; ++i) {
            auto s= std::rand() % (max_speed - min_speed + 1) + min_speed;
            std ::cout << s << "\n";
            
        }

        std::cout << "The number of concurrent threads is "
                  << std::thread::hardware_concurrency() << "\n";

    }

// Task 2. Simulation of train station operation


// Task 3. Simulation of online restaurant kitchen operation


    return 0;
}