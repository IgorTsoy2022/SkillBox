#include <iostream>
#include <algorithm>
#include <vector>

template<typename KeyType, typename ValueType>
class Catalog {
public:
    Catalog() {}

    Catalog(const std::initializer_list<std::pair<KeyType, ValueType>>& values)
        : data_(values)
    {}

    Catalog(const Catalog& other)
        : data_(other.data_)
    {}

    Catalog(Catalog&& other) noexcept
        : data_(std::move(other.data_))
    {}

    Catalog& operator=(const Catalog& other) {
        if (&other != this) {
            data_ = other.data_;
        }
        return *this;
    }

    Catalog& operator=(Catalog&& other) noexcept {
        if (&other != this) {
            data_ = std::move(other.data_);
        }
        return *this;
    }

    void add(const KeyType& key, const ValueType& value) {
        data_.emplace_back(std::make_pair(key, value));
    }

    void remove(const KeyType& key) {
        auto it = std::remove_if(data_.begin(), data_.end(),
            [key = key](const auto& elem) {
                return elem.first == key;
            });

        if (it != data_.end()) {
            data_.erase(it, data_.end());
        }
    }

    auto begin() const {
        return data_.begin();
    }

    auto end() const {
        return data_.end();
    }

    auto findByKey(const KeyType& key) const {
        return std::find_if(data_.begin(), data_.end(),
            [key = key](const auto& elem) {
                return elem.first == key;
            });
    }

    auto findByValue(const ValueType& value) const {
        return std::find_if(data_.begin(), data_.end(),
            [value = value](const auto& elem) {
                return elem.second == value;
            });
    }

    void print(const KeyType& key) {
        for (const auto& element : data_) {
            if (element.first == key) {
                std::cout << element.first << " : " << element.second << std::endl;
            }
        }
    }

    void print() {
        for (const auto& element : data_) {
            std::cout << element.first << " : " << element.second << std::endl;
        }
    }

    void clear() {
        data_.clear();
    }

    ~Catalog() {}
private:
    std::vector<std::pair<KeyType, ValueType>> data_{};
};

int main() {

    Catalog<int, std::string> c = { { 1, "one" }, { 2, "two" }, { 3, "three" } };
    c.add(4, "four");
    c.add(5, "five_1");
    c.add(5, "five_2");
    c.add(5, "five_3");

    std::cout << "Copy c to d:" << std::endl;
    auto d = c;
    std::cout << "c:" << std::endl;
    c.print();
    std::cout << "d:" << std::endl;
    d.print();

    std::cout << "\nMove c to e:" << std::endl;
    auto e = std::move(c);
    std::cout << "c:" << std::endl;
    c.print();
    std::cout << "e:" << std::endl;
    e.print();

    std::cout << "\nFind first key = 5" << std::endl;
    auto it = d.findByKey(5);
    if (it != d.end()) {
        std::cout << it->first << " : " << it->second << std::endl;
    }

    std::cout << "\nFind all values with key = 5" << std::endl;
    d.print(5);

    std::cout << "\nRemove all values with key = 5" << std::endl;
    d.remove(5);
    d.print();

    std::cout << "\nRemove d:" << std::endl;
    d = {};
    d.print();

    return 0;
}