#include <iostream>
#include "smartptr.h"

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

using shared_ptr_toy = shared_ptr<Toy>;

int main() {
    {
        shared_ptr_toy toy_01 = make_shared<Toy>("ball");
        shared_ptr_toy toy_02(toy_01);
        shared_ptr_toy toy_03("duck");
        std::cout << "=================================================" << std::endl;
        std::cout << toy_01->name() << " links:" << toy_01.use_count() << "  "
            << toy_02->name() << " links:" << toy_02.use_count() << "  "
            << toy_03->name() << " links:" << toy_03.use_count() << std::endl;
        std::cout << "=================================================" << std::endl;
        toy_02 = toy_03;
        std::cout << toy_01->name() << " links:" << toy_01.use_count() << "  "
            << toy_02->name() << " links:" << toy_02.use_count() << "  "
            << toy_03->name() << " links:" << toy_03.use_count() << std::endl;
        std::cout << "=================================================" << std::endl;
        toy_01.reset();
        std::cout << "toy_01->name()" << " links:" << toy_01.use_count() << "  "
            << toy_02->name() << " links:" << toy_02.use_count() << "  "
            << toy_03->name() << " links:" << toy_03.use_count() << std::endl;
        std::cout << "=================================================" << std::endl;
    }

    auto ball1 = shared_ptr_toy("ball1");
    auto ball2 = ball1;
    std::cout << "ball1=" << ball1->name() << " shared_count=" << ball1.use_count() << std::endl;
    std::cout << "ball2=" << ball2->name() << " shared_count=" << ball2.use_count() << std::endl;

    std::cout << "ball2=nullptr:\n";
    ball2 = nullptr;
    std::cout << std::boolalpha << "(ball2==nullptr) = " << (ball2 == nullptr) << "\n";
    std::cout << "ball1=" << ball1->name() << " shared_count=" << ball1.use_count() << std::endl;

    std::shared_ptr<Toy> p = std::make_shared<Toy>("ball");
    std::cout << "ball=" << p->name() << " shared_count=" << p.use_count() << std::endl;
    p.reset();
    std::cout << "ball=" << "p->name()" << " shared_count=" << p.use_count() << std::endl;

    return 0;
}
