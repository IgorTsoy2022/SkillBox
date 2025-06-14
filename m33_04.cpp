#include <iostream>
#include <array>
#include <string>

template<typename T, size_t N>
long double average(const std::array<T, N>& arr) {
    long double result = 0.0;
    for (const auto& number : arr) {
        result += number;
    }
    return result / N;
}

template<typename T, size_t N>
void getnumber(std::array<T, N>& arr) {
    std::string input = "";
    std::cout << "Enter " << N << " numbers." << std::endl;
    size_t count = 0;
    while (true) {
        std::cout << "Enter number > ";

        std::getline(std::cin, input);
        try {
            if (std::is_same<T, short>() ||
                std::is_same<T, int>()) {
                arr[count] = std::stoi(input);
            }
            if (std::is_same<T, long>()) {
                arr[count] = std::stol(input);
            }
            if (std::is_same<T, long long>()) {
                arr[count] = std::stoll(input);
            }
            if (std::is_same<T, float>()) {
                arr[count] = std::stof(input);
            }
            if (std::is_same<T, double>()) {
                arr[count] = std::stod(input);
            }
            if (std::is_same<T, long double>()) {
                arr[count] = std::stold(input);
            }
            std::cout << "arr[" << count << "] = " << arr[count] << std::endl;
            if (++count == N) {
                break;
            }
        }
        catch (const std::invalid_argument& e) {
            std::cerr << "Invalid argument: " << e.what() << std::endl;
        }
        catch (const std::out_of_range& e) {
            std::cerr << "Out of range: " << e.what() << std::endl;
        }
        catch (...) {
            std::cerr << "One of the string conversion functions threw an exception!" << std::endl;
        }
    }
}

int main() {
    const int SIZE = 8;

    std::array<short, SIZE> arr;
    getnumber(arr);

    std::cout << "Average number is " << average(arr) << std::endl;

    return 0;
}