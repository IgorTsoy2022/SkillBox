#include <iostream>
#include <sstream>
#include <exception>
#include <functional>
#include <string>
#include <map>

class Store {
public:
    void add_stock(const std::string& article, const int amount) {
        try {
            func_add_stock_(article, amount);
        }
        catch (const std::invalid_argument& e) {
            std::cerr << "Invalid argument: " << e.what() << std::endl;
        }
        catch (...) {
            std::cerr << "An exception thrown!" << std::endl;
        }
    }

    void add_stock() {
        std::string article = "";
        std::string amount_str = "";
        std::cout << "Stock filling. For exit type \"exit\"." << std::endl;
        while (true) {
            std::cout << "Enter the article and quantity of the product > ";
            std::getline(std::cin, article);
            if (article == "exit") {
                break;
            }

            std::stringstream input_stream(article);
            input_stream >> article >> amount_str;

            add_stock_(article, amount_str);
        }
    }

    void reduce_stock(const std::string& article, const int amount) {
        try {
            func_reduce_stock_(article, amount);
        }
        catch (const std::invalid_argument& e) {
            std::cerr << "Invalid argument: " << e.what() << std::endl;
        }
        catch (...) {
            std::cerr << "An exception thrown!" << std::endl;
        }
    }

    void reduce_stock() {
        std::string article = "";
        std::string amount_str = "";
        std::cout << "Stock reducing. For exit type \"exit\"." << std::endl;
        while (true) {
            if (stock_.size() == 0) {
                break;
            }
            std::cout << "Enter the article and quantity to reduce > ";
            std::getline(std::cin, article);
            if (article == "exit") {
                break;
            }

            std::stringstream input_stream(article);
            input_stream >> article >> amount_str;

            reduce_stock_(article, amount_str);
        }
    }

    void add_cart(const std::string& article, const int amount) {
        try {
            func_add_cart_(article, amount);
        }
        catch (const std::invalid_argument& e) {
            std::cerr << "Invalid argument: " << e.what() << std::endl;
        }
        catch (...) {
            std::cerr << "An exception thrown!" << std::endl;
        }
    }

    void add_cart() {
        std::string article = "";
        std::string amount_str = "";
        int amount = 0;
        std::cout << "Cart filling. For exit type \"exit\"." << std::endl;
        while (true) {
            std::cout << "Enter the article and quantity of the product > ";
            std::getline(std::cin, article);
            if (article == "exit") {
                break;
            }

            std::stringstream input_stream(article);
            input_stream >> article >> amount_str;

            add_cart_(article, amount_str);
        }
    }

    void reduce_cart(const std::string& article, const int amount) {
        try {
            func_reduce_cart_(article, amount);
        }
        catch (const std::invalid_argument& e) {
            std::cerr << "Invalid argument: " << e.what() << std::endl;
        }
        catch (...) {
            std::cerr << "An exception thrown!" << std::endl;
        }
    }

    void reduce_cart() {
        std::string article = "";
        std::string amount_str = "";
        std::cout << "Cart reducing. For exit type \"exit\"." << std::endl;
        while (true) {
            if (cart_.size() == 0) {
                break;
            }
            std::cout << "Enter the article and quantity to reduce > ";
            std::getline(std::cin, article);
            if (article == "exit") {
                break;
            }

            std::stringstream input_stream(article);
            input_stream >> article >> amount_str;

            reduce_cart_(article, amount_str);
        }
    }

    void print_stock() const {
        if (stock_.empty()) {
            std::cout << "The stock is empty." << std::endl;
            return;
        }
        std::cout << "The stock:" << std::endl;
        print_(stock_);
    }

    void print_cart() const {
        if (cart_.empty()) {
            std::cout << "The cart is empty." << std::endl;
            return;
        }
        std::cout << "The cart:" << std::endl;
        print_(cart_);
    }

private:
    std::map<std::string, int> stock_;
    std::map<std::string, int> cart_;

    void invoke_(std::function<void(const std::string &, int)> func,
                 const std::string& article, const std::string& amount_str) {
        try {
            auto amount = std::stoi(amount_str);
            try {
                func(article, amount);
            }
            catch (const std::invalid_argument& e) {
                std::cerr << "Invalid argument: " << e.what() << std::endl;
            }
            catch (...) {
                std::cerr << "An exception thrown!" << std::endl;
            }
        }
        catch (const std::invalid_argument& e) {
            std::cerr << "Invalid argument: " << e.what() << std::endl;
        }
        catch (const std::out_of_range& e) {
            std::cerr << "Out of range: " << e.what() << std::endl;
        }
        catch (...) {
            std::cerr << "std::stoi threw an exception!" << std::endl;
        }
    }

    void func_add_stock_(const std::string& article, const int amount) {
        if (amount < 0) {
            throw std::invalid_argument("Negative amount!");
        }
        if (amount == 0) {
            throw std::invalid_argument("Zero amount!");
        }

        stock_[article] += amount;
    }

    void add_stock_(const std::string& article, const std::string& amount_str) {
        auto func = std::bind(&Store::func_add_stock_, this,
                              std::placeholders::_1, std::placeholders::_2);
        invoke_(func, article, amount_str);
    }

    void func_reduce_stock_(const std::string& article, const int amount) {
        if (stock_.count(article) == 0) {
            throw std::invalid_argument("Wrong article!");
        }
        if (amount < 0) {
            throw std::invalid_argument("Negative amount!");
        }
        if (amount == 0) {
            throw std::invalid_argument("Zero amount!");
        }
        if (stock_.at(article) < amount) {
            throw std::invalid_argument("There are only " +
                  std::to_string(stock_.at(article)) + " piece(s) in stock.");
        }

        if (stock_.at(article) == amount) {
            stock_.erase(article);
        }
        else {
            stock_[article] -= amount;
        }
    }

    void reduce_stock_(const std::string& article, const std::string& amount_str) {
        auto func = std::bind(&Store::func_reduce_stock_, this,
                              std::placeholders::_1, std::placeholders::_2);
        invoke_(func, article, amount_str);
    }

    void func_add_cart_(const std::string& article, const int amount) {
        if (stock_.count(article) == 0) {
            throw std::invalid_argument("Wrong article!");
        }
        if (amount < 0) {
            throw std::invalid_argument("Negative amount!");
        }
        if (amount == 0) {
            throw std::invalid_argument("Zero amount!");
        }

        int cart_amount = cart_.count(article) > 0 ? cart_.at(article) : 0;

        if (stock_.at(article) < cart_amount + amount) {
            throw std::invalid_argument("There are only " + 
                  std::to_string(stock_.at(article)) + " piece(s) in stock.");
        }

        cart_[article] += amount;
    }

    void add_cart_(const std::string& article, const std::string& amount_str) {
        auto func = std::bind(&Store::func_add_cart_, this,
                              std::placeholders::_1, std::placeholders::_2);
        invoke_(func, article, amount_str);
    }

    void func_reduce_cart_(const std::string& article, const int amount) {
        if (cart_.count(article) == 0) {
            throw std::invalid_argument("There's no such item in the cart!");
        }
        if (amount < 0) {
            throw std::invalid_argument("Negative amount!");
        }
        if (amount == 0) {
            throw std::invalid_argument("Zero amount!");
        }
        if (cart_.at(article) == amount) {
            cart_.erase(article);
        }
        else {
            cart_[article] -= amount;
        }
    }

    void reduce_cart_(const std::string& article, const std::string& amount_str) {
        auto func = std::bind(&Store::func_reduce_cart_, this,
                              std::placeholders::_1, std::placeholders::_2);
        invoke_(func, article, amount_str);
    }

    void print_(const std::map<std::string, int>& container) const {
        for (const auto& [key, value] : container) {
            std::cout << key << " : " << value << std::endl;
        }
    }

};

int main() {

    Store a_store{};

    a_store.add_stock();
    a_store.print_stock();
    a_store.reduce_stock();
    a_store.print_stock();

    a_store.add_cart();
    a_store.print_cart();
    a_store.reduce_cart();
    a_store.print_cart();

    return 0;
}