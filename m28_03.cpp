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

// Task 3. Simulation of online restaurant kitchen operation

class Bistro;
class Waiter;
static void getting_order(Bistro* bistro, Waiter* waiter, const int row);

class Order {
public:
    Order() = delete;

    Order(Bistro* bistro, const int id, const int course_id)
        : bistro(bistro)
        , id_(id)
        , course_id_(course_id)
    {
    };

    const int id() const {
        return id_;
    }

    const int course_id() const {
        return course_id_;
    }

    const std::string_view course() const;

    ~Order() {};

protected:
    Bistro* bistro = nullptr;

private:
    int id_ = 0;
    int course_id_ = 0;
};

class Bistro {
public:
    Bistro() = delete;

    Bistro(std::vector<std::string>&& menu, const int daily_throughput)
        : menu_(std::move(menu))
        , daily_throughput_(daily_throughput)
    {};

    const int daily_throughput() const {
        return daily_throughput_;
    }

    int meter = 0;

    Order* current_order() {
        if (orders_.empty()) {
            return nullptr;
        }
        return orders_.front();
    }

    Order* current_ready_order() {
        if (ready_orders_.empty()) {
            return nullptr;
        }
        return ready_orders_.front();
    }

    void take_order(Order* order) {
        orders_.emplace_back(order);
    }

    Order* move_to_ready() {
        if (orders_.empty()) {
            return nullptr;
        }

        ready_orders_.emplace_back(orders_.front());
        orders_.pop_front();
        return ready_orders_.back();
    }

    void remove_ready_order() {
        if (ready_orders_.empty()) {
            return;
        }

        delete ready_orders_.front();
        ready_orders_.pop_front();
    }

    const int menu_size() const {
        return menu_.size();
    }

    const std::string_view course(int id) const {
        if (menu_.size() < 1) {
            return "";
        }
        if (id >= 0 && id < menu_.size()) {
            return menu_[id];
        }
        return "";
    }

    void menu() const {
        int id = 0;
        for (const auto& course : menu_) {
            std::cout << id++ << " " << course << "\n";
        }
    }

    ~Bistro() {};
private:
    std::vector<std::string> menu_;
    std::deque<Order*> orders_;
    std::deque<Order*> ready_orders_;
    int daily_throughput_ = 0;
};

const std::string_view Order::course() const {
    return bistro->course(course_id_);
}


class Service {
public:
    Service() = delete;

    Service(Bistro* bistro, const int max_time, const int min_time)
        : bistro(bistro)
        , max_time_(max_time)
        , min_time_(min_time)
    {};

    const int daily_throughput() {
        return bistro->daily_throughput();
    }

    const int random_time() const {
        return std::rand() % (max_time_ - min_time_ + 1) - min_time_;
    }

    const int process_time() const {
        return max_time_;
    }

    virtual ~Service() {};

protected:
    Bistro* bistro;

private:
    int max_time_ = 0;
    int min_time_ = 0;
};

class Waiter : public Service {
public:
    Waiter() = delete;

    Waiter(Bistro* bistro, const int max_time, const int min_time)
        : Service(bistro, max_time, min_time)
    {};

    Order* take_order() {
        Order* order = new Order(bistro, ++bistro->meter,
                                 std::rand() % (bistro->menu_size()));
        bistro->take_order(order);
        return order;
    }

    ~Waiter() {};
};

class Kitchen : public Service {
public:
    Kitchen() = delete;

    Kitchen(Bistro* bistro, const int max_time, const int min_time)
        : Service(bistro, max_time, min_time)
    {};

    Order* current_order() {
        return bistro->current_order();
    }

    Order* move_to_ready() {
        return bistro->move_to_ready();
    }

    ~Kitchen() {};
};

class Courier : public Service {
public:
    Courier() = delete;

    Courier(Bistro* bistro, const int max_time)
        : Service(bistro, max_time, max_time)
    {};

    Order* current_ready_order() {
        return bistro->current_ready_order();
    }

    void remove_ready_order() {
        bistro->remove_ready_order();
    }

    ~Courier() {};
};

static void getting_order(Bistro* bistro, Waiter* waiter, const int row) {
    int count_orders = 0;
    int daily_throughput = bistro->daily_throughput();

    mtx.lock();

    goto_xy(0, row);
    std::cout << "Waiter: waiting for the order.\n";

    mtx.unlock();

    while (true) {
        int time = waiter->random_time();

        std::this_thread::sleep_for(std::chrono::seconds(time));

        mtx.lock();

        auto order = waiter->take_order();
        ++count_orders;

        goto_xy(0, row);
        std::cout << "Waiter: took the order #" << order->id() << " "
                  << order->course() << "\n";

        mtx.unlock();

        if (count_orders >= daily_throughput) {
            return;
        }
    }
}

static void preparing_order(Kitchen* kitchen, const int row) {
    int count_orders = 0;
    int daily_throughput = kitchen->daily_throughput();
    Order* current_order = nullptr;

    while (true) {

        mtx.lock();
        goto_xy(0, row);
        std::cout << "Kitchen: waiting for the order.\n";
        mtx.unlock();

        while (true) {

            std::this_thread::sleep_for(std::chrono::seconds(1));

            mtx.lock();
            current_order = kitchen->current_order();
//            goto_xy(0, row);
//            std::cout << "Kitchen: waiting for the order.\n";
            mtx.unlock();

            if (current_order != nullptr) {
                break;
            }
        }

        mtx.lock();
        goto_xy(0, row);
        std::cout << "Kitchen: preparing the order #" << current_order->id()
                  << " " << current_order->course() << ".\n";
        mtx.unlock();

        int time = kitchen->random_time();
        std::this_thread::sleep_for(std::chrono::seconds(time));

        mtx.lock();
        auto order = kitchen->move_to_ready();
        ++count_orders;
        goto_xy(0, row);
        std::cout << "Kitchen: the order #" << order->id() << " "
                  << order->course() << " is ready.\n";
        mtx.unlock();

        if (count_orders >= daily_throughput) {
            return;
        }
    }
}

static void picking_up_order(Courier* courier, const int row) {
    int count_orders = 0;
    int current_time = 0;
    int daily_throughput = courier->daily_throughput();
    int time = courier->process_time();
    Order* current_ready_order = nullptr;

    while (true) {

        std::this_thread::sleep_for(std::chrono::seconds(1));

        mtx.lock();
        current_ready_order = courier->current_ready_order();
        mtx.unlock();

        if (current_ready_order == nullptr) {
            mtx.lock();
            goto_xy(0, row);
            if (current_time < time) {
                std::cout << "There are no ready orders and no couriers.\n";
            }
            else {
                std::cout << "Courier: waiting for the order to be ready for shipment.\n";
            }
            mtx.unlock();

            ++current_time;
        }
        else {
            if (current_time < time) {
                mtx.lock();
                goto_xy(0, row);
                std::cout << "There's an order ready to go. Waiting for courier.\n";
                mtx.unlock();

                std::this_thread::sleep_for(std::chrono::seconds(time - current_time));
            }

            mtx.lock();
            goto_xy(0, row);
            std::cout << "Courier: took the order #" << current_ready_order->id()
                      << " " << current_ready_order->course() << ".\n";
            courier->remove_ready_order();
            mtx.unlock();

            current_time = 0;
            ++count_orders;
        }

        if (count_orders >= daily_throughput) {
            return;
        }
    }
}

int main() {
    std::cout << "Task 3. Simulation of online restaurant kitchen op.\n";

    std::srand(std::time(nullptr));

    int daily_throughput = 10;
    std::vector<std::string> menu = {
        "pizza", "soup", "steak", "salad", "sushi"
    };
        
    Bistro bistro(std::move(menu), daily_throughput);
    bistro.menu();

    Waiter waiter(&bistro, 5, 10);
    Kitchen kitchen(&bistro, 5, 15);
    Courier courier(&bistro, 30);

    int row = 3;
//    std::thread waiter_thread = std::thread(getting_order, std::ref(bistro), std::ref(waiter), std::ref(row));
    /*
    std::thread waiter_thread(getting_order, waiter, 3);
    waiter_thread.detach();
    std::thread kitchen_thread(preparing_order, kitchen, 4);
    kitchen_thread.detach();
    std::thread courier_thread(picking_up_order, courier, 5);
    courier_thread.detach();

    if (waiter_thread.joinable()) {
        waiter_thread.join();
    }
    if (kitchen_thread.joinable()) {
        kitchen_thread.join();
    }
    if (courier_thread.joinable()) {
        courier_thread.join();
    }
    */

    return 0;
}