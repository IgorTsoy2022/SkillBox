#include <iostream>
#include <algorithm>
#include <vector>
#include <unordered_map>

enum class SKILL {
    SWIM,
    DANCE,
    COUNT,
    NOTHING
};

class Skills {
public:
    Skills() {};

    Skills(const SKILL& skill, const std::string& skill_name)
        : skill_(skill)
        , skill_name_(skill_name)
    {};

    const SKILL get_skill() const {
        return skill_;
    }

    const std::string_view get_skill_name() const {
        return skill_name_;
    }

    virtual void can_do() const = 0;

    virtual ~Skills() {
        std::cout << "~Skills;\n";
    };
private:
    SKILL skill_ = SKILL::NOTHING;
    std::string skill_name_ = "Nothing";
};

class Swimming : public Skills {
public:
    Swimming() : Skills(SKILL::SWIM, "Swimming") {};

    void can_do() const override {
        std::cout << "I can swim.\n";
    }

    ~Swimming() {
        std::cout << "~Swimming;\n";
    };
};

class Dancing : public Skills {
public:
    Dancing() : Skills(SKILL::DANCE, "Dancing") {};

    void can_do() const override {
        std::cout << "I can dance.\n";
    }

    ~Dancing() {
        std::cout << "~Dancing;\n";
    }
};

class Counting : public Skills {
public:
    Counting() : Skills(SKILL::COUNT, "Counting") {};

    void can_do() const override {
        std::cout << "I can count.\n";
    }

    ~Counting() {
        std::cout << "~Counting; \n";
    }
};

class Dog {
public:
    Dog() {};

    Dog(std::string name) 
        : name_(name)
    {};

    void add_skill(const SKILL& value) {
        Skills* skill = nullptr;
        auto it = std::find_if(skills_.begin(), skills_.end(),
            [value](const Skills* skill) {
                return skill->get_skill() == value; });
        if (it != skills_.end()) {
            std::cout << "The dog already has the skill \""
                      << (*it)->get_skill_name() << "\".\n";
            return;
        }
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
            skills_.emplace_back(skill);
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
    std::vector<Skills*> skills_;
};

int main() {
    Dog puppy("Puppy");
    puppy.show_skills();
    Dog super_dog("Rex");
    super_dog.add_skill(SKILL::SWIM);
    super_dog.add_skill(SKILL::DANCE);
    super_dog.show_skills();
    super_dog.add_skill(SKILL::COUNT);
    super_dog.add_skill(SKILL::COUNT);
    super_dog.show_skills();

    return 0;
}