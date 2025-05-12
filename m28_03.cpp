#include <iostream>
#include <deque>
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

// Task 3. Simulation of online restaurant kitchen operation

class Service {
public:
    Service() = default;

    Service(const int max_time, const int min_time = 0)
        : max_time_(max_time)
        , min_time_(min_time)
    {};

    void set_time(const int max_time, const int min_time = 0) {
        max_time_ = max_time;
        min_time_ = min_time;
    }

    const int random_time() const {
        return std::rand() % (max_time_ - min_time_ + 1) + min_time_;
    }

    const int process_time() const {
        return max_time_;
    }

    ~Service() {
//        std::cout << "~Service\n";
    };

private:
    int max_time_ = 0;
    int min_time_ = 0;
};

class Order {
public:
    Order() = delete;

    Order(const int id, const int course_id)
        : id_(id)
        , course_id_(course_id)
    {};

    const int id() const {
        return id_;
    }

    const int course_id() const {
        return course_id_;
    }

    ~Order() {
//        std::cout << "~Order " << id_ << "\n";
    };

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

    void set_waiter(const int max_time, const int min_time) {
        if (waiter_ == nullptr) {
            waiter_ = new Service(max_time, min_time);
        }
        else {
            waiter_->set_time(max_time, min_time);
        }
    }

    void set_kitchen(const int max_time, const int min_time) {
        if (kitchen_ == nullptr) {
            kitchen_ = new Service(max_time, min_time);
        }
        else {
            kitchen_->set_time(max_time, min_time);
        }
    }

    void set_courier(const int max_time) {
        if (courier_ == nullptr) {
            courier_ = new Service(max_time);
        }
        else {
            courier_->set_time(max_time);
        }
    }

    void start(const int row) {
        std::thread waiter_thread(&Bistro::getting_order, this, row);
        std::thread kitchen_thread(&Bistro::preparing_order, this, row + 1);
        std::thread courier_thread(&Bistro::picking_up_order, this, row + 2);

//        waiter_thread.detach();
//        kitchen_thread.detach();
//        courier_thread.detach();

        if (waiter_thread.joinable()) {
            waiter_thread.join();
        }
        if (kitchen_thread.joinable()) {
            kitchen_thread.join();
        }
        if (courier_thread.joinable()) {
            courier_thread.join();
        }
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

    ~Bistro() {
//        std::cout << "~Bistro\n";
        delete waiter_;
        delete kitchen_;
        delete courier_;

        for (const auto& order : orders_) {
            delete order;
        }

        for (const auto& order : ready_orders_) {
            delete order;
        }
    };
private:
    std::vector<std::string> menu_;
    std::deque<Order*> orders_;
    std::deque<Order*> ready_orders_;
    int daily_throughput_ = 0;
    int meter_ = 0;
    Service* waiter_ = nullptr;
    Service* kitchen_ = nullptr;
    Service* courier_ = nullptr;
    
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

    void getting_order(const int row) {
        int count_orders = 0;
        std::string blank(10, ' ');

        mtx.lock();
        goto_xy(0, row);
        std::cout << "Waiter: waiting for the order.";
        mtx.unlock();

        while (true) {
            int time = waiter_->random_time();
            std::this_thread::sleep_for(std::chrono::seconds(time));

            mtx.lock();

            Order* order = new Order(++meter_, std::rand() % menu_.size());
            orders_.emplace_back(order);
            ++count_orders;

            goto_xy(0, row);
            std::cout << "Waiter: took the order #" << order->id() << " "
            << course(order->course_id()) << "." << blank;

            mtx.unlock();

            if (count_orders >= daily_throughput_) {
                return;
            }
        }
    }

    void preparing_order(const int row) {
        int count_orders = 0;
        std::string blank(10, ' ');
        Order* current_order = nullptr;

        mtx.lock();
        goto_xy(0, row);
        std::cout << "Kitchen: waiting for the order." << blank;
        mtx.unlock();

        while (true) {
 
            while (true) {
                std::this_thread::sleep_for(std::chrono::seconds(1));

                mtx.lock();
                if (orders_.size() > 0) {
                    current_order = orders_.front();
                }
                else {
                    current_order = nullptr;
                }
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
                      << " " << course(current_order->course_id()) << "." << blank;
            mtx.unlock();

            int time = kitchen_->random_time();

            std::this_thread::sleep_for(std::chrono::seconds(time));

            mtx.lock();
            auto order = move_to_ready();
            ++count_orders;

            goto_xy(0, row);
            std::cout << "Kitchen: the order #" << order->id() << " "
                      << course(order->course_id()) << " is ready." << blank;
            mtx.unlock();

            if (count_orders >= daily_throughput_) {
                return;
            }
        }
    }

    void picking_up_order(const int row) {
        int count_orders = 0;
        int current_time = 0;
        std::string blank(10, ' ');
        int time = courier_->process_time();
        Order* current_ready_order = nullptr;

        while (true) {

            std::this_thread::sleep_for(std::chrono::seconds(1));

            mtx.lock();
            if (ready_orders_.size() > 0) {
                current_ready_order = ready_orders_.front();
            }
            else {
                current_ready_order = nullptr;
            }
            mtx.unlock();

            if (current_ready_order == nullptr) {
                mtx.lock();
                goto_xy(0, row);
                if (current_time < time) {
                    std::cout << "There are no ready orders and no couriers." << blank;
                }
                else {
                    std::cout << "Courier: waiting for the order to be ready for shipment.";
                }
                mtx.unlock();

                ++current_time;
            }
            else {
                if (current_time < time) {
                    mtx.lock();
                    goto_xy(0, row);
                    std::cout << "There's an order ready to go. Waiting for courier" << blank;
                    mtx.unlock();

                    std::this_thread::sleep_for(std::chrono::seconds(time - current_time));
                }

                mtx.lock();
                goto_xy(0, row);
                std::cout << "Courier: took the order #" << current_ready_order->id()
                          << " " << course(current_ready_order->course_id()) << "."
                          << blank << blank;
                remove_ready_order();
                mtx.unlock();

                current_time = 0;
                ++count_orders;
            }

            if (count_orders >= daily_throughput_) {
                return;
            }
        }
    }
};

int main() {
    std::cout << "Task 3. Simulation of online restaurant kitchen op.\n";

    std::srand(std::time(nullptr));

    int daily_throughput = 10;
    std::vector<std::string> menu = {
        "pizza", "soup", "steak", "salad", "sushi"
    };

    Bistro bistro(std::move(menu), daily_throughput);

    bistro.set_waiter(10, 5);
    bistro.set_kitchen(15, 5);
    bistro.set_courier(30);

    int row = 3;
    bistro.start(row);

    return 0;
}