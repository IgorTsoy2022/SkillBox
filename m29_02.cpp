#include <iostream>
#include <unordered_set>

class Skills {
public:
    Skills() {};

    virtual void can_do() const = 0;

    virtual ~Skills() {
        std::cout << "~Skills;\n";
    };
};

class Swimming : public Skills {
public:
    Swimming() {};

    void can_do() const override {
        std::cout << "I can swim.\n";
    }

    ~Swimming() {
        std::cout << "~Swimming;\n";
    };
};

class Dancing : public Skills {
public:
    Dancing() {};

    void can_do() const override {
        std::cout << "I can dance.\n";
    }

    ~Dancing() {
        std::cout << "~Dancing;\n";
    }
};

class Counting : public Skills {
public:
    Counting() {};

    void can_do() const override {
        std::cout << "I can count.\n";
    }

    ~Counting() {
        std::cout << "~Counting; \n";
    }
};

enum class SKILL {
    SWIM,
    DANCE,
    COUNT,
    LAST
};

class Dog {
public:
    Dog() {};

    Dog(std::string name) 
        : name_(name)
    {};

    void set_skill(const SKILL& value) {
        Skills* skill = nullptr;
        switch (value) {
        case SKILL::SWIM:
            skill = new Swimming();
            break;
        case SKILL::DANCE:
            skill = new Dancing();
            break;
        case SKILL::COUNT:
            skill = new Counting();
            break;
        default:
            break;
        }
        if (skill != nullptr) {
            skills_.emplace(skill);
        }
    }

    void show_skills() const {
        std::cout << name_ << ":\n";
        if (skills_.size() < 1) {
            std::cout << "I can't do anything.\n";
            return;
        }
        for (const auto& skill : skills_) {
            skill->can_do();
        }
    }

    ~Dog() {
        std::cout << name_ << "'s skills:\n";
        for (const auto& skill : skills_) {
            delete skill;
        }
        std::cout << name_ << ": ~Dog;\n";
    }
private:
    std::string name_ = "unknown";
    std::unordered_set<Skills*> skills_;
};

int main() {
    Dog puppy("Puppy");
    puppy.show_skills();
    Dog super_dog("Rex");
    super_dog.set_skill(SKILL::SWIM);
    super_dog.set_skill(SKILL::DANCE);
    super_dog.show_skills();
    super_dog.set_skill(SKILL::COUNT);
    super_dog.show_skills();

    return 0;
}