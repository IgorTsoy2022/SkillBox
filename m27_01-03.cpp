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

// Task 1. Hierarchy of geometric shapes

// Task 2. Company simulation.


// Task 3. Elf Village.

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

    void evict_resident(const std::string name) {
        if (name.size() > 0) {
            residents_.erase(name);
        }
    }

    const std::string_view get_name() const {
        return name_;
    }

    const Branch* get_parent() const {
        return parent_;
    }

    const int get_branches_count() const {
        return branches_count_;
    }

    Branch** get_branches() const {
        return branches_;
    }

    Branch* get_branch(const int id) const {
        if (id < 0 || id >= branches_count_) {
            return nullptr;
        }
        return branches_[id];
    }

    Branch* find_branch(const std::string branch_name,
                        Branch* current) const {
        if (branch_name.size() == 0 || current == nullptr) {
            return nullptr;
        }

        if (toupper(current->name_) == toupper(branch_name)) {
            return current;
        }

        for (int i = 0; i < current->branches_count_; ++i) {
            auto result = find_branch(branch_name, current->branches_[i]);
            if (result != nullptr) {
                return result;
            }
        }
        return nullptr;
    }

    const std::set<std::string> get_residents() const {
        return residents_;
    }

    Branch* find_resident(const std::string name, Branch* current) const {
        if (name.size() == 0 || current == nullptr) {
            return nullptr;
        }

        for (const auto& resident : current->residents_) {
            if (toupper(resident) == toupper(name)) {
                return current;
            }
        }

        for (int i = 0; i < current->branches_count_; ++i) {
            auto result = find_resident(name, current->branches_[i]);
            if (result != nullptr) {
                return result;
            }
        }
        return nullptr;
    }

    void print_branches(const Branch* current) const {
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

void find_elf(const std::string& name, Branch** trees) {
    bool found = false;
    for (int i = 0; i < 5; ++i) {
        auto branch = trees[i]->find_resident(name, trees[i]);

        found = (branch != nullptr);
        if (found) {
            int house_residents = branch->get_residents().size();
            int branch_residents = house_residents;
            int branch_houses = 1;
            std::cout << name << " lives in the house \""
                      << branch->get_name() << "\"\n";

            if (house_residents == 2) {
                std::cout << "with ";
                for (const auto& resident : branch->get_residents()) {
                    if (toupper(resident) == toupper(name)) {
                        continue;
                    }
                    std::cout << resident << ".\n";
                }
            }
            else if (house_residents > 2) {
                std::cout << "with the following residents:\n";
                for (const auto& resident : branch->get_residents()) {
                    if (toupper(resident) == toupper(name)) {
                        continue;
                    }
                    std::cout << "    " << resident << "\n";
                }
            }

            if (branch->get_parent() != nullptr) {
                branch_houses = branch->get_parent()->get_branches_count();
                std::cout << name << "'s neighbors:\n";
                for (int i = 0; i < branch_houses; ++i) {
                    if (branch->get_parent()->get_branches()[i] == branch) {
                        continue;
                    }
                    int residents_count = branch->get_parent()
                        ->get_branches()[i]->get_residents().size();
                    branch_residents += residents_count;
                    std::cout << "    The house \""
                        << branch->get_parent()->get_branches()[i]->get_name()
                        << "\"";
                    if (residents_count > 0) {
                        std::cout << " with " << residents_count
                                  << " resident(s):\n";
                        for (const auto& resident :
                            branch->get_parent()->get_branches()[i]
                            ->get_residents()) {
                            std::cout << "        " << resident << "\n";
                        }
                    }
                    else {
                        std::cout << " is empty.\n";
                    }
                }
            }

            if (house_residents > 1) {
                std::cout << "There are " << house_residents
                    << " residents live";
            }
            else {
                std::cout << "There is " << house_residents
                    << " resident lives";
            }
            std::cout << " in the house \"" << branch->get_name() << "\".\n";

            if (branch_houses > 1) {
                std::cout << "There are " << branch_houses << " houses and ";
            }
            else {
                std::cout << "There is " << branch_houses << " house and ";
            }
            std::cout << branch_residents;

            if (branch_residents > 1) {
                std::cout << " residents live";
            }
            else {
                std::cout << " resident lives";
            }
            if (branch->get_parent() != nullptr) {
                std::cout << " in the branch \""
                    << branch->get_parent()->get_name() << "\".\n";
            }
            else {
                std::cout << " in the trunc \""
                    << branch->get_name() << "\".\n";
            }

            break;
        }
    }
    if (!found) {
        std::cout << name << " not found in the village.\n";
    }
}

bool settle_elf(const std::string& name, const std::string& house,
                Branch** trees) {
    for (int i = 0; i < 5; ++i) {
        auto branch = trees[i]->find_branch(house, trees[i]);
        if (branch != nullptr) {
            branch->add_resident(name);
            return true;
        }
    }
    return false;
}

bool evict_elf(const std::string& name, Branch** trees) {
    bool found = false;
    for (int i = 0; i < 5; ++i) {
        auto branch = trees[i]->find_resident(name, trees[i]);
        if (branch != nullptr) {
            branch->evict_resident(name);
            return true;;
        }
    }
    return false;
}




enum class TASK {
    IDLE = 0,
    A = 1,
    B = 2,
    C = 3
};

class HumanNature {
public:
    HumanNature() {};

    HumanNature(const std::string& name)
        : name_(name)
    {};

    const std::string_view get_name() const {
        return name_;
    }

    void set_name(const std::string& name) {
        name_ = name;
    }

    ~HumanNature() {
        std::cout << "Dtor HumanNature: " << name_ << "\n";
    };

private:
    std::string name_ = "unknown";
};

class Employee : public HumanNature {
public:
    Employee(const std::string& name, const std::string& department)
        : HumanNature(name)
        , department_(department)
    {};

    const std::string_view get_name() const {
        return HumanNature::get_name();
    }

    const std::string_view get_department() const {
        return department_;
    }

    const bool is_busy() const {
        return task_ != TASK::IDLE;
    }

    const std::string_view get_taskname() const {
        switch (task_) {
        case TASK::A:
            return "A";
        case TASK::B:
            return "B";
        case TASK::C:
            return "C";
        default:
            return "IDLE";
        }
    }

    void set_task(const TASK task) {
        task_ = task;
    }

    ~Employee() {
        std::cout << "Dtor Employee: department = " << department_
                  << ", name = " << get_name() << "\n";
    };

private:
    std::string department_ = "unknown";
    TASK task_ = TASK::IDLE;
};

class Manager : public Employee {
public:
    Manager(const std::string& name, const std::string& department)
        : Employee(name, department)
    {};

    const std::string_view get_name() const {
        return HumanNature::get_name();
    }

    void team_up(Employee*& employee) {
        crew_.insert(employee);
        employee = nullptr;
    }

    void team_up(const std::string& name) {
        Employee* employee = new Employee(name, std::string(this->get_department()));
        crew_.insert(employee);
    }

    Employee* fire(const std::string_view name) {
        for (const auto ptr : crew_) {
            if (toupper(std::string(ptr->get_name())) ==
                toupper(std::string(name))) {
                crew_.erase(ptr);
                return ptr;
            }
        }
        return nullptr;
    }

    const int crew_size() const {
        return crew_.size();
    }

    const std::set<Employee*> get_crew() const {
        return crew_;
    }

    ~Manager() {
        std::cout << "Dtor Manager: department = " << Employee::get_department()
                  << ", name = " << get_name() << "\n";
    };

private:
    std::set<Employee*> crew_;
};


int main() {

    {
        const int trees_count = 5;
        std::srand(std::time(nullptr));
//        std::rand() % (max - min + 1) + min;

        Employee* emp = new Employee{ "Tom", "department#1" };
            std::cout << "emp before " << emp->get_name() << "\n";
        Manager* m = new Manager{ "Igor", "department#1" };
        m->team_up(emp);
            if (emp!=nullptr) std::cout << "emp after " << emp->get_name() << "\n";

        for (const auto& c : m->get_crew()) {
            std::cout << "emp in crew " << c->get_name() << "\n";
        }

        auto fired = m->fire("Tom");
        if (fired != nullptr) std::cout << "fired = " << fired->get_name() << "\n";
        for (const auto& c : m->get_crew()) {
            std::cout << "emp in crew " << c->get_name() << "\n";
        }


        delete m;

        delete fired;
        delete emp;

        return 0;
    }

    {
 //       std::cout << "Task 1. Hierarchy of geometric shapes.\n";
    }

    {
        std::cout << "\nTask 2. Company simulation.\n";
        std::cout << "Commands:\n";
        std::cout << "\"find\"  - Find a resident.\n";
        std::cout << "\"add\"   - Settle a resident.\n";
        std::cout << "\"evict\" - Evict the resident.\n";
        std::cout << "\"print\" - Display the trees.\n";
        std::cout << "\"exit\"  - Exit.\n";

        std::string input = "";
        std::string str1 = "";
        std::string str2 = "";
        while (true) {

            if (input == "exit") {
                break;
            }


            std::cout << "Command > ";
            std::getline(std::cin, input);
        }

        return 0;
    }

    {
        std::cout << "Task 3. Elf Village.\n";
        const int trees_count = 5;
        std::srand(std::time(nullptr));
        //        std::rand() % (max - min + 1) + min;

        std::vector<std::string> names = {
    "Adceran Biphidrossel", "Aelara", "Aelinor", "Aerendir", "Aerinwe",
    "Aerisil", "Alandur", "Alara Maplewood", "Alariel", "Alaris",
    "Aldariel", "Alfaboo", "Aranathor", "Arannis", "Arwenya",
    "Astra Timberwing", "Athannon Darknessblight", "Aylia Seaspray",
    "Baldithas Aspengaze", "Bibella Shadowkind", "Bippity Bop",
    "Branwyn Willowthorn", "Breana Birchwood", "Bryn Starfrost",
    "Caelaria", "Caelendir", "Caladwen", "Calindra", "Calithra",
    "Candy Cane", "Celeborn", "Celestia", "Cloudweaver",
    "Cordelia Maplewood", "Cullucai Runecaster",
    "Dingleberry", "Draelin Oakenshield",
    "Eilsys Stonesmile", "Elandra", "Elenara", "Elendriel", "Elenweyr",
    "Elfo", "Elindor", "Elondir", "Elowen", "Elquinal Starcrown",
    "Enqirelle Dolie", "Eranos Deathbringer", "Evaine Cloudjumper",
    "Faefina Wooddreamer", "Faelanor", "Faelanwe", "Faelenor",
    "Faelindra", "Falindor", "Finrodan", "Fizzgig",
    "Galadhel", "Galadra", "Galadriel", "Galanwe", "Galathia",
    "Galathil", "Garron Weirwatcher", "Gingersnap",
    "Grannok Skullcrusher", "Grin Willowthorn", "Gwendolyn Birchwood",
    "Hula-Hooper",
    "Iarthana Shildrirrish", "Iselle Stormchaser", "Isengrin",
    "Jolly Jingle", "Jorani Oakenshield", "Keelie Oakenshield",
    "Kookie Kringle", "Laurana Willowthorn", "Lila Silverfrost",
    "Lirin Snowbreeze", "Lollypop", "Magvyre Larirre",
    "Miavyre Vurnoveviash", "Miri Moonfire", "Mr. Mistletoe",
    "Naren Riverwind", "Nathalie Darknessblight", "Nenithra",
    "Nenriel", "Nerigella Fallmane", "Nirelindil", "Nutmeg",
    "Oakenbreeze", "Omacan Nusonn", "Oreo", "Orgulo Pinebranch",
    "Oriana Deathbringer", "Peppermint Patti",
    "Qivalur Flenescath", "Qixidor Windbreeze", "Quirky Quince",
    "Raena Willowthorn", "Ralokas Dawnbrook", "Ralolamin Drultahe",
    "Riverbend", "Roscoe", "Rowan Riverwind",
    "Saren Seaspray", "Seacutter", "Serafina Skullcrusher", "Silmara",
    "Silmariel", "Silvermoon", "Snickerdoodle", "Starburst",
    "Sylva Silverfrost", "Talia Bloodmoon", "Taren Starfrost",
    "Thalara", "Thalarel", "Thalindra", "Thalionwen", "Thandoria",
    "Thanduril", "Thiriana", "Thiriandil", "Thiriandur", "Thirianna",
    "Thornfrade Windrider", "Twinkle Toes",
    "Umpa Lumpa", "Uthanon Bloodmoon",
    "Valandril", "Valanya", "Valeria", "Varen Cloudjumper", "Vixen",
    "Whirly Twirly", "Willowfire", "Wrantumal Lunint",
    "Wysaphine Galdelenthrash",
    "Xander Sparklebottom", "Xandros Soulreaper", "Xaviera Soulreaper",
    "Yarik Stormchaser", "Yarilani Maplethorn", "Yesstina Elderspark",
    "Yoyo",
    "Zoraida Birchshield", "Zoren Elmwood", "Zorrin Cloudjumper",
    "Zyren Stormchaser", "Zyron Starfrost"
        };

        shuffle(names);
        Branch** trees = new Branch * [trees_count];

        int names_size = names.size();
        int name_id = 0;
        for (int id = 0; id < trees_count; ++id) {
            int big_branches_count = std::rand() % (5 - 3 + 1) + 3;
            std::string tree_name = "Tree_" + std::to_string(id + 1);
            trees[id] = new Branch{ tree_name, big_branches_count };

            if (name_id < names_size) {
                int residents_count =
                    name_id + std::rand() % (5 - 0 + 1) + 0;
                if (residents_count > names_size) {
                    residents_count = names_size;
                }
                for (; name_id < residents_count; ++name_id) {
                    trees[id]->add_resident(names[name_id]);
                }
            }

            for (int i = 0; i < big_branches_count; ++i) {
                int middle_branches_count = std::rand() % (3 - 2 + 1) + 2;
                std::string big_branch_name =
                    tree_name + " BigBranch_" + std::to_string(i + 1);
                trees[id]->add_branch(big_branch_name, middle_branches_count);

                if (name_id < names_size) {
                    int residents_count =
                        name_id + std::rand() % (5 - 0 + 1) + 0;
                    if (residents_count > names_size) {
                        residents_count = names_size;
                    }
                    for (; name_id < residents_count; ++name_id) {
                        trees[id]->get_branch(i)->add_resident(names[name_id]);
                    }
                }

                for (int j = 0; j < middle_branches_count; ++j) {
                    int little_branches_count = std::rand() % (5 - 0 + 1) + 0;
                    std::string middle_branch_name =
                        big_branch_name + " MiddleBranch_" +
                        std::to_string(j + 1);
                    trees[id]->get_branch(i)->add_branch(middle_branch_name,
                        little_branches_count);

                    if (name_id < names_size) {
                        int residents_count =
                            name_id + std::rand() % (5 - 0 + 1) + 0;
                        if (residents_count > names_size) {
                            residents_count = names_size;
                        }

                        for (; name_id < residents_count; ++name_id) {
                            trees[id]->get_branch(i)->get_branch(j)
                                ->add_resident(names[name_id]);
                        }
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

        std::cout << "Commands:\n";
        std::cout << "\"find\"  - Find a resident.\n";
        std::cout << "\"add\"   - Settle a resident.\n";
        std::cout << "\"evict\" - Evict the resident.\n";
        std::cout << "\"print\" - Display the trees.\n";
        std::cout << "\"exit\"  - Exit.\n";

        std::string input = "";
        std::string str1 = "";
        std::string str2 = "";
        while (true) {

            if (input == "exit") {
                break;
            }

            if (input == "find") {
                std::cout << "Enter the elf's name: > ";
                std::getline(std::cin, str1);

                find_elf(str1, trees);
            }

            if (input == "add") {
                std::cout << "Enter the elf's name to settle: > ";
                std::getline(std::cin, str1);
                std::cout << "Enter the house's name: > ";
                std::getline(std::cin, str2);


                if (settle_elf(str1, str2, trees)) {
                    std::cout << str1 << " moved into the house " << str2 << ".\n";
                }
                else {
                    std::cout << str2 << " is not to be found among the trees.\n";
                }
            }

            if (input == "evict") {
                std::cout << "Enter the elf's name to evict: > ";
                std::getline(std::cin, str1);

                if (evict_elf(str1, trees)) {
                    std::cout << str1 << " has been evicted from the village.\n";
                }
                else {
                    std::cout << str1 << " is not to be found among the villagers.\n";
                }
            }

            if (input == "print") {
                for (int i = 0; i < trees_count; ++i) {
                    trees[i]->print_branches(trees[i]);
                }
            }

            std::cout << "Command > ";
            std::getline(std::cin, input);
        }

        for (int id = 0; id < trees_count; ++id) {
            delete trees[id];
        }
        delete[] trees;
    }

    return 0;
}