#include <iostream>
#include <string>
#include <unordered_map>

bool isInteger(const std::string& str, int& number) {
    if (str.empty()) {
        return false;
    }

    auto size = str.size();

    size_t start = (str[0] == '-' || str[0] == '+') ? 1 : 0;
    size_t leadingZeros = 0;

    for (size_t i = start; i < size; ++i) {
        if (str[i] != '0') {
            break;
        }
        ++leadingZeros;
    }

    start += leadingZeros;

    size_t length = size > start + 9 ? start + 9 : size;

    if (!std::isdigit(str[start])) {
        if (leadingZeros > 0) {
            number = 0;
            return true;
        }
        return false;
    }

    ++start;

    for (size_t i = start; i < length; ++i) {
        if (!std::isdigit(str[i])) {
            number = std::stoi(str.substr(0, i - 1));
            return true;
        }
    }

    number = std::stoi(str.substr(0, length));
    return true;
}

bool getInteger(const std::string& prompt, int& number) {
    std::string input = "";
    std::cout << prompt;
    while (std::getline(std::cin, input)) {
        if (input == "exit") {
            return false;
        }

        if (isInteger(input, number)) {
            break;
        }

        std::cout << "Incorrect value!\n";
        std::cout << "Input a number in the range:\n";
        std::cout << "from -999 999 999 to 999 999 999.\n";
        std::cout << "Or type 'exit' for exit.\n";
    }
    return true;
}

bool isLeap(int year) {
    if (year % 4 > 0) {
        return false;
    }
    else {
        if (year % 100 > 0) {
            return true;
        }
        else {
            return year % 400 > 0;
        }
    }
}

std::string getMonth(int month) {
    switch (month) {
    case 1:
        return "January";
    case 2:
        return "February";
    case 3:
        return "March";
    case 4:
        return "April";
    case 5:
        return "May";
    case 6:
        return "June";
    case 7:
        return "July";
    case 8:
        return "August";
    case 9:
        return "September";
    case 10:
        return "October";
    case 11:
        return "November";
    case 12:
        return "December";
    }
    return "";
}

std::string getWeekday(int day) {
    switch (day) {
    case 1:
        return "Monday";
    case 2:
        return "Tuesday";
    case 3:
        return "Wednesday";
    case 4:
        return "Thursday";
    case 5:
        return "Friday";
    case 6:
        return "Saturday";
    case 7:
        return "Sunday";
    }
    return "";
}

bool getDate(const std::string& prompt, int& year, int& month, int& day) {
    std::string input = "";
    std::cout << prompt;
    while (std::getline(std::cin, input)) {
        if (input.size() > 0) {
            if (input == "exit") {
                return false;
            }

            int y = 0, m = 0, d = 0;
            if (isInteger(input.substr(0, 4), y)
                && isInteger(input.substr(5, 2), m)
                && isInteger(input.substr(8, 2), d)) {
                if (m > 0 && m < 13) {
                    int maxDay = 32;
                    if (m == 2) {
                        maxDay = isLeap(y) ? 30 : 29;
                    }
                    else if (m == 4 || m == 6 || m == 9 || m == 11) {
                        maxDay = 31;
                    }
                    if (d > 0 && d < maxDay) {
                        year = y;
                        month = m;
                        day = d;
                        break;
                    }
                    std::cout << "In " << getMonth(m) << " " << y << " there are " << maxDay - 1 << " days.\n";
                }
                else {
                    std::cout << "Month should be in range 1 - 12\n";
                }
            }
            else {
                std::cout << "Incorrect date!\n";
            }
            std::cout << "Enter date in YYYY/MM/DD, where 'YYYY' (year), 'MM' (month), 'DD' (day)\n";
            std::cout << "are integer numbers, '/' - any character.\n";
            std::cout << "For exit type 'exit'.\n";
        }
    }
    return true;
}

bool mayDayIsHoliday(int day, int firstWeekDayNo) {
    if (day < 6) {
        std::cout << " is May Day";
        return true;
    }
    else if (day > 7 && day < 11) {
        std::cout << " is Victory Day";
        return true;
    }
    else {
        --firstWeekDayNo;
        int weeks = (day + firstWeekDayNo) / 7;
        int weekDayNo = day + firstWeekDayNo - weeks * 7;
        std::cout << " is " << getWeekday(weekDayNo);
        return weekDayNo > 5;
    }
}

std::unordered_map<int, int> getCash(int value, int maxValue) {
    std::unordered_map<int, int> result;
    if (value > maxValue) {
        std::cout << "The maximum withdrawal amount is " << maxValue << " rubles.\n";
        std::cout << "Enter the amount no more than " << maxValue << " rubles.\n";
        return result;
    }

    int minNominal = 100;
    int banknotes = value / minNominal;
    if (value > banknotes * minNominal) {
        std::cout << "The ATM cannot issue the request ammount.\n";
        std::cout << "Enter the amount divisible by " << minNominal << "\n";
        return result;
    }

    if (value > 4999) {
        banknotes = value / 5000;
        value -= banknotes * 5000;
        result[5000] = banknotes;
    }
    if (value > 1999) {
        banknotes = value / 2000;
        value -= banknotes * 2000;
        result[2000] = banknotes;
    }
    if (value > 999) {
        banknotes = value / 1000;
        value -= banknotes * 1000;
        result[1000] = banknotes;
    }
    if (value > 499) {
        banknotes = value / 500;
        value -= banknotes * 500;
        result[500] = banknotes;
    }
    if (value > 199) {
        banknotes = value /200;
        value -= banknotes * 200;
        result[200] = banknotes;
    }
    if (value > 0) {
        result[100] = value / 100;
    }

    return result;
}

struct box {
    int a;
    int b;
    int c;
};

void sortBoxDim(box & b) {
    if (b.a > b.b) {
        std::swap(b.a, b.b);
    }
    if (b.b > b.c) {
        std::swap(b.b, b.c);
    }
    if (b.a > b.b) {
        std::swap(b.a, b.b);
    }
}

bool nestedBox(box &box1, box & box2) {
    sortBoxDim(box1);
    sortBoxDim(box2);
    if (box1.c >= box2.c) {
        return box1.a >= box2.a && box1.b >= box2.b;
    }
    else {
        return box2.a >= box1.a && box2.b >= box1.b;
    }
}

struct date {
    int y;
    int m;
    int d;
};

int getAge(const date &today, const date &birthday) {
    int fullYears = today.y - birthday.y;
    if (today.m < birthday.m) {
        --fullYears;
    }
    else if (today.m == birthday.m) {
        if (today.d <= birthday.d) {
            --fullYears;
        }
    }
    return fullYears;
}

int main() {
    std::cout << "Task 1. Check flight path.\n";
    int altitude = 0;
    int speed = 0;
    while (getInteger("Input flight altitude: ", altitude)) {
        if (altitude < 0) {
            std::cout << "A flight altitude cannot be less than 0!\n";
        } 
        else {
            break;
        }
    }
    if (altitude > 0) {
        while (getInteger("Input flight speed: ", speed)) {
            if (speed < 0) {
                std::cout << "A flight speed cannot be less than 0!\n";
            }
            else {
                break;
            }
        }
    }
    if (speed > 0) {
        std::cout << "Your flight altitude is " << altitude << " m.\n";
        std::cout << "The flight speed is " << speed << " km/h.\n";
        if (altitude > 8999 && altitude < 9501 && speed > 749 && speed < 851) {
            std::cout << "All good!\n";
        }
        else {
            if (altitude < 9000) {
                std::cout << "Climb to a height above 9000 m.\n";
            }
            else if (altitude > 9500) {
                std::cout << "Drop to a height below 9500 m.\n";
            }

            if (speed < 750) {
                std::cout << "Increase your speed above 750 km/h.\n";
            }
            else if (speed > 850) {
                std::cout << "Reduce your flight speed below 850 km/h.\n";
            }
        }
    }

    std::cout << "\nTask 2. May days. \n";
    int day = 0;
    std::cout << "Input day (number between 1 and 31): ";
    std::cin >> day;
    if (day > 0 && day < 32) {
        std::cout << "May " << day;
        if (day < 6) {
            std::cout << " is May Day holiday.\n";
        }
        else if (day > 7 && day < 11) {
            std::cout << " is Victory Day celebration.\n";
        }
        else {
            int weeks = day / 7;
            int weekdayNo = day - weeks * 7;
            std::cout << " is " << getWeekday(weekdayNo) << " - ";
            if (weekdayNo > 5) {
                std::cout << "weekend.\n";
            }
            else {
                std::cout << "working day.\n";
            }
        }
    }

    std::cout << "\nTask 3. May days.\n";
    int firstWeekDayNo = 0;
    std::cout << "Input week day number of May 1 (number between 1 and 7): ";
    std::cin >> firstWeekDayNo;
    if (firstWeekDayNo > 0 && firstWeekDayNo < 8) {
        std::cout << "May 1 is " << getWeekday(firstWeekDayNo) << "\n";
        std::cout << "Input day (number between 1 and 31): ";
        std::cin >> day;
        if (day > 0 && day < 32) {
            std::cout << "May " << day;
            if (mayDayIsHoliday(day, firstWeekDayNo)) {
                std::cout << " - holiday.\n";
            }
            else {
                std::cout << " - working day.\n";
            }
        }
    }

    std::cout << "\nTask 4. ATM machine.\n";
    int cash = 0; 
    std::cout << "Input withdrawal amount:";
    std::cin >> cash;
    auto banknotes = getCash(cash, 150000);
    if (banknotes.size() > 0) {
        std::cout << "Issued:\n";
    }

    /*
    for (auto const & [nominal, quantity] : banknotes) {
        std::cout << "'" << nominal << "': " << quantity << " notes.\n";
    }
    */
    for (auto it = banknotes.begin(); it != banknotes.end(); ++it) {
        std::cout << "'" << it->first << "': " << it->second << " notes\n";
    }

    std::cout << "\nTask 5. Boxes.\n";
    box box1 = {0, 0, 0};
    box box2 = {0, 0, 0};
    std::cout << "Input the dimensions of\n";
    std::cout << "the first box (a b c):\n";
    std::cin >> box1.a >> box1.b >> box1.c;
    std::cout << "Input the dimensions of\n";
    std::cout << "the second box (a b c):\n";
    std::cin >> box2.a >> box2.b >> box2.c;

    if (nestedBox(box1, box2)) {
        std::cout << "You can put the box into another one.\n";
    }
    else {
        std::cout << "You cannot put the box into another one.\n";
    }

    std::cout << "\nTask 6. Majority.\n";
    date today = {0, 0, 0};
    date birthday = {0, 0, 0};
    if (getDate("Enter today's date (YYYY/MM/DD):", today.y, today.m, today.d)) {
        if (getDate("Enter birthday's date (YYYY/MM/DD):", today.y, today.m, today.d)) {
            auto age = getAge(today, birthday);
            if (age >= 18) {
                std::cout << "Drinking alcohol is allowed.\n";
            }
            else {
                std::cout << "Drinking alcohol is prohibited.\n";
            }
        }
    }

    return 0;
}
