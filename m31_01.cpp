#include <iostream>

class Toy {
public:
    Toy() {};

    Toy(const std::string& name)
        : name_(name)
    {};

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
    Dog() {};

    Dog(const std::string& name, int age, std::shared_ptr<Toy> toy = nullptr)
        : name_(name)
        , toy_(toy)
    {
        if (age > 0 && age < 30) {
            age_ = age;
        }
    };

    Dog(const std::string& name) : Dog(name, 0) {};
    Dog(const std::string& name, int age, const std::string& toy)
        : Dog(name, age, std::make_shared<Toy>(toy)) {};

    const std::string_view name() const {
        return name_;
    }

    const int age() const {
        return age_;
    }

    void get_toy(std::shared_ptr<Toy>&& toy) {
        if (toy == nullptr) {
            return;
        }
        if (toy_ == toy) {
            std::cout << "I already have this toy.\n";
        }
        else if (toy.use_count() > 0) {
            std::cout << "Another dog is playing with this toy.\n";
        }
        else {
            toy_ = std::move(toy);
        }
    }

    void copyToy(const Dog& other) {
        toy_ = other.toy_;
    }

    void set_bestie(std::shared_ptr<Dog> bestie) {
        bestie_ = bestie;
    }

    const std::string_view dogs_toy() const {
        if (toy_ == nullptr) {
            return "";
        }
        return toy_->name();
    }

    ~Dog() {
        std::cout << "~Dog: " << name_ << std::endl;
    }
private:
    std::string name_ = "a dog";
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

    std::shared_ptr<Dog> a = std::make_shared<Dog>("a", 10, ball);
    std::shared_ptr<Dog> b = std::make_shared<Dog>("b", 11, ball);


        return 0;
}