#include <iostream>
#include <filesystem>
#include <memory>
#include <vector>
#include <unordered_set>

template<typename C>
void print(const C& container) {
    auto size = container.size();
    if (size < 1) {
        std::cout << "[ empty ]" << std::endl;
        return;
    }
    size_t count = 0;
    std::cout << "[ ";
    for (const auto& value : container) {
        std::cout << value;
        if (++count < size) {
            std::cout << ", ";
        }
    }
    std::cout << " ]" << std::endl;
}

template<typename T>
auto unique_values = [](const std::vector<T>& container) {
    auto result = std::make_unique<std::vector<T>>();
    std::unordered_set<T> checker;
    size_t total_repetitions = 0;
    for (const auto& value : container) {
        if (checker.insert(value).second) {
            result->emplace_back(value);
        }
        else {
            ++total_repetitions;
        }
    }
    std::cout << "    Total repetitions: " << total_repetitions << std::endl;
    return result;
};

auto filenames_by_extension = [](const std::filesystem::path& path,
                             const std::string& extension) {
    auto result = std::make_unique<std::vector<std::filesystem::path>>();
    for(const auto& p: std::filesystem::recursive_directory_iterator(path)) {
        if (p.is_regular_file()) {
            if (p.path().extension().compare(extension) == 0) {
                result->emplace_back(p.path().filename());
            }
        }
    }
    return result;
};

int main(int argc, char *argv[]) {

    std::cout << "Task 1. Print container." << std::endl;
    std::cout << "    Print empty vector: ";
    print(std::vector<int>{});

    std::cout << "    Print std::initializer_list: ";
    std::initializer_list<int> lst = { 1, 2, 3, 4, 5 };
    print(lst);

    std::cout << "Task 2. Create a container without repeats." << std::endl;
    std::vector<int> vec = { 5, 8, 1, 2, 3, 1, 3, 1, 1, 5, 8 };
    std::cout << "    Original vector: ";
    print(vec);
    auto values_ptr = unique_values<int>(vec);
    std::cout << "    Result vector without repeats: ";
    print(*values_ptr);

    std::cout << "Task 3. Get a list of all files, directories (and subdirectories)." << std::endl;
    std::filesystem::path path = "C:\\CPP\\GIT\\SkillBox-main";
    std::string extension = ".cpp";
    try {
        auto filenames_ptr = filenames_by_extension(path, extension);
        std::cout << "List of files in " << path  << " with extension \""
                  << extension << "\":" << std::endl;
        print(*filenames_ptr);
    }
    catch (const std::exception& e){
        std::cerr << e.what() << std::endl;
    }

    return 0;
}
