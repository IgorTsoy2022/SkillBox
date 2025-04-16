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

enum class type {
    trunk = 0,
    big_branch = 1,
    middle_branch = 2,
    leaf = 3
};

class Branch {
public:
    Branch() {};

    Branch(const std::string& name, const int branches_count,
           Branch* parent = nullptr)
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

    void add_resident(const std::string name) {
        if (name.size() > 0) {
            residents_.insert(name);
        }
    }

    std::string_view get_name() const {
        return name_;
    }

    Branch* get_parent() const {
        return parent_;
    }

    Branch** get_branches() const {
        return branches_;
    }

    Branch* get_branch(int id) const {
        if (id < 0 || id >= branches_count_) {
            return nullptr;
        }
        return branches_[id];
    }

    std::set<std::string> get_residents() const {
        return residents_;
    }

    void print_branches(Branch* current) {
        if (current == nullptr) {
            return;
        }

        int residents_count = current->residents_.size();

        if (current->parent_ == nullptr) {
            std::cout << "House on trunk ";
        }
        else if (current->parent_->parent_ == nullptr) {
            std::cout << "House on big branch ";
        }
        else if (current->parent_->parent_->parent_ == nullptr) {
            std::cout << "House on middle branch ";
        }
        else {
            residents_count = -1;
        }

        std::cout << "\"" << current->name_ << "\"\n";

        if (residents_count > 0) {
            if (residents_count == 1) {
                std::cout << "   There is 1 elf living in the house:\n";
            }
            else {
                std::cout << "   There are " << residents_count
                          << " elves living in the house :\n";
            }
            for (const auto& name : current->residents_) {
                std::cout << "      " << name << "\n";
            }
        }
        else if (residents_count == 0) {
            std::cout << "   Empty house.\n";
        }

        for (int i = 0; i < current->branches_count_; ++i) {
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
    std::set<std::string> residents_;

    void delete_branches(Branch* current) {
        if (current == nullptr) {
            return;
        }
        int branches_count = current->branches_count_;
        for (int i = 0; i < branches_count; ++i) {
            if (current->branches_[i] != nullptr) {
                delete_branches(current->branches_[i]);
                if (current->branches_[i]->branches_count_ < 1 ) {
                    --current->branches_[i]->parent_->branches_count_;
                    delete current->branches_[i];
                    current->branches_[i] = nullptr;
                }
            }
        }
    }
};

int main() {

    {
        const int trees_count = 5;
        std::srand(std::time(nullptr));
//        std::rand() % (max - min + 1) + min;

        std::vector<std::string> names = {
"Adceran Biphidrossel",
"Aelara",
"Aelinor",
"Aerendir",
"Aerinwe",
"Aerisil",
"Alandur",
"Alara Maplewood",
"Alariel",
"Alaris",
"Aldariel",
"Alfaboo",
"Aranathor",
"Arannis",
"Arwenya",
"Astra Timberwing",
"Athannon Darknessblight",
"Aylia Seaspray",
"Baldithas Aspengaze",
"Bibella Shadowkind",
"Bippity Bop",
"Branwyn Willowthorn",
"Breana Birchwood",
"Bryn Starfrost",
"Caelaria",
"Caelendir",
"Caladwen",
"Calindra",
"Calithra",
"Candy Cane",
"Celeborn",
"Celestia",
"Cloudweaver",
"Cordelia Maplewood",
"Cullucai Runecaster",
"Dingleberry",
"Draelin Oakenshield",
"Eilsys Stonesmile",
"Elandra",
"Elenara",
"Elendriel",
"Elenweyr",
"Elfo",
"Elindor",
"Elondir",
"Elowen",
"Elquinal Starcrown",
"Enqirelle Dolie",
"Eranos Deathbringer",
"Evaine Cloudjumper",
"Faefina Wooddreamer",
"Faelanor",
"Faelanwe",
"Faelenor",
"Faelindra",
"Falindor",
"Finrodan",
"Fizzgig",
"Galadhel",
"Galadra",
"Galadriel",
"Galanwe",
"Galathia",
"Galathil",
"Garron Weirwatcher",
"Gingersnap",
"Grannok Skullcrusher",
"Grin Willowthorn",
"Gwendolyn Birchwood",
"Hula-Hooper",
"Iarthana Shildrirrish",
"Iselle Stormchaser",
"Isengrin",
"Jolly Jingle",
"Jorani Oakenshield",
"Keelie Oakenshield",
"Kookie Kringle",
"Laurana Willowthorn",
"Lila Silverfrost",
"Lirin Snowbreeze",
"Lollypop",
"Magvyre Larirre",
"Miavyre Vurnoveviash",
"Miri Moonfire",
"Mr. Mistletoe",
"Naren Riverwind",
"Nathalie Darknessblight",
"Nenithra",
"Nenriel",
"Nerigella Fallmane",
"Nirelindil",
"Nutmeg",
"Oakenbreeze",
"Omacan Nusonn",
"Oreo",
"Orgulo Pinebranch",
"Oriana Deathbringer",
"Peppermint Patti",
"Qivalur Flenescath",
"Qivalur Lightflight",
"Qixidor Windbreeze",
"Quirky Quince",
"Raena Willowthorn",
"Ralokas Dawnbrook",
"Ralolamin Drultahe",
"Riverbend",
"Roscoe",
"Rowan Riverwind",
"Saren Seaspray",
"Seacutter",
"Serafina Skullcrusher",
"Silmara",
"Silmariel",
"Silvermoon",
"Snickerdoodle",
"Starburst",
"Sylva Silverfrost",
"Talia Bloodmoon",
"Taren Starfrost",
"Thalara",
"Thalarel",
"Thalindra",
"Thalionwen",
"Thandoria",
"Thanduril",
"Thiriana",
"Thiriandil",
"Thiriandur",
"Thirianna",
"Thornfrade Windrider",
"Twinkle Toes",
"Umpa Lumpa",
"Uthanon Bloodmoon",
"Valandril",
"Valanya",
"Valeria",
"Varen Cloudjumper",
"Vixen",
"Whirly Twirly",
"Willowfire",
"Wrantumal Lunint",
"Wysaphine Galdelenthrash",
"Xander Sparklebottom",
"Xandros Soulreaper",
"Xaviera Soulreaper",
"Yarik Stormchaser",
"Yarilani Maplethorn",
"Yesstina Elderspark",
"Yoyo",
"Zoraida Birchshield",
"Zoren Elmwood",
"Zorrin Cloudjumper",
"Zyren Stormchaser",
"Zyron Starfrost"
        };

        std::sort(names.begin(), names.end());
        std::set<std::string> ordered;
        for (const auto& n : names) {
            ordered.insert(n);
        }

        for (const auto& n : ordered) {
//            std::cout << "\"" << n << "\", \n";
        }

//        return 0;
        shuffle(names);
        auto names_size = names.size();
        std::cout << names_size << "\n";

        Branch** trees = new Branch* [trees_count];

        int name_id = 0;
        for (int id = 0; id < trees_count; ++id) {
            int big_branches_count = std::rand() % (5 - 3 + 1) + 3;
            std::string tree_name = "Tree_" + std::to_string(id + 1);
            trees[id] = new Branch{ tree_name, big_branches_count };

            if (name_id < names_size) {
                trees[id]->add_resident(names[name_id++]);
            }

            for (int i = 0; i < big_branches_count; ++i) {
                int middle_branches_count = std::rand() % (3 - 2 + 1) + 2;
                std::string big_branch_name = 
                    tree_name + " BigBranch_" + std::to_string(i + 1);
                trees[id]->add_branch(big_branch_name, middle_branches_count);

                if (name_id < names_size) {
                    trees[id]->get_branch(i)->add_resident(names[name_id++]);
                }

                for (int j = 0; j < middle_branches_count; ++j) {
                    int little_branches_count = std::rand() % (3 - 0 + 1) + 0;
                    std::string middle_branch_name = 
                        big_branch_name + " MiddleBranch_" + 
                        std::to_string(j + 1);
                    trees[id]->get_branch(i)->add_branch(middle_branch_name,
                        little_branches_count);

                    if (name_id < names_size) {
                        trees[id]->get_branch(i)->get_branch(j)->add_resident(names[name_id++]);
                    }

                    for (int k = 0; k < little_branches_count; ++k) {
                        std::string leaf_name = 
                            middle_branch_name + " Leaf_" +
                            std::to_string(k + 1);
                        trees[id]->get_branch(i)->get_branch(j)->add_branch(
                            leaf_name, 0);
                    }
                }
            }
        }

        std::cout << "\n";

        for (int i = 0; i < trees_count; ++i) {
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