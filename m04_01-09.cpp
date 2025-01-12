#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>
//#include <sstream>

bool isDigit(const char& c) {
    switch (c) {
    case '0':
        return true;
    case '1':
        return true;
    case '2':
        return true;
    case '3':
        return true;
    case '4':
        return true;
    case '5':
        return true;
    case '6':
        return true;
    case '7':
        return true;
    case '8':
        return true;
    case '9':
        return true;
    }
    return false;
}

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

    if (!isDigit(str[start]) /* !std::isdigit(str[start]) */) {
        if (leadingZeros > 0) {
            number = 0;
            return true;
        }
        return false;
    }

    //  int firstDigit = str[start] - 48;
    //  std::cout << "firstDigit = " << firstDigit << "\n";

    ++start;

    for (size_t i = start; i < length; ++i) {
        if (!isDigit(str[start]) /* !std::isdigit(str[i]) */) {
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

bool getWeekDay(const std::string& prompt, int& number) {
    std::string input = "";
    std::cout << prompt;
    while (std::getline(std::cin, input)) {
        if (input == "exit") {
            return false;
        }

        if (isInteger(input, number)) {
            if (number > 0 && number < 8) {
                break;
            }
        }

        std::cout << "Incorrect value!\n";
        std::cout << "Input a number in the range:\n";
        std::cout << "from 1 to 7.\n";
        std::cout << "Or type 'exit' for exit.\n";
    }
    return true;
}

template<typename T>
T min(T& a, T& b, bool& areEqual) {
    areEqual = false;
    if (a > b) {
        return b;
    }
    else if (a < b) {
    }
    else {
        areEqual = true;
    }
    return a;
}

bool isEven(int a) {
    return a % 2 == 0;
}

int main() {
    int a = 0, b = 0;

    std::cout << "‡ ¤ ç  1. Œ¨­¨¬ã¬ ¨§ ¤¢ãå ç¨á¥«.\n";
    if (getInteger("‚¢¥¤¨â¥ ¯¥à¢®¥ ç¨á«®: ", a)) {
        if (getInteger("‚¢¥¤¨â¥ ¢â®à®¥ ç¨á«®: ", b)) {
            bool areEqual = false;
            int minimum = min(a, b, areEqual);

            if (areEqual) {
                std::cout << "—¨á«  à ¢­ë!\n";
            }
            else {
                std::cout << " ¨¬¥­ìè¥¥ ç¨á«®: " <<
                    minimum << "\n";
            }
        }
    }

    std::cout << "\n‡ ¤ ç  2. ‘«®¦¥­¨¥ ¤¢ãå ç¨á¥«.\n";
    if (getInteger("‚¢¥¤¨â¥ ¯¥à¢®¥ ç¨á«®: ", a)) {
        if (getInteger("‚¢¥¤¨â¥ ¢â®à®¥ ç¨á«®: ", b)) {
            int sum = 0;
            if (getInteger("‚¢¥¤¨â¥ ¨å áã¬¬ã: ", sum)) {
                int correctSum = a + b;
                if (sum == correctSum) {
                    std::cout << "‚¥à­®!\n";
                }
                else {
                    std::cout << "Žè¨¡ª ! ‚¥à­ë© à¥§ã«ìâ â: " << correctSum << "\n";
                }
            }
        }
    }

    std::cout << "\n‡ ¤ ç  3. à®¢¥àª  ­  ç¥â­®¥ ç¨á«®.\n";
    if (getInteger("‚¢¥¤¨â¥ ç¨á«®: ", a)) {
        std::cout << "-----à®¢¥àï¥¬-----\n";
        std::cout << "—¨á«® " << a << " - ";
        if (!isEven(a)) {
            std::cout << "­¥";
        }
        std::cout << "ç¥â­®¥.\n";
    }

    std::cout << "\n‡ ¤ ç  4. Š «ìªã«ïâ®à ®¯ëâ .\n";
    if (getInteger("‚¢¥¤¨â¥ ç¨á«® ®çª®¢ ®¯ëâ : ", a)) {
        int experience = 1;
        std::cout << "-----à®¢¥àï¥¬-----\n";
        if (a >= 5000) {
            experience = 4;
        }
        else if (a >= 2500) {
            experience = 3;
        }
        else if (a >= 1000) {
            experience = 2;
        }
        std::cout << "‚ è ãà®¢¥­ì: " << experience << "\n";
    }

    std::cout << "\n‡ ¤ ç  5. Šà â­®áâì ç¨á« .\n";
    if (getInteger("‚¢¥¤¨â¥ ¯¥à¢®¥ ç¨á«®: ", a)) {
        if (getInteger("‚¢¥¤¨â¥ ¢â®à®¥ ç¨á«®: ", b)) {
            if (b == 0) {
                std::cout << "®¯ëâª  ¤¥«¨âì ç¨á«® " << a << " ­  ?0 \n";
            }
            else {
                std::cout << "—¨á«® " << a;
                if (a % b > 0) {
                    std::cout << " ­¥";
                }
                std::cout << " ¤¥«¨âáï ­  " << b << " ¡¥§ ®áâ âª !\n";
            }
        }
    }

    std::cout << "\n‡ ¤ ç  6. ‡ ¤ ç  ¯à® ¡ à¡¥à®¢.\n";
    int men = 0, barbers = 0;
    if (getInteger("‚¢¥¤¨â¥ ç¨á«® ¬ã¦ç¨­ ¢ £®à®¤¥: ", men)) {
        if (getInteger("‚¢¥¤¨â¥ ç¨á«® ¡ à¡¥à®¢: ", barbers)) {

            int menPerBarberPerShift = 8;
            int menPerBarberPerMonth = menPerBarberPerShift * 30;
            std::cout << "Ž¤¨­ ¡ à¡¥à áâà¨¦¥â " << menPerBarberPerMonth << " ¬ã¦ç¨­ ¢ ¬¥áïæ\n";

            int requiredBarbers = men % menPerBarberPerMonth > 0 ? men / menPerBarberPerMonth + 1 : men / menPerBarberPerMonth;
            std::cout << "¥®¡å®¤¨¬®¥ ç¨á«® ¡ à¡¥à®¢: " << requiredBarbers << "\n";

            std::cout << requiredBarbers << " ¡ à¡¥à®¢ ¬®£ãâ ¯®áâà¨çì " << requiredBarbers * menPerBarberPerMonth << " ¬ã¦ç¨­ ¢ ¬¥áïæ\n";

            if (requiredBarbers > barbers) {
                std::cout << " à¡¥à®¢ ­¥ å¢ â ¥â\n";
            }
            else {
                std::cout << " à¡¥à®¢ å¢ â ¥â\n";
            }
        }
    }

    std::cout << "\n‡ ¤ ç  7. Œ¥­î à¥áâ®à ­ .\n";
    int weekday = 0;
    if (getWeekDay("‚¢¥¤¨â¥ ¤¥­ì ­¥¤¥«¨ (ç¨á«® ®â 1 ¯® 7): ", weekday)) {
        std::unordered_map <int, std::vector<std::string>> weekMenu;
        weekMenu[1] = { "á « â", "¥é¥ á « â", "¢ªãá­ïèª " };
        weekMenu[2] = { "á « â2", "¥é¥ á « â2", "¢ªãá­ïèª 2" };
        weekMenu[3] = { "á « â3", "¥é¥ á « â3", "¢ªãá­ïèª 3" };
        weekMenu[4] = { "á « â4", "¥é¥ á « â4", "¢ªãá­ïèª 4" };
        weekMenu[5] = { "á « â5", "¥é¥ á « â5", "¢ªãá­ïèª 5" };
        weekMenu[6] = { "á « â6", "¥é¥ á « â6", "¢ªãá­ïèª 6" };
        weekMenu[7] = { "á « â7", "¥é¥ á « â7", "¢ªãá­ïèª 7" };

        std::cout << "Œ¥­î á¥£®¤­ï (";
        switch (weekday) {
        case 1:
            std::cout << "®­¥¤¥«ì­¨ª):\n";
            break;
        case 2:
            std::cout << "‚â®à­¨ª):\n";
            break;
        case 3:
            std::cout << "‘à¥¤ ):\n";
            break;
        case 4:
            std::cout << "—¥â¢¥à£):\n";
            break;
        case 5:
            std::cout << "ïâ­¨æ ):\n";
            break;
        case 6:
            std::cout << "‘ã¡¡®â ):\n";
            break;
        case 7:
            std::cout << "‚®áªà¥á¥­ì¥):\n";
            break;
        }

        for (const auto& item : weekMenu[weekday]) {
            std::cout << item << "\n";
        }
    }

    std::cout << "\n‡ ¤ ç  8. ‡ à¯« â  (¤®¯®«­¨â¥«ì­®¥ § ¤ ­¨¥).\n";
    std::unordered_map<std::string, int> employees;
    int employeeNum = 1;
    std::cout << "‚¢¥¤¨â¥ ¦ «®¢ ­¨ï á®âàã¤­¨ª®¢ ('exit' ¤«ï ¢ëå®¤ ) :\n";
    while(true) {
        int num = 0;
        if (!getInteger("† «®¢ ­¨¥ " + std::to_string(employeeNum) + " á®âàã¤­¨ª : ",
            num)) {
            break;
        }
        employees["name" + std::to_string(employeeNum++)] = num;
//        ++employeeNum;
    }

    std::cout << "‡ à¯« âë á®âàã¤­¨ª®¢:\n";
    for (const auto& [name, wage] : employees) {
        std::cout << name << " " << wage << " àã¡.\n";
    }

    std::cout << "--------‘ç¨â ¥¬--------\n";
    int minWage = 999999999;
    int maxWage = 0;
    int average = 0;
    for (const auto& [_, wage] : employees) {
        minWage = minWage > wage ? wage : minWage;
        maxWage = maxWage > wage ? maxWage : wage;
        average += wage;
    }
    average /= employees.size();
    std::cout << "‘ ¬ ï ¢ëá®ª ï § à¯« â : " << maxWage << " àã¡.\n";
    std::cout << " §­¨æ  ¬¥¦¤ã á ¬®© ¢ëá®ª®© ¨ á ¬®© ­¨§ª®© § à¯« âë: " << maxWage - minWage << " àã¡.\n";
    std::cout << "‘à¥¤­ïï § à¯« â : " << average << " àã¡.\n";

    std::cout << "\n‡ ¤ ç  9. à®£à¥áá¨¢­ë© ­ «®£ (¤®¯®«­¨â¥«ì­®¥ § ¤ ­¨¥).\n";
    int wage = 0;
    int tax = 0;
    if (getInteger("‚¢¥¤¨â¥ áã¬¬ã ¤®å®¤ : ", wage)) {
        if (wage > 50000) {
            tax = (wage - 50000) * 0.3 + 40000 * 0.2 + 10000 * 0.13;
        }
        else if (wage > 10000) {
            tax = (wage - 10000) * 0.2 + 10000 * 0.13;
        }
        else if (wage > 0) {
            tax = wage * 0.13;
        }
        std::cout << "‘ã¬¬  ­ «®£ : " << tax << " àã¡.\n";
    }

    return 0;
}
