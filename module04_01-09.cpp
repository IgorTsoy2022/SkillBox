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

        std::cout << "�����४⭮� ���祭��!";
        std::cout << " �������: " << input << "\n";
        std::cout << "������ �᫮��� ���祭�� � ���������:\n";
        std::cout << "�� -999 999 999 �� 999 999 999.\n";
        std::cout << "������ 'exit' ��� ��室�.\n";

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

        std::cout << "�����४⭮� ���祭��!";
        std::cout << " �������: " << input << "\n";
        std::cout << "������ �᫮��� ���祭�� � ���������: 1 - 7.\n";
        std::cout << "������ 'exit' ��� ��室�.\n";

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

    std::cout << "����� 1. ������ �� ���� �ᥫ.\n";
    if (getInteger("������ ��ࢮ� �᫮: ", a)) {
        if (getInteger("������ ��஥ �᫮: ", b)) {
            bool areEqual = false;
            int minimum = min(a, b, areEqual);

            if (areEqual) {
                std::cout << "��᫠ ࠢ��!\n";
            }
            else {
                std::cout << "�������襥 �᫮: " <<
                    minimum << "\n";
            }
        }
    }

    std::cout << "\n����� 2. �������� ���� �ᥫ.\n";
    if (getInteger("������ ��ࢮ� �᫮: ", a)) {
        if (getInteger("������ ��஥ �᫮: ", b)) {
            int sum = 0;
            if (getInteger("������ �� �㬬�: ", sum)) {
                int correctSum = a + b;
                if (sum == correctSum) {
                    std::cout << "��୮!\n";
                }
                else {
                    std::cout << "�訡��! ���� १����: " << correctSum << "\n";
                }
            }
        }
    }

    std::cout << "\n����� 3. �஢�ઠ �� �⭮� �᫮.\n";
    if (getInteger("������ �᫮: ", a)) {
        std::cout << "-----�஢��塞-----\n";
        std::cout << "��᫮ " << a << " - ";
        if (!isEven(a)) {
            std::cout << "��";
        }
        std::cout << "�⭮�.\n";
    }

    std::cout << "\n����� 4. �������� ����.\n";
    if (getInteger("������ �᫮ �窮� ����: ", a)) {
        int experience = 1;
        std::cout << "-----�஢��塞-----\n";
        if (a >= 5000) {
            experience = 4;
        }
        else if (a >= 2500) {
            experience = 3;
        }
        else if (a >= 1000) {
            experience = 2;
        }
        std::cout << "��� �஢���: " << experience << "\n";
    }

    std::cout << "\n����� 5. ��⭮��� �᫠.\n";
    if (getInteger("������ ��ࢮ� �᫮: ", a)) {
        if (getInteger("������ ��஥ �᫮: ", b)) {
            if (b == 0) {
                std::cout << "����⪠ ������ �᫮ " << a << " �� ?0 \n";
            }
            else {
                std::cout << "��᫮ " << a;
                if (a % b > 0) {
                    std::cout << " ��";
                }
                std::cout << " ������� �� " << b << " ��� ���⪠!\n";
            }
        }
    }

    std::cout << "\n����� 6. ����� �� ��ࡥ஢.\n";
    int men = 0, barbers = 0;
    if (getInteger("������ �᫮ ��稭 � ��த�: ", men)) {
        if (getInteger("������ �᫮ ��ࡥ஢: ", barbers)) {

            int menPerBarberPerShift = 8;
            int menPerBarberPerMonth = menPerBarberPerShift * 30;
            std::cout << "���� ��ࡥ� ��ਦ�� " << menPerBarberPerMonth << " ��稭 � �����\n";

            int requiredBarbers = men % menPerBarberPerMonth > 0 ? men / menPerBarberPerMonth + 1 : men / menPerBarberPerMonth;
            std::cout << "����室���� �᫮ ��ࡥ஢: " << requiredBarbers << "\n";

            std::cout << requiredBarbers << " ��ࡥ஢ ����� ������� " << requiredBarbers * menPerBarberPerMonth << " ��稭 � �����\n";

            if (requiredBarbers > barbers) {
                std::cout << "��ࡥ஢ �� 墠⠥�\n";
            }
            else {
                std::cout << "��ࡥ஢ 墠⠥�\n";
            }
        }
    }

    std::cout << "\n����� 7. ���� ���࠭�.\n";
    int weekday = 0;
    if (getWeekDay("������ ���� ������ (�᫮ �� 1 �� 7): ", weekday)) {
        std::unordered_map <int, std::vector<std::string>> weekMenu;
        weekMenu[1] = { "ᠫ��", "�� ᠫ��", "�����誠" };
        weekMenu[2] = { "ᠫ��2", "�� ᠫ��2", "�����誠2" };
        weekMenu[3] = { "ᠫ��3", "�� ᠫ��3", "�����誠3" };
        weekMenu[4] = { "ᠫ��4", "�� ᠫ��4", "�����誠4" };
        weekMenu[5] = { "ᠫ��5", "�� ᠫ��5", "�����誠5" };
        weekMenu[6] = { "ᠫ��6", "�� ᠫ��6", "�����誠6" };
        weekMenu[7] = { "ᠫ��7", "�� ᠫ��7", "�����誠7" };

        std::cout << "���� ᥣ���� (";
        switch (weekday) {
        case 1:
            std::cout << "�������쭨�):\n";
            break;
        case 2:
            std::cout << "��୨�):\n";
            break;
        case 3:
            std::cout << "�।�):\n";
            break;
        case 4:
            std::cout << "��⢥�):\n";
            break;
        case 5:
            std::cout << "��⭨�):\n";
            break;
        case 6:
            std::cout << "�㡡��):\n";
            break;
        case 7:
            std::cout << "����ᥭ�):\n";
            break;
        }

        for (const auto& item : weekMenu[weekday]) {
            std::cout << item << "\n";
        }
    }

    std::cout << "\n����� 8. ��௫�� (�������⥫쭮� �������).\n";
    std::unordered_map<std::string, int> employees;
    int employeeNum = 1;
    std::cout << "������ ��������� ���㤭���� ('exit' ��� ��室�) :\n";
    while(true) {
        int num = 0;
        if (!getInteger("��������� " + std::to_string(employeeNum) + " ���㤭���: ",
            num)) {
            break;
        }
        employees["name" + std::to_string(employeeNum++)] = num;
//        ++employeeNum;
    }

    std::cout << "��௫��� ���㤭����:\n";
    for (const auto& [name, wage] : employees) {
        std::cout << name << " " << wage << " ��.\n";
    }

    std::cout << "--------��⠥�--------\n";
    int minWage = 999999999;
    int maxWage = 0;
    int average = 0;
    for (const auto& [_, wage] : employees) {
        minWage = minWage > wage ? wage : minWage;
        maxWage = maxWage > wage ? maxWage : wage;
        average += wage;
    }
    average /= employees.size();
    std::cout << "����� ��᮪�� ��௫��: " << maxWage << " ��.\n";
    std::cout << "������ ����� ᠬ�� ��᮪�� � ᠬ�� ������ ��௫���: " << maxWage - minWage << " ��.\n";
    std::cout << "�।��� ��௫��: " << average << " ��.\n";

    std::cout << "\n����� 9. �ண��ᨢ�� ����� (�������⥫쭮� �������).\n";
    int wage = 0;
    int tax = 0;
    if (getInteger("������ �㬬� ��室�: ", wage)) {
        if (wage > 50000) {
            tax = (wage - 50000) * 0.3 + 40000 * 0.2 + 10000 * 0.13;
        }
        else if (wage > 10000) {
            tax = (wage - 10000) * 0.2 + 10000 * 0.13;
        }
        else if (wage > 0) {
            tax = wage * 0.13;
        }
        std::cout << "�㬬� ������: " << tax << " ��.\n";
    }

    return 0;
}