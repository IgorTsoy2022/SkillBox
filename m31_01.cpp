#include <iostream>

class Toy {
public:
    Toy() {}

    Toy(const std::string& name)
        : name_(name)
    {}

    const std::string_view name() const {
        return name_;
    }

    ~Toy() {
        std::cout << "The toy \"" << name_ << "\" has been destroyed." << std::endl;
    }
private:
    std::string name_ = "unknown";
};

class Dog {
public:
    Dog() {}

    Dog(const std::string& name, int age, std::shared_ptr<Toy> toy = nullptr)
        : name_(name)
        , toy_(toy)
    {
        if (age > 0 && age < 30) {
            age_ = age;
        }
    }

    Dog(const std::string& name) : Dog(name, 0) {}
    Dog(const std::string& name, int age, const std::string& toy)
        : Dog(name, age, std::make_shared<Toy>(toy)) {}

    const std::string_view name() const {
        return name_;
    }

    const int age() const {
        return age_;
    }

    const bool get_toy(std::shared_ptr<Toy>& toy) {
        if (toy == nullptr) {
            return false;
        }
        if (toy_ == toy) {
            std::cout << "The dog " << name_
                      << ": I already have the toy \"" << toy_name() << "\".\n";
            return false;
        }
        else if (toy.use_count() > 1) {
            std::cout << "The dog " << name_ 
                      << ": Another dog is playing with the toy \""
                      << toy->name() << "\".\n";
            return false;
        }
        else {
            toy_ = toy;
            std::cout << "The dog " << name_
                      << ": I've taken the toy \"" << toy_name() << "\".\n";
            return true;
        }
    }

    const std::shared_ptr<Toy> drop_toy() {
        auto dropped_toy = toy_;
        if (toy_ == nullptr) {
            std::cout << "The Dog " << name_ << ": Nothing to drop.\n";
        }
        else {
            std::cout << "The Dog " << name_ << ": I've dropped the toy "
                      << toy_name() << ".\n";
            toy_ = nullptr;
        }
        return dropped_toy;
    }

    void set_bestie(std::shared_ptr<Dog> bestie) {
        bestie_ = bestie;
    }

    const std::shared_ptr<Toy>& toy() const {
        return toy_;
    }

    const std::string_view toy_name() const {
        if (toy_ == nullptr) {
            return "";
        }
        return toy_->name();
    }

    ~Dog() {
        std::cout << "~Dog: " << name_ << std::endl;
    }
private:
    std::string name_ = "A dog";
    int age_ = 0;
    std::shared_ptr<Toy> toy_;
    std::weak_ptr<Dog> bestie_;
};

int main() {
    {
/*
        std::shared_ptr<Toy> ball = std::make_shared<Toy>("Ball");
        std::shared_ptr<Toy> bone = std::make_shared<Toy>("Bone");

        Dog a("a", ball, 10);
        Dog b("b", ball, 11);
        Dog c("c", ball, 12);
        Dog d("d", bone, 13);
        Dog e("e", bone, 14);

        e.copyToy(c);

        Toy* f = bone.get();

        d.copyToy(c);

        ball.reset();
        bone.reset();
*/
    }
    {
/*
        std::shared_ptr<Toy> ball = std::make_shared<Toy>("Ball");
        std::shared_ptr<Toy> bone = std::make_shared<Toy>("Bone");

        std::shared_ptr<Dog> a = std::make_shared<Dog>("a", 10, ball);
        std::shared_ptr<Dog> b = std::make_shared<Dog>("b", 11, ball);
        std::shared_ptr<Dog> c = std::make_shared<Dog>("c", 12, ball);

        a->set_bestie(b);
        b->set_bestie(a);

        Dog* pa = a.get();
        Dog* pb = b.get();

        a.reset();
        b.reset();

        ball.reset();
        bone.reset();
*/
    }
    std::cout << std::boolalpha;

    std::shared_ptr<Toy> ball = std::make_shared<Toy>("Ball");
    std::shared_ptr<Toy> bone = std::make_shared<Toy>("Bone");

    std::shared_ptr<Dog> a = std::make_shared<Dog>("Alpha", 10);
    std::shared_ptr<Dog> b = std::make_shared<Dog>("Betta", 11);

    std::cout << a->name() << "! Take the toy \"" << ball->name() << "\"!\n";
    a->get_toy(ball);
    std::cout << "    ball.use_count() = " << ball.use_count() << "\n";
    std::cout << "    a->toy().use_count() = " << a->toy().use_count() << "\n";

    std::cout << a->name() << "! Take the toy \"" << ball->name() << "\"!\n";
    a->get_toy(ball);
    std::cout << b->name() << "! Take the toy \"" << ball->name() << "\"!\n";
    b->get_toy(ball);

    std::cout << "Reset ball ptr.\n";
    ball.reset();
    std::cout << "    ball.use_count() = " << ball.use_count() << "\n";
    std::cout << "    a->toy().use_count() = " << a->toy().use_count() << "\n";
    std::cout << "    b->toy().use_count() = " << b->toy().use_count() << "\n";

    std::cout << a->name() << "! Drop the toy \"" << a->toy_name() << "\"!\n";
    ball = a->drop_toy();
    std::cout << "    ball.use_count() = " << ball.use_count() << "\n";
    std::cout << "    a->toy().use_count() = " << a->toy().use_count() << "\n";

    std::cout << b->name() << "! Drop the toy \"" << b->toy_name() << "\"!\n";
    auto another_ball = b->drop_toy();
    std::cout << "    ball.use_count() = " << ball.use_count() << "\n";
    std::cout << "    another_ball.use_count() = " << another_ball.use_count() << "\n";
    std::cout << "    a->toy().use_count() = " << a->toy().use_count() << "\n";
    std::cout << "    b->toy().use_count() = " << b->toy().use_count() << "\n";

    return 0;
}