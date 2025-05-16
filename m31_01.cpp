#include <iostream>

class Toy {
public:
    Toy() {};

    Toy(const std::string& name)
        : name_(name)
    {};

    ~Toy() {
        std::cout << "~Toy: " << name_ << std::endl;
    }

private:
    std::string name_ = "unknown";
};

class Dog {
public:
    Dog(const std::string& name, std::shared_ptr<Toy> toy, int age)
        : name_(name)
        , toy_(toy)
    {
        if (age > 0 && age < 30) {
            age_ = age;
        }
    };

    Dog() : Dog("Dog", std::make_shared<Toy>("Toy"), 0) {};
    Dog(const std::string& name) : Dog(name, std::make_shared<Toy>("Toy"), 0) {};
    Dog(int age) : Dog("Dog", std::make_shared<Toy>("Toy"), age) {};

    void copyToy(const Dog& other) {
        toy_ = other.toy_;
    }

    void set_bestie(std::shared_ptr<Dog> bestie) {
        bestie_ = bestie;
    }

    ~Dog() {
        std::cout << "~Dog: " << name_ << std::endl;
    }
private:
    std::string name_ = "a dog";
    int age_ = 0;
    std::shared_ptr<Toy> toy_;
//    std::shared_ptr<Dog> bestie_;
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

        std::shared_ptr<Dog> a = std::make_shared<Dog>("a", ball, 10);
        std::shared_ptr<Dog> b = std::make_shared<Dog>("b", ball, 11);
        std::shared_ptr<Dog> c = std::make_shared<Dog>("c", ball, 12);

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

    std::shared_ptr<Toy> ball = std::make_shared<Toy>("Ball");
    std::shared_ptr<Toy> bone = std::make_shared<Toy>("Bone");

    std::cout << ball.use_count() << " dog(s) is(are) playing with this toy.\n";

    return 0;
}