#include <iostream>
#include <sstream>
#include <random>
#include <map>
#include <set>
#include <string>
#include <string_view>
#include <vector>

bool is_number(const std::string str) {
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


// Task 1. Hierarchy of geometric shapes

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


// Task 2. Company simulation.

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


// Task 3. Elf Village.

class Branch {
public:
    Branch() {};

    Branch(const int number, const int children_count, Branch* parent = nullptr)
        : number_(number)
        , children_count_(children_count)
        , parent_(parent)
    {
        std::cout << "Ctor " << number_ << "\n";
        children_ = new Branch* [children_count_] {};
        for (int i = 0; i < children_count_; ++i) {
            children_[i] = new Branch{};
        }
    };

    Branch* get_children() {
        return *children_;
    }

    Branch* get_child(int id) {
        if (id < 0 || id >= children_count_) {
            return nullptr;
        }
        return children_[id];
    }

    ~Branch() {
        std::cout << "Dtor " << number_ << "\n";
        for (int i = 0; i < children_count_; ++i) {
            delete[] children_[i];
            std::cout << "destruct child " << i << "\n";
        }
        delete[] children_;
        if (parent_ != nullptr) {
            for (int i = 0; i < parent_->children_count_; ++i) {
                if (this == parent_->children_[i]) {
                    parent_->children_[i] = nullptr;
                    std::cout << "selfdestruct from parent.\n";
                }
            }
        }
    };

private:
    int number_ = 0;
    int children_count_ = 0;
    Branch* parent_ = nullptr;
    Branch** children_ = nullptr;
};

int main() {

    {
        Branch* tree = new Branch{ 1, 3 };

        delete tree->get_child(2);
//        delete tree;
    }

    return 0;

    {
 //       std::cout << "Task 1. Hierarchy of geometric shapes.\n";
    }

    {
//        std::cout << "\nTask 2. Company simulation.\n";
    }

    {
        std::cout << "Task 3. Elf Village.\n";
        std::cout << "Commands for window:\n";
        std::cout << "\"window w h\" - Make window, where w-width, h-height are optional.\n";
        std::cout << "\"move dx dy\" - Move the window, where dx and dy are x-axis and y-axis\n";
        std::cout << "               offset and are optional.\n";
        std::cout << "\"resize w h\" - Resize the window, where w-width, h-height are optional.\n";
        std::cout << "\"display\"    - Display the window.\n";
        std::cout << "\"hide\"       - Hide the window.\n";
        std::cout << "\"screen\"     - Display the screen.\n";
        std::cout << "\"close\"      - Close the window and exit.\n";

        std::string input = "";
        while (true) {
            std::string str1 = "";
            std::string str2 = "";
            std::stringstream input_stream(input);
            input_stream >> input >> str1 >> str2;

            if (input == "close") {
                break;
            }


            if (input == "move") {
                int dx = 0, dy = 0;
                if (str1.empty() || str2.empty()) {
                    std::cout << "Enter the x-axis and y-axis offset: > ";
                    std::getline(std::cin, input);
                    std::stringstream input_stream(input);
                    input_stream >> str1 >> str2;
                }
                if (is_number(str1) && is_number(str2)) {
                    dx = std::stod(str1);
                    dy = std::stod(str2);
                }
                else {
                    std::cout << "Incorrect offset values have been entered."
                        << "Press \"Enter\" to continue.";
                    std::cin.get();
                }
            }

            if (input == "resize") {
                int width = 0, height = 0;
                if (str1.empty() || str2.empty()) {
                    std::cout << "Enter the dimensions of the window (width, height): > ";
                    std::getline(std::cin, input);
                    std::stringstream input_stream(input);
                    input_stream >> str1 >> str2;
                }
                if (is_number(str1)) {
                    width = std::stod(str1);
                }
                if (is_number(str2)) {
                    height = std::stod(str2);
                }
                if (width > 0 && height > 0) {
                }
                else {
                    std::cout << "Incorrect window size values have been entered."
                        << "Press \"Enter\" to continue.\n";
                    std::cin.get();
                }
            }

            if (input == "screen") {
            }

            std::cout << "Command > ";
            std::getline(std::cin, input);
        }
    }

    return 0;
}