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


int getFibonacci(int sequenceNo) {
    if (sequenceNo < 1) {
        return 0;
    }
    if (sequenceNo < 3) {
        return 1;
    }

    int maxInt = 2147483647;
    int previous = 1;
    int current = 1;
    int next = 0;

    for (int i = 2; i < sequenceNo; ++i) {
        if (previous >= maxInt - current) {
            return -1;
        }

        next  = previous + current;
        previous = current;
        current = next;
    }

    return current;
}

int gcd(int a, int b) {
    if (b == 0) {
        return a < 0 ? -a : a;
    }
    return gcd(b, a % b);
}

int main() {
/*
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
*/

    std::cout << "\nTask 3. Red Mars.\n";
    const int Xsize = 15;
    const int Ysize = 20;
    coord currentPosition = { 7, 10 };
    char c = '0';
    while (true) {
        std::cout << "[Program]: The rover is at position (X, Y): (" << currentPosition.x << ", " << currentPosition.y << ")\n";
        if (c == 'E' || c == 'e') {
            break;
        }
        switch (c) {
        case 'A':
        case 'a':
            break;
        case 'D':
        case 'd':
            break;
        case 'W':
        case 'w':
            break;
        case 'S':
        case 's':
            break;
        default:
            std::cout << "[Program]: Wrong command!\n";
        }
        std::cout << "[Program]: Enter command (A - left, D - right, W - up, S - down, E - exit):\n";
        std::cout << "[Operator]: ";
        std::cin >> c;
    }

//*/
    return 0;

	std::cout << "\nTask 4. Lucky ticket.\n";
	int category = 6;
	int ticketNo = 0;
	if (getInteger("Enter ticket number: ", ticketNo)) {
		category /= 2;
		int digitSum = 0;
		int digit = 0;
		for (int i = 0; i < category; ++i) {
			digit = ticketNo % 10;
			ticketNo /= 10;
			digitSum += digit;
		}
		for (int i = 0; i < category; ++i) {
			digit = ticketNo % 10;
			ticketNo /= 10;
			digitSum -= digit;
		}
		if (digitSum == 0) {
			std::cout << "The ticket is lucky!\n";
		}
		else {
			std::cout << "The ticket is ordinary.\n";
		}
	}
	
	std::cout << "\nTask 5. Fibonacci numbers.\n";
	int sequenceNo = 0;
	if (getInteger("Enter sequence number: ", sequenceNo)) {
	    int fib = getFibonacci(sequenceNo);
	    if (fib < 0) {
	        std::cout << "The " << sequenceNo << " Fibonacci number causes an overflow.\n";
	    }
	    else {
	        std::cout << "Fibonacci number: " << fib << "\n";
	    }
	}
	
	std::cout << "\nTask 6. Deposits.\n";
	int deposit = 0;
	int interest = 0;
	int desirableSum = 0;
	int years = 0;
	if (getInteger("Enter deposit amount: ", deposit)) {
		if (getInteger("Enter interest rate in annual percentage: ", interest)) {
			if (getInteger("Enter the desired amount: ", desirableSum)) {
			     if (deposit > 0 && interest > 0 && desirableSum > deposit) {
                    int balance = deposit;
                    if (balance * interest / 100 > 0) {
                        while (balance <= desirableSum) {
                	        balance += balance * interest / 100;
                	        ++years;
                	        std::cout << "b = " << balance << " y = " << years << "\n";
                        }
                        std::cout << "The disarable amount one can get in " << years << " years.\n";
                    }
                    else {
                    	std::cout << "Deposit and interest rate are too small to be able to get any revenue.\n";
                    }
			    }
			}
		}
	}

	std::cout << "\nTask 7. Fraction reduction.\n";
	int numerator = 0;
	int denominator = 0;
	if (getInteger("Enter the numerator: ", numerator)) {
		if (getInteger("Enter the denominator: ", denominator)) {
			int divisor = gcd(numerator, denominator);
			if (denominator < 0) {
				denominator = - denominator;
				numerator = - numerator;
			}
			std::cout << "The result: " << numerator / divisor << " / " << denominator / divisor << "\n";
			
		}
	}
		
	std::cout << "\nTask 8. Guess the magic number.\n";
	int left = 0, right = 63;
	int hiddenNumber = 0, guessNumber = 0;
	std::string prompt = "Pick a number in the range from " + std::to_string(left) + " to " + std::to_string(right) + ": ";
	if (getInteger(prompt, hiddenNumber)) {
		int guessNumber = (right + left) / 2;
		int counter = 0;
		char c = '0';
		int quant = 1, tmp = 0;
		do {
			++counter;
			std::cout << "Is " << guessNumber << " the hidden number? (Y/N):";
			std::cin >> c;
			if (c == 'y' || c == 'Y') {
				break;
			}

			++counter;
			std::cout << "Is the hidden number greater than " << guessNumber << "? (Y/N):";
			std::cin >> c;
			if (c == 'y' || c == 'Y') {
				left = guessNumber + 1;
				quant = 1;
			}
			else if (c == 'n' || c == 'N') {
				right = guessNumber;
				quant = -1;
			}
			else {
				break;
			}
			tmp = (right + left) / 2;
			guessNumber = guessNumber == tmp ? guessNumber + quant : tmp;
		} while (guessNumber != hiddenNumber);
		std::cout << "The number of questioned asked: " << counter << "\n";
		std::cout << "The hidden number is " << guessNumber << "\n";
	}

	return 0;
}