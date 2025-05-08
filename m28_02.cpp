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

// Task 2. Simulation of train station operation

class Station;
void train(Station* station, const int train_no, const int row);
void station_command(Station* station, const int row);

class Station {
public:
    Station() = delete;
    
    Station(const int trains_count)
        : trains_count(trains_count)
    {
        trains_durations_.resize(trains_count, 0);
    };

    int trains_count = 0;

    bool is_free = true;

    bool departure = false;

    int row = 0;

    int& train_duration(int id) {
        return trains_durations_[id];
    }

    void start() {
        std::vector<std::thread> threads(trains_count);

        for (int i = 0; i < trains_count; ++i) {
            threads[i] = std::thread(train, this, i, row);
        }
        std::thread station(station_command, this, row + 3);

        if (station.joinable()) {
            station.join();
        }
        for (auto & thread : threads) {
            if (thread.joinable()) {
                thread.join();
            }
        }
    }

    ~Station() {};
private:
    std::vector<int> trains_durations_;
};

void train(Station* station, const int id, const int row) {
    bool on_platform = false;
    int current_time = 0;
    std::string blank10(16, ' ');

    while(true) {
        std::this_thread::sleep_for(std::chrono::seconds(1));
    	++current_time;

        mtx.lock();

        if (current_time >= station->train_duration(id)) {
            goto_xy(10, row + id);
            if (station->is_free) {
                std::cout << "has arrived";
                std::cout << blank10;
                station->is_free = false;
                on_platform = true;
            }
            else {
                std::cout << "waiting for free platform";
            }
            current_time = 0;
        }
        else {
            goto_xy(23, row + id);
            std::cout << "expected in " << to_time(station->train_duration(id) - current_time);
            std::cout << blank10;	
        }

        mtx.unlock();

        if (current_time == 0) {
            break;
        }
    }

    while(!on_platform) {
        std::this_thread::sleep_for(std::chrono::seconds(1));

        mtx.lock();

        if (station->is_free) {
            goto_xy(10, row + id);
            std::cout << "has arrived";
            std::cout << blank10;
            station->is_free = false;
            on_platform = true;
        }

        mtx.unlock();
    }

    while (on_platform) {
         std::this_thread::sleep_for(std::chrono::seconds(1));

        mtx.lock();

        if (station->departure) {
            goto_xy(10, row + id);
            std::cout << "departured";
            std::cout << blank10;
            station->is_free = true;
            station->departure = false;
            on_platform = false;
            --station->trains_count;
        };

        mtx.unlock();
    }
}

void station_command(Station* station, const int row) {
    std::string blank(30, ' ');
    goto_xy(0, row);
    std::string input = "";

    mtx.lock();
    std::cout << "Station operation > ";
    mtx.unlock();

    while(true) {
        std::this_thread::sleep_for(std::chrono::seconds(1));

        mtx.lock();

        int trains_count = station->trains_count;
        if (!station->is_free) {
            goto_xy(21, row);
            std::cout << blank;
            goto_xy(21, row);
            std::getline(std::cin, input);
            if (input == "depart") {
                station->departure = true;
            }
        }

        mtx.unlock();

        if (trains_count < 1) {
            return;
        }
    }
}

int main() {
    std::cout << "Task 2. Simulation of train station operation.\n";

    int trains_count = 3;
    int current_row = 3;

    Station Paddington(3);
    std::vector<int> trains(trains_count, 0);
    std::cout << "Enter travel time for each train.\n";
    std::cout << "Enter \"exit\" for exit.\n";
    std::string input = "";
    std::string blank40(60, ' ');
    int train_id = 0;
    while (true) {
        std::cout << "Train #" << train_id << " > ";
        std::getline(std::cin, input);

        if (input == "exit") {
            break;
        }

        if (is_number(input)) {
            Paddington.train_duration(train_id++) = std::stod(input);
            ++current_row;
        }
        else {
            goto_xy(0, current_row + 1);
            std::cout << blank40;
            goto_xy(0, current_row +1);
        }
 
        if (train_id == trains_count) {
            break;
        }
    }

    current_row += 1;
    goto_xy(0, current_row);
    std::cout << "Enter \"depart\" for train departure.\n";
    std::cout << std::setw(30) << "Arrivals\n";
    current_row += 2;;
    Paddington.row = current_row;
    for (int i = 0; i < trains_count; ++i) {
        std::cout << "Train #" << i << "\n";
    }

    Paddington.start();
    goto_xy(0, current_row + 4);

    return 0;
}