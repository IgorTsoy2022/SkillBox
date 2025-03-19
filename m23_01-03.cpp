#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <map>
#include <set>

#define UP (1 << 0)                 // 1
#define DOWN (1 << 1)               // 2
#define LEFT (1 << 2)               // 4
#define RIGHT (1 << 3)              // 8
#define UP_LEFT (UP | LEFT)         // 5
#define UP_RIGHT (UP | RIGHT)       // 9
#define DOWN_LEFT (DOWN | LEFT)     // 6
#define DOWN_RIGHT (DOWN | RIGHT)   // 10

#define TANK(model, name) model ## _ ## name

void tiger_shoot(int count) {
    for (int i = 0; i < count; ++i) {
        std::cout << "SHOOT!\n";
    }
	}

#define SWAP(a, b) ({ decltype(a) tmp = a; a = b; b = tmp; })


bool is_number(const std::string str) {
    if (str.size() < 1) {
        return false;
    }
    for (const auto& c : str) {
        if (c >= '0' && c <= '9') {
            continue;
        }
        if (c == '.') {
            continue;
        }
        return false;
    }
    return true;
}

// Task 1. WEEKDAYS.

#define TO_STRING(text) #text
#define ENDL std::endl;

#define WEEKDAY1 "MONDAY"
#define WEEKDAY2 "TUESDAY"
#define WEEKDAY3 "WEDNESDAY"
#define WEEKDAY4 "THURSDAY"
#define WEEKDAY5 "FRIDAY"
#define WEEKDAY6 "SATURDAY"
#define WEEKDAY7 "SUNDAY"

#define PRINT_WEEKDAY(number) std::cout << WEEKDAY##number

void print_weekday(int number) {
    switch (number) {
    case 1:
        PRINT_WEEKDAY(1);
        break;
    case 2:
        PRINT_WEEKDAY(2);
        break;
    case 3:
        PRINT_WEEKDAY(3);
        break;
    case 4:
        PRINT_WEEKDAY(4);
        break;
    case 5:
        PRINT_WEEKDAY(5);
        break;
    case 6:
        PRINT_WEEKDAY(6);
        break;
    case 7:
        PRINT_WEEKDAY(7);
        break;
    default:
        break;
    }
}

// Task 2. SEASONS.

#define SUMMER

// Task 3. Analysis of car occupancy in a train.

#define PRINT(container) for (auto & [key, value] : container)  \
    std::cout << "coach #" << key << " : " << value << "\n";
#define LOOP_INPUT(function, source, destination) while (function(source, destination)) 

int main() {

    double tiger_weight = 55.0;
    std::cout << TANK(tiger, weight) << ENDL;
    TANK(tiger, weight) = 60;
    std::cout << TANK(tiger, weight) << ENDL;

    int n = 5;
    TANK(tiger, shoot)(n);

/*
    std::cout << "Task 1. Days of week.\n";
    {
        std::string input = "";
        std::cout << "Enter the day (number) of the week or \"exit\" for quit.\n";
        while (std::getline(std::cin, input)) {
            if (input == "exit") {
                break;
            }
            if (is_number(input)) {
                int number = std::stod(input);
                if (number == 0) {
                    std::cout << "Day " << number;
                }
                else if (number == 1) {
                    std::cout << "The " << number << "st";
                }
                else if (number == 2) {
                    std::cout << "The " << number << "nd";
                }
                else if (number == 3) {
                    std::cout << "The " << number << "rd";
                }
                else if (number > 3) {
                    std::cout << "The " << number << "th";
                }
                std::cout << " of the week";
                if (number == 0 || number > 7) {
                    std::cout << " doesn't exist!\n";
                }
                else {
                    std::cout << " is ";
                    print_weekday(number);
                    std::cout << ENDL;
                }
            }
        }
    }
*/

    std::cout << "\nTask 2. Seasons.\n";
    {
#ifdef SPRING
        std::cout << TO_STRING(SPRING) << ENDL;
#elif defined SUMMER
        std::cout << TO_STRING(SUMMER) << ENDL;
#elif defined AUTUMN
        std::cout << TO_STRING(AUTUMN) << ENDL;
#elif defined WINTER
        std::cout << TO_STRING(WINTER) << ENDL;
#endif
    }

    std::cout << "\nTask 3. Analysis of car occupancy in a train.\n";
    {
        const int number_of_coaches = 10;
        const int number_of_seats = 20;
        std::vector<int> passengers(number_of_coaches, 0);
        std::map<int, int> overcrowded_coaches;
        std::map<int, int> underfilled_coaches;
        std::string input = "";
        int coach_id = 1;
        int total_number_of_passengers = 0;
        std::cout << "Enter the number of passengers in 1 car: ";
        LOOP_INPUT(std::getline, std::cin, input) {
            if (input == "exit") {
                break;
            }
            if (is_number(input)) {
                int number = std::stod(input);

                if (number > number_of_seats) {
                    overcrowded_coaches[coach_id] = number;
                }
                else if (number < number_of_seats) {
                    underfilled_coaches[coach_id] = number;
                }
                total_number_of_passengers += number;
                if (coach_id == number_of_coaches) {
                    break;
                }
                ++coach_id;
            }
            else {
                std::cout << "Incorrect number!\n";
            }
            std::cout << "Enter the number of passengers in " << coach_id << " car: ";
        }

        std::cout << "Overcrowded coaches with passengers over " << number_of_seats << ":\n";
        PRINT(overcrowded_coaches);
        std::cout << "Underfilled coaches with passengers under " << number_of_seats << ":\n";
        PRINT(underfilled_coaches);
        std::cout << "Total passengers:" << total_number_of_passengers << "\n";
    }

    return 0;
}