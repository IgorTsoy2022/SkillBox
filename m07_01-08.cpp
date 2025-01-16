#include <iostream>
#include <string>

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

struct coord {
    int x;
    int y;
};

int main() {
    std::cout << "Task 1. Food stocks.\n";
    int buckwheat = 100;
    int monthlyConsumption = 4;
    int months = 0;
    std::cout << "In the beginning there were " << buckwheat << " kg of buckwheat.\n";
    std::cout << "The consumption is " << monthlyConsumption << " kg per month.\n";
    do {
        ++months;
        buckwheat -= monthlyConsumption;
        std::cout << "After the " << months << " month, the stock will remain " << buckwheat << " kg.\n";
    } while (buckwheat > 0);

    std::cout << "\nTask 2. Coffee machine.\n";
    const int waterForAmericano = 300;
    const int waterForLatte = 30;
    const int milkForLatte = 270;
    int water = 0, milk = 0;
    int americano = 0, latte = 0;
    while (getInteger("Enter the amount of water (ml): ", water)) {
        if (water < 0) {
            std::cout << "The amount cannot be less than 0\n";
        }
        else {
            break;
        }
    }
    while (getInteger("Enter the amount of milk (ml): ", milk)) {
        if (milk < 0) {
            std::cout << "The amount cannot be less than 0\n";
        }
        else {
            break;
        }
    }

    if (water > 0 && milk > 0) {
        int choose = 0;
        while (getInteger("Choose a drink (1 - Americano, 2 - Latte, 0 - turn off): ", choose)) {
            if (choose == 1) {
                if (water >= waterForAmericano) {
                    water -= waterForAmericano;
                    ++americano;
                    std::cout << "Americano is ready!\n";
                }
                else {
                    std::cout << "No igredients for Americano!\n";
                }
            }
            else if (choose == 2) {
                if (water >= waterForLatte && milk >= milkForLatte) {
                    water -= waterForLatte;
                    milk -= milkForLatte;
                    ++latte;
                    std::cout << "Latte is ready!\n";
                }
                else {
                    std::cout << "No igredients for Latte!\n";
                }
            }
            else if (choose == 0) {
                break;
            }

            if (water < waterForAmericano && (water < waterForLatte || milk < milkForLatte)) {
                std::cout << "No ingredients left for a single drink!\n";
                break;
            }
        }
        std::cout << "---------------Report---------------\n";
        std::cout << "Ingredients left:\n";
        std::cout << "                  water: " << water << " ml\n";
        std::cout << "                  milk:  " << milk << " ml\n";
        std::cout << "Americano has been cooked: " << americano << " cups\n";
        std::cout << "Latte has been cooked:     " << latte << " cups\n";
    }

    std::cout << "\nTask 3. Red Mars.\n";
    const int Xsize = 15;
    const int Ysize = 20;
    coord currentPosition = { 7, 10 };
    char c = '0';
    std::cout << "[Program]: The rover is at position (X, Y): (" << currentPosition.x << ", " << currentPosition.y << ")\n";
    while (true) {
        std::cout << "[Program]: Enter command (A - left, D - right, W - up, S - down, E - exit):\n";
        std::cout << "[Operator]: ";
        std::cin >> c;
        if (c == 'E' || c == 'e') {
            break;
        }
        switch (c) {
        case 'A':
        case 'a':
            if (currentPosition.x >= 1) {
                --currentPosition.x;
            }
            break;
        case 'D':
        case 'd':
            if (currentPosition.x < Xsize) {
            	++currentPosition.x;
            }
            break;
        case 'S':
        case 's':
            if (currentPosition.y >= 1) {
            	--currentPosition.y;
            }
            break;
        case 'W':
        case 'w':
            if (currentPosition.y < Ysize) {
            	++currentPosition.y;
            }
            break;
        default:
            std::cout << "[Program]: Wrong command!\n";
        }
        std::cout << "[Program]: The rover is at position (X, Y): (" << currentPosition.x << ", " << currentPosition.y << ")\n";
    }

    std::cout << "\nTask 4. The frame.\n";
    int width = 0;
    int height = 0;
    std::cout << "Enter the width and the height of the frame: ";
    std::cin >> width >> height;
    std::cout << "|";
    if (width > 0) {
        for (int i = 0; i < width - 2; ++i) {
            std::cout << "-";
        }
        if (width > 1) {
            std::cout << "|\n";
        }
        else {
            std::cout << "\n";
        }
    }

    for (int y = 0; y < height -2; ++y) {
        std::cout << "|";
        if (width > 1) {
            for (int x = 0; x < width - 2; ++x) {
                std::cout << " ";
            }
            std::cout << "|\n";
        }
        else {
            std::cout << "\n";
        }
    }

    if (height > 1) {
        std::cout << "|";
        for (int i = 0; i < width - 2; ++i) {
            std::cout << "-";
        }
       if (width > 1) {
           std::cout << "|\n";
       }
       else {
           std::cout << "\n";
       }
    }

    std::cout << "\nTask 5. Coordinate axes.\n";
    int maxX = 10;
    int maxY = 5;

    for (int x = 0; x < maxX - 1; ++x) {
        std::cout << " ";
    }
    std::cout << "^\n";

    for (int y = 0; y < maxY - 2; ++y) {
        for (int x = 0; x < maxX - 1; ++x) {
            std::cout << " ";
        }
        std::cout << "|\n";
    }

    for (int x = 0; x < maxX - 1; ++x) {
        std::cout << "-";
    }
    std::cout << "+";

    for (int x = 0; x < maxX - 2; ++x) {
        std::cout << "-";
    }
    std::cout << ">\n";

    for (int y = 0; y < maxY - 1; ++y) {
        for (int x = 0; x < maxX - 1; ++x) {
            std::cout << " ";
        }
        std::cout << "|\n";
    }
	
    std::cout << "\nTask 6. Important announcements.\n";
    int header = 0, exclamations = 0;
    while (getInteger("Enter the total length of the header: ", header)) {
        if (header <= 0) {
            std::cout << "The header length must be greater than zero.\n";
        }
        else {
            break;
        }
    }
    while (getInteger("Enter the number of exclamation marks: ", exclamations)) {
        if (exclamations < 0) {
            std::cout << "The number of exclamation cannot be less than zero.\n";
        }
        else if (exclamations > header) {
            std::cout << "The number of exclamation cannot be greater than the total length of the header.\n";
        }
        else {
            break;
        }
    }

    if (header > 0) {
        int left = (header - exclamations) / 2;
        int right = header - left - exclamations;
        if (left < right) {
            std::swap(left, right);
        }

        for (int i = 0; i < left; ++i) {
            std::cout << "~";
        }
        for (int i = 0; i < exclamations; ++i) {
            std::cout << "!";
        }
        for (int i = 0; i < right; ++i) {
            std::cout << "~";
        }
        std::cout << "\n";
    }

    std::cout << "\nTask 7. Biolabs.\n";
    int bacteria = 0, antibiotics = 0;
    while (getInteger("Enter the number of bacteria: ", bacteria)) {
        if (bacteria < 0) {
            std::cout << "The number of bacteria must be greater than zero.\n";
        }
        else {
            break;
        }
    }
    while (getInteger("Enter the number of antibiotics: ", antibiotics)) {
        if (antibiotics < 0) {
            std::cout << "The number of antibiotics must be greater than zero.\n";
        }
        else {
            break;
        }
    }
    if (bacteria > 0 && antibiotics > 0) {
        int hours = 0;
        int killPerHour = 10;
        while (bacteria > 0 && killPerHour > 0) {
            bacteria *= 2;
            int killed = antibiotics * killPerHour--;
            killed = bacteria > killed ? killed : bacteria;
            bacteria -= killed;

            std::cout << "After " << ++hours << " hours killed " << killed << " bacteria.";
            std::cout << "There are " << bacteria << " bacteria left.\n";
        }
    }

    std::cout << "\nTask 8. The Christmas tree.\n";
    int heightCT = 0, widthCT = 0;
    while (getInteger("Enter the height of the Christmas tree: ", heightCT)) {
        if (heightCT < 0) {
            std::cout << "The height of the Christmas tree cannot be less than zero.\n";
        }
        else {
            break;
        }
    }
    widthCT = heightCT - 1;
    for (int y = 0; y < heightCT; ++y) {
        for (int x = 0; x < widthCT - y; ++x) {
            std::cout << " ";
        }
        for (int x = 0; x < y * 2 + 1; ++x) {
            std::cout << "#";
        }
        std::cout << "\n";
    }

    return 0;
}
