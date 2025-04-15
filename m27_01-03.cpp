#include <iostream>
#include <sstream>
#include <ctime>
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

template <typename C>
void shuffle(C& container) {
        std::random_device rd;
        std::mt19937 g(rd());
        std::shuffle(container.begin(), container.end(), g);
}

template<typename C>
void print(const C& container) {
    for (const auto& value : container) {
        std::cout << value << "\n";
    }
//    std::cout << "\n";
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

    Branch(const std::string& name, const int children_count, Branch* parent = nullptr)
        : name_(name)
        , children_count_(children_count)
        , parent_(parent)
    {
        std::cout << "Ctor " << name_ << "\n";
        children_ = new Branch* [children_count_] {};
        for (int i = 0; i < children_count_; ++i) {
 //           children_[i] = new Branch;
        }
    };

    void add_branch(const std::string name, const int children_count) {
        if (children_init_ < children_count_) {
            children_[children_init_++] = new Branch(name, children_count, this);
        }
    }

    std::string_view get_name() const {
        return name_;
    }

    Branch* get_parent() {
        return parent_;
    }

    Branch** get_children() {
        return children_;
    }

    Branch* get_child(int id) {
        if (id < 0 || id >= children_count_) {
            return nullptr;
        }
        return children_[id];
    }

    ~Branch() {

        for (int i = 0; i < children_count_; ++i) {
  //          std::cout << "destruct child " << i << " "  << children_[i]->get_name() << "\n";
   //         children_[i]->~Branch();
//            delete children_[i];
        }
        delete_branch(this);
        delete[] children_;
        std::cout << "Dtor " << name_ << "\n";
    };

private:
    std::string name_ = "unknown";
    int children_init_ = 0;
    int children_count_ = 0;
    Branch* parent_ = nullptr;
    Branch** children_ = nullptr;

    void delete_branch(Branch* current) {
        if (current == nullptr) {
            return;
        }
        for (int i = 0; i < current->children_count_; ++i) {
            if (current->children_[i] != nullptr) {
                delete_branch(current->children_[i]);
            }
            
        }
//        delete current;
    }
};

int main() {

    {
        const int number_of_trees = 5;
        const int number_of_levels = 3;
        std::srand(std::time(nullptr));
//        std::rand() % (max - min + 1) + min;
        std::vector<std::string> names = {
            "Alfaboo", "Bippity Bop",
            "Candy Cane", "Dingleberry",
            "Elfo", "Fizzgig", "Gingersnap",
            "Hula-Hooper", "Isengrin",
            "Jolly Jingle", "Kookie Kringle",
            "Lollypop", "Mr. Mistletoe",
            "Nutmeg", "Oreo",
            "Peppermint Patti", "Quirky Quince",
            "Roscoe", "Snickerdoodle",
            "Twinkle Toes", "Umpa Lumpa",
            "Vixen", "Whirly Twirly",
            "Xander Sparklebottom",
            "Yoyo"
        };

        shuffle(names);
 //       print(names);

        int level0 = 5;
        int level1 = 3;

        Branch* tree = 
        new Branch{ "tree1", 3 };
        tree->add_branch("branch1", 5);
        tree->add_branch("branch2", 3);
        tree->add_branch("branch3", 2);

        tree->get_child(0)->add_branch("branch11", 0);
        tree->get_child(0)->add_branch("branch12", 0);
        tree->get_child(0)->add_branch("branch13", 0);
        tree->get_child(0)->add_branch("branch14", 0);


        auto p = tree->get_child(0);
        std::cout << p->get_name() << "\n";
        
        std::cout << tree->get_child(0)->get_parent()->get_name() << "\n";

   //     std::cout << tree->get_children()[0]->get_name() << "\n";

        delete tree;
        
        std::cout << p->get_name() << "\n";
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