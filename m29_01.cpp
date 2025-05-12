#include <iostream>
#include <set>


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
    SWIM = 1,
    DANCE = 2,
    COUNT = 3
};

class Dog {
public:
    Dog() {};

    Dog(std::string name) 
        : name_(name)
    {};

    void set_skill(const std::initializer_list<SKILL>& skills) {
        for (const auto& skill : skills) {
            switch (skill) {
            case SKILL::SWIM:
                break;
            default:
                break;
            }
        }
    }

    void show_skills() const {
        if (skills_.size() < 1) {
            std::cout << "I can't do anything.\n";
            return;
        }
        for (const auto& skill : skills_) {
            skill->can_do();
        }
    }

    ~Dog() {
        for (const auto& skill : skills_) {
            delete skill;
        }
        std::cout << "~Dog;\n";
    }
private:
    std::string name_ = "unknown";
    std::set<Skills*> skills_;
};

int main() {

    Skills* talent = new Swimming();
    talent->can_do();
    delete talent;
    return 0;
}