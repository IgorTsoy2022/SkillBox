#include <iostream>
#include <sstream>
#include <ctime>
#include <random>
#include <map>
#include <set>
#include <string>
#include <string_view>
#include <vector>
#include <unordered_map>

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

enum class TASK {
    IDLE = 0,
    A = 1,
    B = 2,
    C = 3,
    BUSY = 100
};

class HumanNature {
public:
    HumanNature() {};

    HumanNature(const std::string_view name)
        : name_(name)
    {};

    const std::string_view get_name() const {
        return name_;
    }

    virtual ~HumanNature() {
        std::cout << "~HumanNature: " << name_ << "\n";
    };

private:
    std::string name_ = "unknown";
};

class Persons {
public:
    Persons() {};

    HumanNature* add_person(const std::string_view name) {
        if (name.empty()) {
            return nullptr;
        }
 
        if (persons_.count(name) > 0) {
            std::cout << name << " already exists.\n";
            return persons_[name];
        }

        auto person = new HumanNature(name);
        persons_[person->get_name()] = person;
        return person;
    }

    void remove_person(const std::string_view name) {
         if (persons_.count(name) == 0) {
             std::cout << name << " doesn't exist.\n";
             return;
         }
         delete persons_[name];
         persons_.erase(name);
    }

    HumanNature* get_person(const std::string_view name) {
        if (persons_.count(name) > 0) {
            return persons_[name];
        }
        return nullptr;
    }

    ~Persons() {
        for(const auto& [_, person] : persons_) {
            delete person;
        }
        std::cout << "~Persons" << std::endl;
    };

private:
    std::map<std::string_view, HumanNature*> persons_;
};

class Employee {
public:
    Employee(HumanNature* person, std::string_view division = "")
        : person_(person)
        , division_(division)
    {};

    Employee* get_employee() {
        return this;
    }

    const std::string_view get_name() {
        return person_->get_name();
    }

    void set_division(const std::string_view division) {
        division_ = division;
    }

    const std::string_view get_division() const {
        return division_;
    }

    const bool is_busy() const {
        return task_ != TASK::IDLE;
    }

    void set_task(const TASK task) {
        task_ = task;
    }

    const TASK get_task() const {
        return task_;
    }

    virtual ~Employee() {
        std::cout << "~Employee: "
                  << person_->get_name() << "\n";
    };

private:
    HumanNature* person_ = nullptr;
    std::string_view division_ = "";
    
    TASK task_ = TASK::IDLE;
};

class Manager : public Employee {
public:
    Manager(HumanNature* person, std::string_view division = "")
        : Employee(person, division)
    {};

    void set_crew(std::set<Employee*>* crew) {
        crew_ = crew;
    }

    const std::set<Employee*>* get_crew() const {
        return crew_;
    }

    const int inactive_labours() const {
        return inactive_labours_;
    }

    void inactive_labours(int numbers) {
        inactive_labours_ = numbers;
    }

    virtual ~Manager() {
        std::cout << "~Manager: "
                  << get_name() << "\n";
    };

private:
    std::set<Employee*>* crew_ = nullptr;
    int inactive_labours_ = 0;
};

class CEO : public Manager {
public:
    CEO(HumanNature* person)
        : Manager(person)
    {};

    ~CEO() {
        std::cout << "~CEO: " << get_name() << "\n";
    };
};

class Company {
public:
    Company() {
        init_tasks();
    };

    Company(Persons* persons)
        : persons_(persons)
    {
        init_tasks();
    };

    void set_persons(Persons* persons) {
        persons_ = persons;
    }

    void add_division(const std::string& division) {
        if (division.empty()) {
            return;
        }
        crews_[division];
        crews_managers_[crews_.find(division)->first];
    }

    const std::map<std::string, std::set<Employee*>>* get_divisions() const {
        return &crews_;
    }

    Employee* add_employee(const std::string& name, const std::string& division = "") {
        if (name.empty()) {
            return nullptr;
        }

        if (employees_.count(name) > 0) {
            std::cout << "Employee " << name << " already exists.\n";
            return employees_[name];
        }

        auto person = persons_->add_person(name);

        auto employee = new Employee(person, division);

        if (division.size() > 0) {
            if (crews_.count(division) == 0) {
                add_division(division);
            }
            auto it = crews_.find(division);
            employee->set_division(it->first);
            crews_[division].insert(employee);
        }

        employees_[person->get_name()] = employee;

        return employee;
    }

    void remove_employee(const std::string_view name) {
        if (employees_.count(name) == 0) {
             std::cout << "Employee " << name << " doesn't exist.\n";
             return;
        }

        for (auto& [_, employees] : crews_) {
            auto it = employees.find(employees_[name]);
            if (it != employees.end()) {
                employees.erase(it);
            }
        }

         delete employees_[name];
         employees_.erase(name);
    }

    Employee* get_employee(const std::string_view name) {
        if (employees_.count(name) > 0) {
            return employees_[name];
        }
        return nullptr;
    }

    std::string_view get_task(std::string_view name) {
        if (employees_.count(name) > 0) {
        	return tasks_[employees_[name]->get_task()];
        }
        else if (managers_.count(name) > 0) {
            return tasks_[managers_[name]->get_task()];
        }
        return "";
    }

    std::string_view get_task(Employee* employee) {
        if (employee != nullptr) {
        	return tasks_[employee->get_task()];
        }
        return "";
    }

    std::string_view get_task(Manager* manager) {
        if (manager != nullptr) {
            return tasks_[manager->get_task()];
        }
        return "";
    }

    Manager* add_manager(const std::string& name,
                         const std::string& division = "") {
        if (name.empty()) {
            return nullptr;
        }

        if (managers_.count(name) > 0) {
            std::cout << "Manager " << name << " already exists.\n";
            return managers_[name];
        }

        auto person = persons_->add_person(name);

        auto manager = new Manager(person, division);

        if (division.size() > 0) {
            if (crews_.count(division) == 0) {
                add_division(division);
            }
            auto it = crews_.find(division);
            manager->set_division(it->first);
            manager->set_crew(&crews_[division]);

            crews_managers_[it->first] = manager;
        }

        managers_[person->get_name()] = manager;
        return manager;
    }

    void remove_manager(const std::string_view name) {
        if (managers_.count(name) == 0) {
             std::cout << "Manager " << name << " doesn't exist.\n";
             return;
        }

        for (auto& [_, manager] : crews_managers_) {
            if (manager == managers_[name]) {
                manager = nullptr;
            }
        }

        delete managers_[name];
        managers_.erase(name);
    }

    Manager* get_manager(const std::string_view name) {
        if (managers_.count(name) > 0) {
            return managers_[name];
        }
        return nullptr;
    }

    void reset_inactive_labours(Manager* manager) {
        manager->set_task(TASK::IDLE);
        manager->inactive_labours(manager->get_crew()->size());
        for (const auto& employee : *manager->get_crew()) {
            employee->set_task(TASK::IDLE);
        }
    }

    void reset_inactive_labours() {
        for (const auto& [_, manager] : managers_) {
            manager->set_task(TASK::IDLE);
            manager->inactive_labours(manager->get_crew()->size());
        }
        for (const auto& [_, employee] : employees_) {
            employee->set_task(TASK::IDLE);
        }
    }

    int get_idle_labours() {
        int count = 0;
        for (const auto& [_, employee] : employees_) {
            if (employee->get_task() == TASK::IDLE) {
                ++count;
            }
        }
        return count;
    }

    CEO* appoint_ceo(const std::string_view name) {
        if (name.empty()) {
            return nullptr;
        }

        if (ceo_ != nullptr) {
            if (ceo_->get_name() == name) {
                return ceo_;
            }
            delete ceo_;
        }

        auto person = persons_->add_person(name);

        ceo_ = new CEO(person);

        return ceo_;
    }

    CEO* get_ceo() {
        return ceo_;
    }

    int execute_order(const int seed) {
        int total_tasks = 0;
        int crew_id = 1;
        for (const auto& [division, employees] : crews_) {
            int idle_count = 0;
            Manager* manager = nullptr;
            if (crews_managers_.count(division) > 0) {
                manager = crews_managers_[division];
                idle_count = manager->inactive_labours();
            }
            else {
                std::cout << division << ": no manager has been assigned.\n";
                continue;
            }
            
            if (idle_count == 0) {
                continue;
            }

            std::cout << division << ": " << manager->get_name()
                      << " has been given an assignment.\n";

            std::srand(seed + crew_id++);
            int tasks_count = std::rand() % (idle_count - 0 + 1) + 0;
            for (const auto& employee : employees) {
                if (employee->get_task() != TASK::IDLE) {
                    continue;
                }

                TASK task = TASK(std::rand() % (3 - 1 + 1) + 1);
                employee->set_task(task);
                ++total_tasks;
                --idle_count;

                std::cout << "    " << employee->get_name()
                          << " has been given an assignment: "
                          << tasks_[task] << "\n";

                if (--tasks_count == 0) {
                    break;
                }
            }

            manager->inactive_labours(idle_count);
        }
        return total_tasks;
    }

    void print_staff_list() {
        std::cout << "CEO: " << ceo_->get_name() << "\n";
        for (const auto& [division, employees] : crews_) {
            std::cout << "Division: " << division << "\n";
            if (crews_managers_.count(division) > 0) {
                auto manager = crews_managers_[division];
                std::cout << "Manager:  " << manager->get_name() << "\n";
                std::cout << "Idle employees: " << manager->inactive_labours() << "\n";
            }
            else {
                std::cout << "Manager:  no manager has been assigned.\n";
                int idle_count = 0;
                for (const auto& employee : employees) {
                    if (employee->get_task() == TASK::IDLE) {
                        ++idle_count;
                    }
                }
                std::cout << "Idle employees: " << idle_count << "\n";
            }
            std::cout << "Workers:\n";
            for (const auto& employee : employees) {
                std::cout << "          " << employee->get_name() << " "
                          << tasks_[employee->get_task()] << "\n";
            }
        }
    }

    ~Company() {
        for (const auto& [_, employee] : employees_) {
            delete employee;
        }
        for (const auto& [_, manager] : managers_) {
            delete manager;
        }
        delete ceo_;
        std::cout << "~Company\n";
    }

private:
    Persons* persons_ = nullptr;
    std::map<std::string_view, Employee*> employees_;
    std::map<std::string_view, Manager*> managers_;
    CEO* ceo_ = nullptr;
    std::map<std::string, std::set<Employee*>> crews_;
    std::map<std::string_view, Manager*> crews_managers_;

    std::map<TASK, std::string> tasks_;
    void init_tasks() {
        tasks_[TASK::IDLE] = "idle";
        tasks_[TASK::A] = "task A";
        tasks_[TASK::B] = "task B";
        tasks_[TASK::C] = "task C";
        tasks_[TASK::BUSY] = "busy";
    }
};


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
        std::cout << "~Branch: " << name_ << "\n";
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
                if (current->branches_[i]->branches_count_ < 1) {
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
    for (int i = 0; i < 5; ++i) {
        auto branch = trees[i]->find_resident(name, trees[i]);
        if (branch != nullptr) {
            branch->evict_resident(name);
            return true;
        }
    }
    return false;
}

enum class COLOR {
    NONE = 0,
    RED = 1,
    ORANGE = 2,
    YELLOW = 3,
    BROWN = 4,
    GREEN = 5,
    BLUE = 6,
    INDIGO = 7,
    VIOLET = 8
};

struct COORD {
    double x = 0.0;
    double y = 0.0;
    COORD& operator=(const COORD& right) {
        x = right.x;
        y = right.y;
    }
};

struct RECT {
    double width = 0.0;
    double height = 0.0;
    RECT& operator=(const RECT& right) {
        width = right.width;
        height = right.height;
    }
};

class Shape {
public:
    Shape() {};

    Shape(const RECT& frame, const COORD& coords = { 0.0, 0.0 })
        : frame_(frame)
        , centre_(coords)
    {};

    void color(const COLOR& color) {
        color_ = color;
    }

    const COLOR color() const {
        return color_;
    }

    void set_coords(const double x, const double y) {
        centre_.x = x;
        centre_.y = y;
    }

    void set_coords(const COORD& coords) {
        centre_ = coords;
    }

    const COORD get_coords() const {
        return centre_;
    }

    void set_rectangle(const double width, const double height) {
        frame_.width = width;
        frame_.height = height;
    }

    void set_rectangle(const RECT& frame) {
        frame_ = frame;
    }

    RECT& get_rectangle() {
        return frame_;
    }

    const double frame_square() const {
        return frame_.width * frame_.height;
    }

    ~Shape() {
        std::cout << "~Shape\n";
    };

private:
    RECT frame_ = { 0.0, 0.0 };
    COORD centre_ = { 0.0, 0.0 };
    COLOR color_ = COLOR::NONE;
};

class Rectangle : public Shape {
public:
    Rectangle() {};

    Rectangle(const RECT& sizes, const RECT& frame, 
              const COORD& coords = { 0.0, 0.0 })
        : sizes_(sizes)
        , Shape(frame, coords)
    {};

    Rectangle(const double& width, const double& height, const RECT& frame,
              const COORD& coords = { 0.0, 0.0 })
        : Shape(frame, coords)
    {
        sizes_.width = width;
        sizes_.height = height;
    };

    void set_sizes(const RECT& sizes) {
        sizes_ = sizes;
    }

    const RECT get_sizes() const {
        return sizes_;
    }

    const double square() const {
        return sizes_.width * sizes_.height;
    }

    ~Rectangle() {
        std::cout << "~Rectangle\n";
    }

private:
    RECT sizes_ = RECT{ 0.0, 0.0 };
};

class Quad : public Rectangle {
public:
    Quad() {};

    Quad(const double& width, const RECT& frame,
        const COORD& coords = { 0.0, 0.0 })
        : Rectangle(width, width, frame, coords)
    {
    };


    ~Quad() {
        std::cout << "~Quad\n";
    }

private:
};

int main() {
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

    {
        std::cout << "Task 1. Hierarchy of geometric shapes.\n";
    }
    return 0;
    {
        std::cout << "\nTask 2. Company simulation.\n";

        Persons persons{};
        Company company(&persons);

        int crews_count = 0;
        int crew_max_employees = 0;

        std::string input = "";
        while (true) {
            if (crews_count == 0) {
                std::cout << "Enter the number of crews: ";
            }
            else if (crew_max_employees == 0) {
                std::cout << "Enter the maximum number of employees in crew: ";
            }
            else {
                break;
            }
            std::getline(std::cin, input);

            if (is_number(input)) {
                if (crews_count == 0) {
                    crews_count = std::stod(input);
                }
                else {
                    crew_max_employees = std::stod(input);
                }
            }
            else {
                std::cout << "Incorrect number! Try again.\n";
            }
        }

        std::srand(std::time(nullptr));
//        std::rand() % (max - min + 1) + min;

        int name_id = 0;
        company.appoint_ceo(names[name_id++]);
        int crew_id = 0;
        for (; crew_id < crews_count; ++crew_id) {
            int employees_count = std::rand() % (crew_max_employees - 1 + 1) + 1;
            std::string division = "division#" + std::to_string(crew_id + 1);
            company.add_division(division);
            company.add_manager(names[name_id++], division);
            if (name_id == names.size()) {
                break;
            }
            for (int j = 0; j < employees_count; ++j) {
                company.add_employee(names[name_id++], division);
                if (name_id == names.size()) {
                    break;
                }
            }
            division = "";
        }
        company.reset_inactive_labours();
        int idle_labours = company.get_idle_labours();

        std::cout << "The company has " << crew_id << " divisions.\n";
        std::cout << "Total number of staff - " << name_id << " persons.\n\n";
        
        std::cout << "Commands:\n";
        std::cout << "\"order\" - Instructions from the company's head.\n";
        std::cout << "\"reset\" - Reset the staff.\n";
        std::cout << "\"print\" - Display the staff list.\n";
        std::cout << "\"exit\"  - Exit.\n";
        std::string number = "";
        while (true) {
            if (input == "exit") {
                break;
            }

            number.clear();
            std::stringstream input_stream(input);
            input_stream >> input >> number;
            if (input == "order") {
                int seed = 0;
                if (is_number(number)) {
                    seed = std::stod(number);
                }
                else {
                    while (true) {
                        std::cout << "Enter order: ";
                        std::getline(std::cin, number);

                        if (is_number(number)) {
                            seed = std::stod(number);
                            break;
                        }
                        else {
                            std::cout << "Incorrect number! Try again.\n";
                        }
                    }
                }
                idle_labours -= company.execute_order(seed);
                if (idle_labours < 1) {
                    std::cout << "\nAll employees have been given an assignment.\n";
                    company.print_staff_list();
                    break;
                }
            }

            if (input == "reset") {
                company.reset_inactive_labours();
                idle_labours = company.get_idle_labours();
            }

            if (input == "print") {
                company.print_staff_list();
            }

            std::cout << "Command > ";
            std::getline(std::cin, input);
        }
    }

    {
        std::cout << "Task 3. Elf Village.\n";
        const int trees_count = 5;
        std::srand(std::time(nullptr));
        //        std::rand() % (max - min + 1) + min;

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