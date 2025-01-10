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

        std::cout << "Некорректное значение!";
        std::cout << " Введено: " << input << "\n";
        std::cout << "Введите числовое значение в диапазоне:\n";
        std::cout << "от -999 999 999 по 999 999 999.\n";
        std::cout << "Введите 'exit' для выхода.\n";

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

        std::cout << "Некорректное значение!";
        std::cout << " Введено: " << input << "\n";
        std::cout << "Введите числовое значение в диапазоне: 1 - 7.\n";
        std::cout << "Введите 'exit' для выхода.\n";

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

    std::cout << "Задача 1. Минимум из двух чисел.\n";
    if (getInteger("Введите первое число: ", a)) {
        if (getInteger("Введите второе число: ", b)) {
            bool areEqual = false;
            int minimum = min(a, b, areEqual);

            if (areEqual) {
                std::cout << "Числа равны!\n";
            }
            else {
                std::cout << "Наименьшее число: " <<
                    minimum << "\n";
            }
        }
    }

    std::cout << "\nЗадача 2. Сложение двух чисел.\n";
    if (getInteger("Введите первое число: ", a)) {
        if (getInteger("Введите второе число: ", b)) {
            int sum = 0;
            if (getInteger("Введите их сумму: ", sum)) {
                int correctSum = a + b;
                if (sum == correctSum) {
                    std::cout << "Верно!\n";
                }
                else {
                    std::cout << "Ошибка! Верный результат: " << correctSum << "\n";
                }
            }
        }
    }

    std::cout << "\nЗадача 3. Проверка на четное число.\n";
    if (getInteger("Введите число: ", a)) {
        std::cout << "-----Проверяем-----\n";
        std::cout << "Число " << a << " - ";
        if (!isEven(a)) {
            std::cout << "не";
        }
        std::cout << "четное.\n";
    }

    std::cout << "\nЗадача 4. Калькулятор опыта.\n";
    if (getInteger("Введите число очков опыта: ", a)) {
        int experience = 1;
        std::cout << "-----Проверяем-----\n";
        if (a >= 5000) {
            experience = 4;
        }
        else if (a >= 2500) {
            experience = 3;
        }
        else if (a >= 1000) {
            experience = 2;
        }
        std::cout << "Ваш уровень: " << experience << "\n";
    }

    std::cout << "\nЗадача 5. Кратность числа.\n";
    if (getInteger("Введите первое число: ", a)) {
        if (getInteger("Введите второе число: ", b)) {
            if (b == 0) {
                std::cout << "Попытка делить число " << a << " на ?0 \n";
            }
            else {
                std::cout << "Число " << a;
                if (a % b > 0) {
                    std::cout << " не";
                }
                std::cout << " делится на " << b << " без остатка!\n";
            }
        }
    }

    std::cout << "\nЗадача 6. Задача про барберов.\n";
    int men = 0, barbers = 0;
    if (getInteger("Введите число мужчин в городе: ", men)) {
        if (getInteger("Введите число барберов: ", barbers)) {

            int menPerBarberPerShift = 8;
            int menPerBarberPerMonth = menPerBarberPerShift * 30;
            std::cout << "Один барбер стрижет " << menPerBarberPerMonth << " мужчин в месяц\n";

            int requiredBarbers = men % menPerBarberPerMonth > 0 ? men / menPerBarberPerMonth + 1 : men / menPerBarberPerMonth;
            std::cout << "Необходимое число барберов: " << requiredBarbers << "\n";

            std::cout << requiredBarbers << " барберов могут постричь " << requiredBarbers * menPerBarberPerMonth << " мужчин в месяц\n";

            if (requiredBarbers > barbers) {
                std::cout << "Барберов не хватает\n";
            }
            else {
                std::cout << "Барберов хватает\n";
            }
        }
    }

    std::cout << "\nЗадача 7. Меню ресторана.\n";
    int weekday = 0;
    if (getWeekDay("Введите день недели (число от 1 по 7): ", weekday)) {
        std::unordered_map <int, std::vector<std::string>> weekMenu;
        weekMenu[1] = { "салат", "еще салат", "вкусняшка" };
        weekMenu[2] = { "салат2", "еще салат2", "вкусняшка2" };
        weekMenu[3] = { "салат3", "еще салат3", "вкусняшка3" };
        weekMenu[4] = { "салат4", "еще салат4", "вкусняшка4" };
        weekMenu[5] = { "салат5", "еще салат5", "вкусняшка5" };
        weekMenu[6] = { "салат6", "еще салат6", "вкусняшка6" };
        weekMenu[7] = { "салат7", "еще салат7", "вкусняшка7" };

        std::cout << "Меню сегодня (";
        switch (weekday) {
        case 1:
            std::cout << "Понедельник):\n";
            break;
        case 2:
            std::cout << "Вторник):\n";
            break;
        case 3:
            std::cout << "Среда):\n";
            break;
        case 4:
            std::cout << "Четверг):\n";
            break;
        case 5:
            std::cout << "Пятница):\n";
            break;
        case 6:
            std::cout << "Суббота):\n";
            break;
        case 7:
            std::cout << "Воскресенье):\n";
            break;
        }

        for (const auto& item : weekMenu[weekday]) {
            std::cout << item << "\n";
        }
    }

    std::cout << "\nЗадача 8. Зарплата (дополнительное задание).\n";
    std::unordered_map<std::string, int> employees;
    int employeeNum = 1;
    std::cout << "Введите жалования сотрудников ('exit' для выхода) :\n";
    while(true) {
        int num = 0;
        if (!getInteger("Жалование " + std::to_string(employeeNum) + " сотрудника: ",
            num)) {
            break;
        }
        employees["name" + std::to_string(employeeNum++)] = num;
//        ++employeeNum;
    }

    std::cout << "Зарплаты сотрудников:\n";
    for (const auto& [name, wage] : employees) {
        std::cout << name << " " << wage << " руб.\n";
    }

    std::cout << "--------Считаем--------\n";
    int minWage = 999999999;
    int maxWage = 0;
    int average = 0;
    for (const auto& [_, wage] : employees) {
        minWage = minWage > wage ? wage : minWage;
        maxWage = maxWage > wage ? maxWage : wage;
        average += wage;
    }
    average /= employees.size();
    std::cout << "Самая высокая зарплата: " << maxWage << " руб.\n";
    std::cout << "Разница между самой высокой и самой низкой зарплаты: " << maxWage - minWage << " руб.\n";
    std::cout << "Средняя зарплата: " << average << " руб.\n";

    std::cout << "\nЗадача 9. Прогрессивный налог (дополнительное задание).\n";
    int wage = 0;
    int tax = 0;
    if (getInteger("Введите сумму дохода: ", wage)) {
        if (wage > 50000) {
            tax = (wage - 50000) * 0.3 + 40000 * 0.2 + 10000 * 0.13;
        }
        else if (wage > 10000) {
            tax = (wage - 10000) * 0.2 + 10000 * 0.13;
        }
        else if (wage > 0) {
            tax = wage * 0.13;
        }
        std::cout << "Сумма налога: " << tax << " руб.\n";
    }

    return 0;
}