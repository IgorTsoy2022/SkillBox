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

    Branch(const std::string& name, const int branches_count, Branch* parent = nullptr)
        : name_(name)
        , branches_count_(branches_count)
        , parent_(parent)
    {
        std::cout << "Ctor " << name_ << "\n";
        if (branches_count_ > 0) {
            branches_ = new Branch * [branches_count_] {};
        }
    };

    void add_branch(const std::string name, const int branches_count) {
        if (branches_init_ < branches_count_) {
            branches_[branches_init_++] = new Branch(name, branches_count, this);
        }
    }

    std::string_view get_name() const {
        return name_;
    }

    Branch* get_parent() {
        return parent_;
    }

    Branch** get_branches() {
        return branches_;
    }

    Branch* get_branch(int id) {
        if (id < 0 || id >= branches_count_) {
            return nullptr;
        }
        return branches_[id];
    }

    void print_branches(Branch* current) {
        if (current == nullptr) {
            std::cout << "RETURN\n";
            return;
        }
        for (int i = 0; i < current->branches_count_; ++i) {
            if (current->branches_[i] != nullptr) {
                std::cout << current->branches_[i]->branches_count_ << " " << current->branches_[i]->name_ << "\n";
                
            }
            print_branches(current->branches_[i]);
        }

    }

    ~Branch() {

        delete_branches(this);
        delete[] branches_;
        std::cout << "Dtor " << name_ << "\n";
    };

private:
    std::string name_ = "unknown";
    int branches_init_ = 0;
    int branches_count_ = 0;
    Branch* parent_ = nullptr;
    Branch** branches_ = nullptr;

    void delete_branches(Branch* current) {
        if (current == nullptr) {
            std::cout << "NULLPTR\n";
            return;
        }
        int branches_count = current->branches_count_;
        for (int i = 0; i < branches_count; ++i) {
            if (current->branches_[i] != nullptr) {
//                std::cout << current->branches_[i]->branches_count_ << " " << current->branches_[i]->name_ << "\n";
                delete_branches(current->branches_[i]);
                if (current->branches_[i]->branches_count_ == 0) {
//                    std::cout << "Deleting " << " " << current->branches_[i]->name_ << "\n";
                    delete current->branches_[i];
                    current->branches_[i] = nullptr;
                }
            }
            if (i == branches_count - 1 && current != nullptr && current->parent_ != nullptr) {
                std::cout << "!!!Deleting " << " " << current->name_ << "\n";
 //               delete current;
            }

        }
//        std::cout << "current " << current->branches_count_ << " " << current->name_ << "\n";
        if (current->branches_count_ == 0) {
//            std::cout << "!!!!!!!!Deleting " << " " << current->name_ << "\n";
//            delete current;
        }
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

        Branch** trees = new Branch* [number_of_trees];

        for (int id = 0; id < number_of_trees; ++id) {
            int number_of_big_branches = std::rand() % (5 - 3 + 1) + 3;
            std::string tree_name = "Tree_" + std::to_string(id + 1);
            trees[id] = new Branch{ tree_name, number_of_big_branches };
            for (int i = 0; i < number_of_big_branches; ++i) {
                int number_of_middle_branches = std::rand() % (3 - 2 + 1) + 2;
                std::string big_branch_name = tree_name + " BigBranch_" + std::to_string(i + 1);
                trees[id]->add_branch(big_branch_name, number_of_middle_branches);
                for (int j = 0; j < number_of_middle_branches; ++j) {
                    int number_of_little_branches = 3;
                    std::string middle_branch_name = big_branch_name + " MiddleBranch_" + std::to_string(j + 1);
                    trees[id]->get_branch(i)->add_branch(middle_branch_name, number_of_little_branches);
                    for (int k = 0; k < number_of_little_branches; ++k) {
                        std::string little_branch_name = middle_branch_name + " LittleBranch_" + std::to_string(k + 1);
                        trees[id]->get_branch(i)->get_branch(j)->add_branch(little_branch_name, 0);
                    }
                }
            }
        }
        
        delete trees[0];
//        delete trees[1];
//        delete trees[2];
//        delete trees[3];
//        delete trees[4];

        for (int i = 0; i < number_of_trees; ++i) {
            trees[i]->print_branches(trees[i]);
        }
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